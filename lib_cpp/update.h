#pragma once

#include <cmath>

#include "common.h"
#include "csv.h"

class BinanceUpdateParser : public OrderBookHolder {
public:
  BinanceUpdateParser(const FullSnapshot &snapshot, const std::string &filename,
                      int price_multiplier)
      : OrderBookHolder(price_multiplier), csv_parser_(filename) {
    ob_.timestamp = snapshot.timestamp;
    ob_.last_update_id = snapshot.last_update_id;
    ob_.ask.clear();
    ob_.bid.clear();
    for (size_t i = 0; i < FullSnapshot::my_levels; i++) {
      auto ask_price_mul =
          std::llround(snapshot.ask_prices[i] * price_multiplier_);
      auto bid_price_mul =
          std::llround(snapshot.bid_prices[i] * price_multiplier_);
      ob_.ask[ask_price_mul] = snapshot.ask_sizes[i];
      ob_.bid[bid_price_mul] = snapshot.bid_sizes[i];
    }
  }

  std::vector<FullSnapshot> read_full() { return read_all<FullSnapshot>(); }

  std::vector<SimplifiedSnapshot> read_simplified() {
    return read_all<SimplifiedSnapshot>();
  }

private:
  BinanceCsvParser csv_parser_;

  template <typename TSnapshot> std::vector<TSnapshot> read_all() {
    std::vector<TSnapshot> results;
    while (csv_parser_.has_row()) {
      auto ret = read_update_batch();
      if (ret)
        results.push_back(dump<TSnapshot>());
    }
    return results;
  }

  bool read_update_batch() {
    // Skip included updates
    while (csv_parser_.has_row() &&
           csv_parser_.get_data().last_update_id < ob_.last_update_id)
      csv_parser_.read_row();

    if (!csv_parser_.has_row())
      return false;

    // Check if updates are complete
    const auto &row = csv_parser_.get_data();
    if (row.first_update_id > ob_.last_update_id &&
        row.pu != ob_.last_update_id)
      throw std::runtime_error("Some updates are missing");

    ob_.last_update_id = csv_parser_.get_data().last_update_id;
    ob_.timestamp = csv_parser_.get_data().timestamp;

    while (csv_parser_.has_row() &&
           csv_parser_.get_data().last_update_id == ob_.last_update_id) {

      auto price_mul = std::llround(row.price * price_multiplier_);

      if (row.side == Side::Ask) {
        if (row.qty == 0)
          ob_.ask.erase(price_mul);
        else
          ob_.ask[price_mul] = row.qty;
      }

      if (row.side == Side::Bid) {
        if (row.qty == 0)
          ob_.bid.erase(price_mul);
        else
          ob_.bid[price_mul] = row.qty;
      }
      csv_parser_.read_row();
    }
    return true;
  }
};
