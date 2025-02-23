# Contributor's Guide

lib3mf is an open-source project.

Contributions are welcome, and we are always looking for people who can improve the implementation of the specification and extensions of 3MF.

You can also contribute by reporting bugs in the [Issue tracker](https://github.com/3MFConsortium/lib3mf/issues), helping review pull requests, participating in discussions about issues, and more.

---

## Filing Issues

When filing an [issue](https://github.com/3MFConsortium/lib3mf/issues) to report errors or problems, make sure to answer these five questions:

1. **Which version of lib3mf are you using?**  
   - The API call `lib3mf_getlibraryversion` reveals lib3mf's version number.  
   - **Windows only:** The properties of lib3mf's binary contain version information (e.g., right-click on the DLL → Properties → Details).  

2. **Which operating system are you using?**  
3. **What did you do?**  
4. **What did you expect to see?**  
5. **What did you see instead?**  

When contributing to this repository, please first discuss the change you wish to make via an issue with the [maintainers](#maintainers). This way, we can ensure that there is no overlap between contributions or development work by the 3MF consortium itself.

---

## Submitting a Pull Request

When ready to contribute, **fork** this repository and submit a **pull request** that references the issue it resolves. Be sure to include a clear and detailed description of the changes you've made so that we can verify them and eventually merge.

lib3mf follows the [git-flow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow) branching model.  
New developments are integrated into the [develop](https://github.com/3MFConsortium/lib3mf/tree/develop) branch.  
lib3mf's maintainers will create releases from the develop branch when appropriate.

---

## Maintainers

Maintainers are responsible for responding to pull requests and issues, as well as guiding the direction of the project.

We currently have two maintainers:

- **Alexander Oster** - [alexander.oster@autodesk.com](mailto:alexander.oster@autodesk.com)  
- **Martin Weismann** - [martin.weismann@autodesk.com](mailto:martin.weismann@autodesk.com)  

If you've established yourself as an impactful contributor to the project and are willing to take on extra work, we'd love to have your help maintaining it!  
Email the current maintainers for details.

---

## Building lib3mf from Source

1. **Clone the repository**  
   ```sh
   git clone https://github.com/3MFConsortium/lib3mf
   ```
2. **Enter local checkout**  
   ```sh
   cd lib3mf
   ```
3. **Initialize the [googletest](https://github.com/google/googletest) submodule**  
   ```sh
   git submodule update --init
   ```
4. **Generate project files using CMake**  
   Use the scripts in the **[cmake](cmake)** folder:
   - **Windows:** Run `GenerateVS2017.bat`
   - **Linux/Mac:** Run `GenerateMake.sh`
   - To specify custom [CMake generators](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html), define your own target.

   This will create a `build/` folder containing the appropriate project files.

5. **Build the projects**  
   - **Windows:**
     1. Open the **Visual Studio Solution** in the `build/` folder.
     2. Build the solution.
     3. Select the project you want to run as the "Start-Up Project."
     4. Run/debug it in Visual Studio.

   - **Linux/Mac:**
     1. Navigate to the `build/` folder.
     2. Run:
        ```sh
        make
        ```
     3. Run/debug the compiled binaries.

---

## How to Develop lib3mf

lib3mf is based on the **hourglass pattern** for software components and uses the  
[Automatic Component Toolkit (ACT)](https://github.com/Autodesk/AutomaticComponentToolkit/) to simplify development.

lib3mf's goal is to run on **Windows, Linux, and macOS**, with most of its code being **platform-independent**.

!!! note ""
    The contents of this page have been largely copied from  
    [https://github.com/3MFConsortium/lib3mf/blob/master/CONTRIBUTING.md](https://github.com/3MFConsortium/lib3mf/blob/master/CONTRIBUTING.md).  
    If you find any information to be outdated, feel free to let us know.
