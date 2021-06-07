#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "csv.h"
#include "snapshot.h"

using AskBook = std::map<int64_t, double>;
using BidBook = std::map<int64_t, double, std::greater<int64_t>>;

struct OrderBook {
  AskBook ask;
  BidBook bid;
  int64_t timestamp, last_update_id;

  void debug_print() {
    constexpr size_t DBG_LEVELS = 5;
    printf("------------\ntimestamp: %ld last_update_id: %ld valid: %d\n",
           timestamp, last_update_id, ask.begin()->first > bid.begin()->first);
    auto iter_ask = ask.begin();
    for (size_t i = 0; i < DBG_LEVELS; i++)
      ++iter_ask;
    for (size_t i = DBG_LEVELS; i > 0; i--, --iter_ask)
      printf("Ask%02lu %ld: %lf\n", i, iter_ask->first, iter_ask->second);
    auto iter_bid = bid.begin();
    for (size_t i = 1; i <= DBG_LEVELS; i++, ++iter_bid)
      printf("Bid%02lu %ld: %lf\n", i, iter_bid->first, iter_bid->second);
    printf("------------\n");
  }
};

class BinanceSnapshotParser {
public:
  BinanceSnapshotParser(const std::string &filename, int price_multiplier)
      : csv_parser_(filename), price_multiplier_(price_multiplier) {}

  std::vector<FullSnapshot> read_full() { return read_all<FullSnapshot>(); }

  std::vector<SimplifiedSnapshot> read_simplified() {
    return read_all<SimplifiedSnapshot>();
  }

private:
  OrderBook ob_;
  BinanceCsvParser csv_parser_;
  int price_multiplier_;

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
      auto row = csv_parser_.get_data();
      auto price_mul = std::llround(row.price * price_multiplier_);
      if (row.side == Side::Ask)
        ob_.ask[price_mul] = row.qty; // ask
      if (row.side == Side::Bid)
        ob_.bid[price_mul] = row.qty; // bid
      csv_parser_.read_row();
    }
  }

  template <typename TypeSnapshot> TypeSnapshot dump() {
    TypeSnapshot s;
    s.timestamp = ob_.timestamp;
    s.last_update_id = ob_.last_update_id;
    auto ask_iter = ob_.ask.begin();
    auto bid_iter = ob_.bid.begin();
    for (size_t i = 0; i < TypeSnapshot::my_levels;
         i++, ++ask_iter, ++bid_iter) {
      s.ask_prices[i] = ask_iter->first / (double)price_multiplier_;
      s.ask_sizes[i] = ask_iter->second;
      s.bid_prices[i] = bid_iter->first / (double)price_multiplier_;
      s.bid_sizes[i] = bid_iter->second;
    }
    return s;
  }
};
