#include <cstddef>
#include <cstdint>

constexpr size_t SIMPLIFIED_LEVELS = 20;
constexpr size_t FULL_LEVELS = 1000;

template <size_t Levels> struct Snapshot {
  double ask_prices[Levels], ask_sizes[Levels], bid_prices[Levels],
      bid_sizes[Levels];
  int64_t timestamp, last_update_id;

  static constexpr size_t my_levels = Levels;

  void debug_print(int dbg_levels=5) const {
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
