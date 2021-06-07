#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "orderbook.h"

namespace py = pybind11;

PYBIND11_MODULE(ob_ext, m) {
  PYBIND11_NUMPY_DTYPE(SimplifiedSnapshot, ask_prices, ask_sizes, bid_prices,
                       bid_sizes, timestamp, last_update_id);
  PYBIND11_NUMPY_DTYPE(FullSnapshot, ask_prices, ask_sizes, bid_prices,
                       bid_sizes, timestamp, last_update_id);
  m.def(
      "read_snapshots_full",
      [](const std::string &snap_path, int price_multiplier) {
        BinanceSnapshotParser s(snap_path, price_multiplier);
        auto snaps = s.read_full();

        // Copy from vector to numpy array, maybe optimize with capsule
        return py::array_t<FullSnapshot>(snaps.size(), snaps.data());
      },
      "Read full 1000 levels snapshots from csv/zipped csv");
  m.def(
      "read_snapshots_simplified",
      [](const std::string &snap_path, int price_multiplier) {
        BinanceSnapshotParser s(snap_path, price_multiplier);
        auto snaps = s.read_simplified();

        // Copy from vector to numpy array, maybe optimize with capsule
        return py::array_t<SimplifiedSnapshot>(snaps.size(), snaps.data());
      },
      "Read simplified snapshots from csv/zipped csv");
}