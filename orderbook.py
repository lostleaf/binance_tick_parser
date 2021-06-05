import os
from ob_ext import read_orderbook

def read(snap_path, update_path, price_multiplier):
    if not os.path.exists(snap_path) or not os.path.exists(update_path):
        raise ValueError('Path not exists')
    return read_orderbook(snap_path, update_path, price_multiplier)