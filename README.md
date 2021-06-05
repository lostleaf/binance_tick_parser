# Binance orderbook

First, make sure you have pybind11 installed

```
pip install pybind11
```

Then, build the extensions
```
python setup.py build_ext --inplace
```

You should be able to run the test code by changing `SNAP_PATH` and `UPDATE_PATH` to your own path

``` python
import orderbook

SNAP_PATH = "....../BTCUSDT_T_DEPTH_yyyy-mm-dd_depth_snap.csv"
UPDATE_PATH = "....../BTCUSDT_T_DEPTH_yyyy-mm-dd_depth_update.csv"

snaps = orderbook.read(SNAP_PATH, UPDATE_PATH, 10000)
print(snaps.shape, snaps.dtype)
print(snaps[0])
```