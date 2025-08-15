# mu-data-products

A C++ library to unpack raw midas data.

## Requirements

- [CMake]
- [ROOT]
- [ZLIB]
- [nlohmann_json]
- [mu-data-products]

## Cloning the repository

You can either clone this repository on its own via:

```bash
git clone git@github.com:PIONEER-Experiment/mu-unpackers.git
``` 

or you can clone the app level repository. Follow the instructions here: git@github.com:PIONEER-Experiment/mu-app

## Build instructions

If you are building this repository on its own, you should first build the mu-data-producuts library. Details can be found here: git@github.com:PIONEER-Experiment/mu-data-products.git.

You should then source the `setenv.sh` script to set up the environment, and then you can build:

```bash
source ./scripts/setenv.sh
./scripts/build.sh -o
```

Building the library will produce a shared library `libunpackers.dylib` (or `libunpackers.so`) in the `build/lib` directory.

## Running the application

Building the application will create two executables, one to unpack midas files with WFD5 payloads and the other to unpack midas files with NALU payloads.

Assuming that `MY_FILE.mid` is your midas file, you can run the unpacker via (for WFD5 payloads):
```bash
./install/bin/unpack_wfd5 MY_FILE.mid 0
```
or (for NALU payloads):
```bash
./install/bin/unpack_nalu MY_FILE.mid 0
``` 
where the `0` is the verbosity level. 

Running these commands will create a ROOT file that you can use for all your analysis needs!