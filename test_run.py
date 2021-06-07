import orderbook

data = orderbook.read_snapshots('sample_data/BTCUSDT_T_DEPTH_2021-06-05_depth_snap.csv.zst')
print(type(data), data.shape)
print(data.dtype)
print(type(data[0]), data[0].shape)
print(data[0].dtype)
print(data[0])
