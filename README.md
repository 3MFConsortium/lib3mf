# lib3MF
[![Build Status](https://travis-ci.org/3MFConsortium/lib3mf.svg?branch=master)](https://travis-ci.org/3MFConsortium/lib3mf)

Lib3MF is a C++ implementation of the 3D Manufacturing Format file standard.

As 3MF shall become an universal 3D Printing standard, its quick adoption is very important. 
This library shall lower all barriers of adoption to any possible user, let it be software 
providers, hardware providers, service providers or middleware tools.
The specification can be downloaded at
[http://3mf.io/specification/](http://3mf.io/specification/)

Its aim is to offer an open source way to integrate 3MF reading and writing capabilities, as well 
as conversion and validation tools for input and output data. The 3MF Library shall provide a 
clean and easy-to-use API to speed up the development and keep integration costs at a minimum.

A lot of energy has been put into keeping it as platform independent as far as possible. For
example, it compiles well with the GCC compiler, but there is some work left to recode a few
platform specific functionalities, which are now covered by the WinRT platform (like XML parsing
and ZIP compression). This will come when time passes.

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
You are best of starting with on of the examples that are shipped with the binary SDK.
From there, you should be able to include lib3mf in your host application or service.

## Contributing
lib3mf an open source project and supported by the [3MF Consortium](https://3mf.io/).

Contributions are welcome and we are always looking for people that improve the implementation of the specification and extensions of 3MF. Have a look at the [contributor's guide](CONTRIBUTING.md) for details.
This guide also contains a section about building lib3mf from source.

## Documentation
For a more detailed documentation, please take a look into the [PDF](Lib3MF-1.pdf) in the root directory
of this repository.

## License
Lib3MF is licensed under the [simplified BSD license](LICENSE).

## Products using Lib3MF
A large number of software or hardware products and projects [support 3MF](https://3mf.io/adoption/), and many of them make use of lib3MF:

| Product | Owner/Company/Developers |
| --- | --- |
| [Element](https://www.ntopology.com/element-pro/) | [nTopology](https://www.ntopology.com/) |
| [Netfabb](https://www.autodesk.com/products/netfabb/overview/) | [Autodesk](https://www.autodesk.com/) |
| [Meshmixer](http://www.meshmixer.com/) | [Autodesk](https://www.autodesk.com/) |
| [OpenSCAD](https://www.openscad.org/) | [OpenSCAD developers](https://github.com/orgs/openscad/people) |
| [Rhino3D](https://www.rhino3d.com/) | [Robert McNeel & Associates](https://www.mcneel.com/) |

If your product or project uses Lib3MF, please create a pull request or an issue to have it included in this list.
