import os
from enum import Enum

from . import ext
from .default_multiplier import MULTIPLIER_MAP


class SnapshotType(Enum):
    FULL = 'full'
    SIMPLIFIED = 'simplified'


def check_valid_source(path):
    if not os.path.exists(path):
        raise ValueError('File not exists ' + path)
    if not path.endswith('.csv') and not path.endswith('.csv.zst'):
        raise ValueError('File is not csv or zstd zipped csv')


def get_multiplier(path):
    symbol = os.path.basename(path).split('_')[0]
    if symbol not in MULTIPLIER_MAP:
        raise ValueError(f'Cannot infer price multiplier for symbol {symbol}')
    return MULTIPLIER_MAP[symbol]


def read_snapshots(snap_path, snapshot_type=SnapshotType.SIMPLIFIED, price_multiplier=None):
    check_valid_source(snap_path)
    if price_multiplier is None:
        price_multiplier = get_multiplier(snap_path)
    if snapshot_type == SnapshotType.FULL:
        return ext.read_snapshots_full(snap_path, price_multiplier)
    elif snapshot_type == SnapshotType.SIMPLIFIED:
        return ext.read_snapshots_simplified(snap_path, price_multiplier)


def read_updates(update_path, full_snapshot, snapshot_type=SnapshotType.SIMPLIFIED, price_multiplier=None):
    check_valid_source(update_path)
    if price_multiplier is None:
        price_multiplier = get_multiplier(update_path)
    full_snapshot = full_snapshot.reshape((-1,))
    if snapshot_type == SnapshotType.FULL:
        return ext.read_updates_full(full_snapshot, update_path, price_multiplier)
    elif snapshot_type == SnapshotType.SIMPLIFIED:
        return ext.read_updates_simplified(full_snapshot, update_path, price_multiplier)