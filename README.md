# lib3mf
[![Build Status](https://travis-ci.org/3MFConsortium/lib3mf.svg?branch=master)](https://travis-ci.org/3MFConsortium/lib3mf)
[![Documentation Status](https://readthedocs.org/projects/lib3mf/badge/?version=latest)](https://readthedocs.org/projects/lib3mf)
[![Supported platforms](https://img.shields.io/static/v1.svg?label=platform&message=windows%20%7C%20macos%20%7C%20linux&color=lightgrey)]()
[![Simplified BSD License](https://img.shields.io/static/v1.svg?label=license&message=BSD&color=green)](LICENSE)

Lib3MF is a C++ implementation of the 3D Manufacturing Format file standard.

It offers an open source way to integrate 3MF reading and writing capabilities, as well 
as conversion and validation tools for input and output data.
clean and easy-to-use API to speed up the development and keep integration costs at a minimum.

As 3MF shall become an universal 3D Printing standard, its quick adoption is very important. 
This library shall lower all barriers of adoption to any possible user, let it be software 
providers, hardware providers, service providers or middleware tools.
The specification can be downloaded at
[http://3mf.io/specification/](http://3mf.io/specification/)


## Using lib3MF
The current code runs on Windows, Linux and Mac and has a small number of external dependencies.

### 1. Obtain dependencies
* Unix: [uuid](https://linux.die.net/man/3/uuid)
    * Linux (Ubuntu/Debian): `sudo apt-get install uuid-dev`
    * MAC: `brew install ossp-uuid`

### 2. Download the precompiled binary SDK of lib3mf
You can download the compiled shared libarary as part of a minimal SDK from the [official releases](https://github.com/3MFConsortium/lib3mf/releases),
or use the [nightly builds](https://github.com/3MFConsortium/lib3mf-binaries).

### 3. Examples 
You are best of starting with one of the examples that are shipped with the binary SDK.
From there, you should be able to include lib3mf in your host application or service.

## Contributing
lib3mf an open source project and supported by the [3MF Consortium](https://3mf.io/).

Contributions are welcome and we are always looking for people that improve the implementation of the specification and extensions of 3MF. Have a look at the [contributor's guide](CONTRIBUTING.md) for details.
This guide also contains a section about building lib3mf from source.

## Documentation
Lib3MF's documentation is hosted on https://lib3mf.readthedocs.io.
