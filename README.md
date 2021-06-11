# Binance tick data parser

A simple C++ based parser for binance's official tick data with Python interface

## Environment

First, make sure you have `numpy`, `pybind11` and `boost` installed. You may make a new environment by

```
conda create -n binance_tick boost-cpp pybind11 numpy
conda activate binance_tick
```

Also make sure you have compilers installed. For example, you should install the `build-essential` package under Ubuntu

```
sudo apt install build-essential
```

## Download

You can simply clone the repository by

```
git clone https://github.com/lostleaf/binance_tick_parser.git
```

Optionally, since the sample data is held by [Git Large File System](https://git-lfs.github.com/), you will have to install the git-lfs extension to run the test code with sample data

```
curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash
sudo apt update
sudo apt install git-lfs
```

And get the sample data by

```
git lfs pull
```

## Run

First, build the extensions
```
python setup.py build_ext --inplace
```

You should be able to run the test code

``` 
python test_run.py
```
