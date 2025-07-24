# mu-data-products

A C++ library to unpack raw midas data.

## Requirements

- [CMake]
- [ROOT]
- [mu-data-products]

## Cloning the repository

You can either clone this repository on its own via:

```bash
git clone https://github.com/sbfoster12/mu-unpackers.git
``` 

or you can clone the app level repository. Follow the instructions here: https://github.com/sbfoster12/mu-app

## Build instructions

If you are building this repository on its own, you should first build the mu-data-producuts library. Details can be found here: https://github.com/sbfoster12/mu-data-products.git.

You should then source the `setenv.sh` script to set up the environment, and then you can build:

```bash
source ./scripts/setenv.sh
./scripts/build.sh
```