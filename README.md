# Binance tick data parser

A simple C++ based parser for binance's official tick data with Python interface

First, make sure you have pybind11 and boost installed

```
conda install pybinda11 boost-cpp
```

Then, build the extensions
```
python setup.py build_ext --inplace
```

You should be able to run the test code

``` 
python test_run.py
```
