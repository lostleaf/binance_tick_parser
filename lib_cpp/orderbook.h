#pragma once

#include "common.h"
#include "csv.h"

class BinanceSnapshotParser : public OrderBookHolder {
public:
  BinanceSnapshotParser(const std::string &filename, int price_multiplier)
      : OrderBookHolder(price_multiplier), csv_parser_(filename) {}

  std::vector<FullSnapshot> read_full() { return read_all<FullSnapshot>(); }

  std::vector<SimplifiedSnapshot> read_simplified() {
    return read_all<SimplifiedSnapshot>();
  }

private:
  BinanceCsvParser csv_parser_;

  template <typename TSnapshot> std::vector<TSnapshot> read_all() {
    std::vector<TSnapshot> results;
    while (csv_parser_.has_row()) {
      read_book();
      results.push_back(dump<TSnapshot>());
    }
    return results;
  }

  void read_book() {
    if (!csv_parser_.has_row())
      return;

    ob_.ask.clear();
    ob_.bid.clear();
    ob_.timestamp = csv_parser_.get_data().timestamp;
    ob_.last_update_id = csv_parser_.get_data().last_update_id;

    while (csv_parser_.has_row() &&
           csv_parser_.get_data().last_update_id == ob_.last_update_id) {
      const auto &row = csv_parser_.get_data();
      auto price_mul = std::llround(row.price * price_multiplier_);
      if (row.side == Side::Ask)
        ob_.ask[price_mul] = row.qty; // ask
      if (row.side == Side::Bid)
        ob_.bid[price_mul] = row.qty; // bid
      csv_parser_.read_row();
    }
  }
};
