# Contributor's Guide
Lib3MF is an open source project.

Contributions are welcome and we are always looking for people that improve the implementation of the specification and extensions of 3MF.

You can also contribute by reporting bugs in the [Issue tracker](../../issues), helping review pull requests, participate in discussions about issues and more.

## Filing issues
1. When filing an issue to report errors or problems, make sure to answer these five questions:
	1. Which version of lib3mf are you using?

		a) The API call `lib3mf_geinterfaceversion` exports lib3mf version number.

		b) Windows only: The properties of lib3mfs binary contain version information (e.g. right click on the DLL -> Properties -> Details).
	2. Which operating system are you using?
	3. What did you do?
	4. What did you expect to see?
	5. What did you see instead?

2. When contributing to this repository, please first discuss the change you wish to make via issue with the [maintainers](#maintainers) of this repository. This way, we can ensure that there is no overlap between contributions or development work by the 3MF consortium itself.

## Submitting a pull request
When ready to contribute, fork this repository and submit a pull request that references the issue it resolves. Be sure to include a clear and detailed description of the changes you've made so that we can verify them and eventually merge.

lib3mf follows the [git-flow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow) branching model. New developments are integrated into the [develop](../../tree/develop)-branch. lib3mf's maintainers will create releases from the develop-branch when appropriate.


## Maintainers
Maintainers are responsible for responding to pull requests and issues, as well as guiding the direction of the project.

We currently have two maintainers:
- Alexander Oster alexander.oster@autodesk.com
- Martin Weismann martin.weismann@autodesk.com

If you've established yourself as an impactful contributor to the project, and are willing take on the extra work, we'd love to have your help maintaining it! Email the current maintainers for details.



## Building lib3mf from source
1. Clone the repository\
`git clone https://github.com/3MFConsortium/lib3mf`
2. Enter local checkout\
`cd lib3mf`
3. Initialize the [googletest](https://github.com/google/googletest) submodule by running\
`git submodule update --init`
4. To generate projects for Visual Studio or Unix Makefiles use the scripts in the folder [cmake](cmake):\
Call `GenerateVS2015.bat` (Windows) or `GenerateMake.sh` (Linux, Mac), or
define other targets by specifying custom [CMake generators](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html).\
A new folder `build` is created and contains projects for the IDE/build target you selected.
5. Build the projects:
	* Windows:
		1. Open the VS Solution in the build folder and build it
		2. Select the project you want to run as "Start Up Project"
		3. Run/debug this project in VS
	* Linux/Mac:
		1. navigate to the `build`-folder 
		2. Call `make` to build the projects
		3. Run/debug a project 

## Building Fuzzers for lib3mf
```bash
mkdir build
cd build
cmake -DLIB3MF_FUZZ=on  -DCMAKE_BUILD_TYPE=AddressSanitizer ..
make -j4
./Fuzz/Fuzz_ReadFromBuffer
```
