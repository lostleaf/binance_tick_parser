#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// https://stackoverflow.com/questions/53203970/why-boostbind-insists-pulling-boostplaceholders-into-global-namespace
#define BOOST_BIND_NO_PLACEHOLDERS

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zstd.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

const std::string CSV_HEADER{"symbol,timestamp,first_update_id,last_update_id,"
                             "side,update_type,price,qty,pu"};
const std::string ROW_FMT{" %10[^,],%lld,%lld,%lld,%c,%10[^,],%lf,%lf,%lld"};
const std::string ZIP_EXT{".csv.zst"};

enum class UpdateType { Snap, Set, Delta };
enum class Side { Ask, Bid };

struct CsvRow {
  int64_t timestamp, first_update_id, last_update_id, pu;
  double price, qty;
  UpdateType update_type;
  Side side;

  void debug_print() const{
    printf("---------\n");
    printf("timestamp: %ld\n", timestamp);
    printf("first_update_id: %ld\n", first_update_id);
    printf("last_update_id: %ld\n", last_update_id);
    printf("pu: %ld\n", pu);
    printf("price: %lf\n", price);
    printf("qty: %lf\n", qty);
    printf("update_type: %s\n",
           update_type == UpdateType::Snap
               ? "snap"
               : (update_type == UpdateType::Set ? "set" : "delta"));
    printf("side: %s\n", side == Side::Ask ? "ask" : "bid");
    printf("---------\n");
  }
};

class BinanceCsvParser {
public:
  BinanceCsvParser(const std::string &filename)
      : file_(filename, std::ios_base::in | std::ios_base::binary),
        instream_(&inbuf_), has_row_(false) {

    if (std::equal(ZIP_EXT.rbegin(), ZIP_EXT.rend(), filename.rbegin()))
      inbuf_.push(boost::iostreams::zstd_decompressor());

    inbuf_.push(file_);

    std::string header;
    std::getline(instream_, header);
    if (CSV_HEADER != header)
      throw std::runtime_error(filename + " CSV header not match");

    read_row();
  }
  ~BinanceCsvParser() { file_.close(); }

  bool read_row() {
    std::vector<std::string> tokens;
    std::string line, tk;
    std::getline(instream_, line);

    if (!instream_) {
      has_row_ = false;
      return false; // readline failed
    }

    std::stringstream s(line);

    while (std::getline(s, tk, ','))
      tokens.push_back(tk);

    // Ignore tokens[0], i.e. symbol
    row_.timestamp = std::stoll(tokens[1]);
    row_.first_update_id = std::stoll(tokens[2]);
    row_.last_update_id = std::stoll(tokens[3]);

    switch (tokens[4][0]) {
    case 'a':
      row_.side = Side::Ask;
      break;
    case 'b':
      row_.side = Side::Bid;
      break;
    default:
      throw std::runtime_error("Unknown side " + tokens[4]);
    }

    switch (tokens[5][2]) { // May not correct
    case 'a':
      row_.update_type = UpdateType::Snap;
      break;
    case 't':
      row_.update_type = UpdateType::Set;
      break;
    case 'l':
      row_.update_type = UpdateType::Delta;
      break;
    default:
      throw std::runtime_error("Unknown update_type " + tokens[5]);
    }

    row_.price = std::stod(tokens[6]);
    row_.qty = std::stod(tokens[7]);
    row_.pu = std::stoll(tokens[8]);

    has_row_ = true;
    return true;
  }

  const CsvRow &get_data() { return row_; }

  const bool has_row() { return has_row_; }

private:
  std::ifstream file_;
  boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf_;
  std::istream instream_;
  CsvRow row_;
  bool has_row_;
};