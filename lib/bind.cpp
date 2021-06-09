#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "orderbook.h"
#include "update.h"

namespace py = pybind11;

PYBIND11_MODULE(ext, m) {
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
  m.def(
      "read_updates_full",
      [](const py::array_t<FullSnapshot> &snapshot,
         const std::string &update_path, int price_multiplier) {
        auto b = snapshot.request();
        auto sptr = (FullSnapshot *)b.ptr;
        auto tmp = snapshot.at(0);
        BinanceUpdateParser u(sptr, sptr + b.size, update_path,
                              price_multiplier);
        auto snaps = u.read_full();

        // Copy from vector to numpy array, maybe optimize with capsule
        return py::array_t<FullSnapshot>(snaps.size(), snaps.data());
      },
      "Read full snapshots with updates from csv/zipped csv");
  m.def(
      "read_updates_simplified",
      [](const py::array_t<FullSnapshot> &snapshot,
         const std::string &update_path, int price_multiplier) {
        auto b = snapshot.request();
        auto sptr = (FullSnapshot *)b.ptr;
        auto tmp = snapshot.at(0);
        BinanceUpdateParser u(sptr, sptr + b.size, update_path,
                              price_multiplier);
        auto snaps = u.read_simplified();
        auto last_full_snapshot = u.get_full();

        // Copy from vector to numpy array, maybe optimize with capsule
        return py::make_tuple(
            py::array_t<SimplifiedSnapshot>(snaps.size(), snaps.data()),
            py::array_t<FullSnapshot>(1, &last_full_snapshot));
      },
      "Read simplified snapshots with updates from csv/zipped csv");
  m.def("show_array_info", [](const py::array_t<FullSnapshot> &snapshots) {
    using namespace std;
    auto b = snapshots.request();
    cout << "ptr: " << b.ptr << endl // Pointer to the underlying storage
         << "itemsize: " << b.itemsize
         << endl                       // Size of individual items in bytes
         << "size: " << b.size << endl // Total number of entries
         << "format: " << b.format
         << endl // For homogeneous buffers, this should be set to
                 // format_descriptor<T>::format()
         << "ndim: " << b.ndim << endl // Number of dimensions
         << "shape size: " << b.shape.size()
         << endl // Shape of the tensor (1 entry per dimension)
         << "shape: " << b.shape[0] << endl
         << "strides size: " << b.strides.size()
         << endl // Number of bytes between adjacent entries (for each
                 // per dimension)
         << "strides : " << b.strides[0] << endl
         << "readonly: " << b.readonly
         << endl // flag to indicate if the underlying storage may be
                 // written to
        ;
  });
}