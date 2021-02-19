# lib3mf
![Build Status](https://github.com/3MFConsortium/lib3mf/workflows/Build/badge.svg?branch=develop)
[![Documentation Status](https://readthedocs.org/projects/lib3mf/badge/?version=master)](https://readthedocs.org/projects/lib3mf)
[![Version 2.1.1](https://img.shields.io/static/v1.svg?label=lib3mf&message=v2.1.1&color=green)]()
[![Supported platforms](https://img.shields.io/static/v1.svg?label=platform&message=windows%20%7C%20macos%20%7C%20linux&color=lightgrey)]()
[![Simplified BSD License](https://img.shields.io/static/v1.svg?label=license&message=BSD&color=green)](LICENSE)

lib3mf is a C++ implementation of the 3D Manufacturing Format file standard.

It provides 3MF reading and writing capabilities, as well as conversion and validation tools for input and output data.
lib3mf runs on Windows, Linux and MacOS and offers a clean and easy-to-use API in
[various programming languages](https://lib3mf.readthedocs.io/en/latest/#api-documentation)
to speed up the development and keep integration costs at a minimum.

As 3MF shall become an universal 3D Printing standard, its quick adoption is very important. 
This library shall lower all barriers of adoption to any possible user, let it be software 
providers, hardware providers, service providers or middleware tools.

The specification can be downloaded at
[http://3mf.io/specification/](http://3mf.io/specification/).


## Documentation
lib3mf's documentation is available on https://lib3mf.readthedocs.io.

## Obtaining lib3mf
Simply download the precompiled binary SDK https://github.com/3MFConsortium/lib3mf/releases.

## Using lib3mf
Allthough the different language bindings are kept as similar as possible,
the usage of lib3mf still depends your programming language.

You are best-off starting with one of the examples distributed in the [SDK](https://github.com/3MFConsortium/lib3mf/releases) or with this [minimal example in C++](SDK/Examples/CppDynamic/Source/ExtractInfo.cpp).

## Contributing
lib3mf is an open source project and supported by the [3MF Consortium](https://3mf.io/).

Contributions are welcome and we are always looking for people that improve the implementation of the specification and extensions of 3MF. Have a look at the [contributor's guide](CONTRIBUTING.md) for details.
This page also contains a section about building lib3mf from source or obtaining nightly snapshot builds.

## Licenses and third party code
lib3mf is released under the [BSD license](LICENSE). The library contains code of the following [third parties](SDK/Credits.txt):
1. libzip 1.2.11, https://libzip.org/license/
2. zlib 1.7.3, http://www.zlib.net/zlib_license.html
3. cpp-base64 1.01.00, https://github.com/ReneNyffenegger/cpp-base64/blob/master/LICENSE

In addition, the automated tests of lib3mf make use of LibReSSL 3.0.2, [License](Tests/libressl/COPYING).
