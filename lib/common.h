#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <map>

constexpr size_t SIMPLIFIED_LEVELS = 20;
constexpr size_t FULL_LEVELS = 1000;

template <size_t Levels> struct Snapshot {
  double ask_prices[Levels], ask_sizes[Levels], bid_prices[Levels],
      bid_sizes[Levels];
  int64_t timestamp, last_update_id;

  static constexpr size_t my_levels = Levels;

  void debug_print(int dbg_levels = 5) const {
    printf("------------\ntimestamp: %ld last_update_id: %ld valid: %d\n",
           timestamp, last_update_id, ask_prices[0] > bid_prices[0]);

    for (int i = dbg_levels - 1; i >= 0; i--)
      printf("Ask%02lu %lf: %lf\n", i + 1, ask_prices[i], ask_sizes[i]);

    for (int i = 0; i < dbg_levels; i++)
      printf("Bid%02lu %lf: %lf\n", i + 1, bid_prices[i], bid_sizes[i]);
    printf("------------\n");
  }
};

using SimplifiedSnapshot = Snapshot<SIMPLIFIED_LEVELS>;
using FullSnapshot = Snapshot<FULL_LEVELS>;

using AskBook = std::map<int64_t, double>;
using BidBook = std::map<int64_t, double, std::greater<int64_t>>;

struct OrderBook {
  AskBook ask;
  BidBook bid;
  int64_t timestamp, last_update_id;

  void debug_print(int dbg_levels = 5) {
    printf("------------\ntimestamp: %ld last_update_id: %ld valid: %d\n",
           timestamp, last_update_id, ask.begin()->first > bid.begin()->first);
    auto iter_ask = ask.begin();
    for (int i = 0; i < dbg_levels; i++)
      ++iter_ask;
    for (int i = dbg_levels; i > 0; i--, --iter_ask)
      printf("Ask%02d %ld: %lf\n", i, iter_ask->first, iter_ask->second);
    auto iter_bid = bid.begin();
    for (int i = 1; i <= dbg_levels; i++, ++iter_bid)
      printf("Bid%02d %ld: %lf\n", i, iter_bid->first, iter_bid->second);
    printf("------------\n");
  }
};

class OrderBookHolder {
protected:
  OrderBookHolder(int price_multiplier) : price_multiplier_(price_multiplier) {}
  OrderBook ob_;
  const int price_multiplier_;

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