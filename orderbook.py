import os
from enum import Enum
import ob_ext

MULTIPLIER_MAP = {'BTCUSDT': 10000, 'ETHUSDT': 10000}


class SnapshotType(Enum):
    FULL = 'full'
    SIMPLIFIED = 'simplified'


def read_snapshots(snap_path, snapshot_type=SnapshotType.SIMPLIFIED, price_multiplier=None):
    if not os.path.exists(snap_path):
        raise ValueError('File not exists')
    if not snap_path.endswith('.csv') and not snap_path.endswith('.csv.zst'):
        raise ValueError('File is not csv or zstd zipped csv')
    if price_multiplier is None:
        symbol = os.path.basename(snap_path).split('_')[0]
        if symbol not in MULTIPLIER_MAP:
            raise ValueError(f'Cannot infer price multiplier for symbol {symbol}')
        price_multiplier = MULTIPLIER_MAP[symbol]
    if snapshot_type == SnapshotType.FULL:
        return ob_ext.read_snapshots_full(snap_path, price_multiplier)
    elif snapshot_type == SnapshotType.SIMPLIFIED:
        return ob_ext.read_snapshots_simplified(snap_path, price_multiplier)
