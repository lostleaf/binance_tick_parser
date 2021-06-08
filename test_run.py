import numpy as np

from binance_tick_parser import *

data = read_snapshots('sample_data/BTCUSDT_T_DEPTH_2021-06-05_depth_snap.csv.zst', SnapshotType.FULL)
print(type(data), data.shape)
print(data.dtype)
print(type(data[0]), data[0].shape)
print(data[0].dtype)

data = read_updates('sample_data/BTCUSDT_T_DEPTH_2021-06-05_depth_update.csv.zst', data[0], SnapshotType.SIMPLIFIED)
print(type(data), data.shape)
print(data.dtype)
print(type(data[0]), data[0].shape)
print(data[0].dtype)
print(data[0])