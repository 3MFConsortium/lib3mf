# lib3mf
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

## Building lib3MF
The current code runs on Windows, Linux and Mac. To generate projects for Visual Studio or Unix
Makefiles use the scripts in the folder [cmake](cmake).

## Examples 
For example code, please refer to the examples in the SDK (SDK/Examples) or to
[https://github.com/3MFConsortium/lib3mf-examples](https://github.com/3MFConsortium/lib3mf-examples)

## Documentation
For a more detailed documentation, please take a look into the PDF in the root directory
of this repository.

## License
Lib3MF is licensed under the [simplified BSD license](LICENSE).
