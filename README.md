# DiagMC
![example workflow](https://github.com/Enry99/DiagMC/actions/workflows/cmake-multi-platform.yml/badge.svg)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

## Installation using CMake
The installation procedure relies on [CMake](https://cmake.org/), which is cross-platform and can be used on many operative systems, like Linux and Windows.\
Note that on Windows you need to have an available C++ compiler, such as [MSVC](https://docs.microsoft.com/en-us/cpp/), which is provided with [Visual Studio](https://visualstudio.microsoft.com/).\
A viable alternative on Windows is to use [Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/).

First, **clone the repository** in your local machine using [git](https://git-scm.com/):
```sh
$ git clone https://github.com/Enry99/DiagMC.git
```
This will create a folder called ```DiagMC```. Go inside this folder with:
```sh
$ cd DiagMC
```
Now **build and install** the program using **CMake**:
```sh
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --target install
```
**The program executable can be found in ```DiagMC/bin```**.

Note that by default the build type is set to ```Release```. To build for ```Debug```, instead of ```$ cmake ..``` in the third line, run:
```sh
$ cmake .. -DCMAKE_BUILD_TYPE="Debug"
```
Be aware that due to the absence of compiler optimization, the performance of the program built in Debug mode is massively impacted.

### Dependencies
The program depends on the [json](https://github.com/nlohmann/json.git) library for reading the settings from file, and on [googletest](https://github.com/google/googletest.git) for unit testing.
Both dependencies are automatically downloaded and included by CMake during the build process, so an Internet connection is required during the installation.

It is possible to exclude the googletest dependency by building without tests:
```sh
$ cmake .. cmake .. -DBUILD_TESTING=OFF
```
However, testing is recommended before using the program.


### Execute unit tests
In order to execute the tests, go to the  ```build``` directory and run:
```sh
$ ctest
```

## Examples

<img src="/examples/sweep_beta=10.0.png" width="800">
<img src="/examples/convergence_test.png" width="800">





## License
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

This work is licensed under the [GNU General Public License Version 3][GPLv3].

[GPLv3]: https://www.gnu.org/licenses/gpl-3.0