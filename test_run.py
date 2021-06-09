import numpy as np

from binance_tick_parser import *
import binance_tick_parser.ext as ext

data = read_snapshots('sample_data/BTCUSDT_T_DEPTH_2021-06-05_depth_snap.csv.zst', SnapshotType.FULL)
print(data.shape)
print(data.dtype)
ext.test(data)

data, last_full_snapshot = read_updates('sample_data/BTCUSDT_T_DEPTH_2021-06-05_depth_update.csv.zst',
                                       data[0],
                                       SnapshotType.SIMPLIFIED,
                                       last_full=True)
print(data.shape)
print(data.dtype)
print(convert_to_dataframe(data))
print(convert_to_dataframe(last_full_snapshot.reshape((-1,))).iloc[0])