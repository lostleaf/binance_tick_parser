#! /bin/bash

rm -fr binance_tick_parser/ext.cpython-39-x86_64-linux-gnu.so 
rm -fr build 
python setup.py build_ext --inplace --force 
python test_run.py