# Fun C implementation of NTRUEncrypt

Don't use this for any serious purpose, really. This is a fun
implementation with the primary goal of learning.

## Building

### Dependencies

* FLINT (compiled with gmp and mpfr)
* glib-2.0
* lz4 (https://code.google.com/p/lz4)
* cunit (for the tests only)
* doxygen (for the documentation only)
* pkgconfig (for the build only)

### Compiling the library

Run ```make``` to build.

### Running the tests

Run ```make check``` to run the test suite.

### Installing the library

Run ```make install``` to install.

### Building the documentation

Run ```make doc ```. The result will be in the doxygen/html/ subfolder.

### Build Status
[![Build Status](https://travis-ci.org/hasufell/pqc.png)](https://travis-ci.org/hasufell/pqc)
