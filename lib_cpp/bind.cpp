#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
// #define DEBUG_FLAG
#include "orderbook.h"

namespace py = pybind11;

PYBIND11_MODULE(ob_ext, m) {
  PYBIND11_NUMPY_DTYPE(BookSnapshot, ask_prices, ask_sizes, bid_prices,
                       bid_sizes, timestamp, last_update_id);
  m.def(
      "read_orderbook",
      [](const std::string &snap_path, const std::string &update_path,
         int price_multiplier) {
        Parser p(price_multiplier);
        auto snaps = p.parse(snap_path, update_path);

        // Copy from vector to numpy array, maybe optimize with capsule
        return py::array_t<BookSnapshot>(snaps.size(), snaps.data());
      },
      "Read binance from snapshots and updates csv");
}