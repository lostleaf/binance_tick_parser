import numpy as np

from binance_tick_parser import *
import binance_tick_parser.ext as ext

data = read_snapshots('sample_data/BTCUSDT_T_DEPTH_2021-06-02_depth_snap.csv.zst', SnapshotType.FULL)
print(data.shape)
print(data.dtype)
ext.show_array_info(data)

p = 'sample_data/BTCUSDT_T_DEPTH_2021-06-02_depth_update.csv.zst'
data, last_full_snapshot = read_updates(p, data, SnapshotType.SIMPLIFIED, last_full=True)
print(data.shape)
print(data.dtype)
print(convert_to_dataframe(last_full_snapshot.reshape((-1,))).iloc[0])