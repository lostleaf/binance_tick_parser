#pragma once

#include "common.h"
#include "csv.h"

class BinanceUpdateParser : public OrderBookHolder {
public:
  BinanceUpdateParser(FullSnapshot *anchor_begin, FullSnapshot *anchor_end,
                      const std::string &filename, int price_multiplier)
      : OrderBookHolder(price_multiplier), csv_parser_(filename),
        snap_anchors_(anchor_begin, anchor_end), snap_idx_(0) {
    init_book_from_snapshot(snap_anchors_[0]);
  }

  std::vector<FullSnapshot> read_full() { return read_all<FullSnapshot>(); }

  std::vector<SimplifiedSnapshot> read_simplified() {
    return read_all<SimplifiedSnapshot>();
  }

  FullSnapshot get_full() { return dump<FullSnapshot>(); }

private:
  BinanceCsvParser csv_parser_;
  const std::vector<FullSnapshot> snap_anchors_;
  size_t snap_idx_;

  void init_book_from_snapshot(const FullSnapshot &snapshot) {
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

  template <typename TSnapshot> std::vector<TSnapshot> read_all() {
    std::vector<TSnapshot> results;
    while (csv_parser_.has_row()) {
      auto ret = read_update_batch();
      if (ret) { // Successfully updated
        results.push_back(dump<TSnapshot>());
      } else if (csv_parser_.has_row()) {
        // Still have rows to read, must loss updates
        auto &first_id = csv_parser_.get_data().first_update_id;
        while (snap_idx_ + 1 <= snap_anchors_.size() &&
               snap_anchors_[snap_idx_ + 1].last_update_id < first_id)
          snap_idx_++;

        if (snap_anchors_[snap_idx_].last_update_id < first_id)
          break; // There is no suitalbe anchors left
        
        init_book_from_snapshot(snap_anchors_[snap_idx_]);
      }
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
        row.pu != ob_.last_update_id) {
      std::stringstream ss;
      ss << "Updates lost, row.first_update_id=" << row.first_update_id
         << ", ob_.last_update_id=" << std::to_string(ob_.last_update_id);
      std::cerr << ss.str() << std::endl;
      return false;
    }

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
