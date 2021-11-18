# cpp_starter_project

### Necessary Dependencies
1. A C++ compiler that supports C++20.
See [cppreference.com](https://en.cppreference.com/w/cpp/compiler_support)
to see which features are supported by each compiler.
The following compilers should work:

  * [gcc 9+](https://gcc.gnu.org/)

  * [clang 10+](https://clang.llvm.org/)
  <details>
  - Debian/Ubuntu
  ```command
    apt install llvm-12 llvm-12-doc clang-12 clang-tidy-12 clang-format-12 clangd-12 lld-12 lldb-12
  ```
  </details>

  * [Visual Studio 2019 or higher](https://visualstudio.microsoft.com/)

2. [Conan](https://conan.io/)
	<details>
	<summary>Install Command</summary>

	- Via pip - https://docs.conan.io/en/latest/installation.html#install-with-pip-recommended

			pip install --user conan

	- Windows:

			choco install conan -y

	- MacOS:

			brew install conan

	</details>

3. [CMake 3.15+](https://cmake.org/)

## Build Instructions

A full build has different steps:
1) Specifying the compiler using environment variables
2) Configuring the project
3) Building the project

For the subsequent builds, in case you change the source code, you only need to repeat the last step.

### (1) Specify the compiler using environment variables

By default (if you don't set environment variables `CC` and `CXX`), the system default compiler will be used.

Conan and CMake use the environment variables CC and CXX to decide which compiler to use. So to avoid the conflict issues only specify the compilers using these variables.

CMake will detect which compiler was used to build each of the Conan targets. If you build all of your Conan targets with one compiler, and then build your CMake targets with a different compiler, the project may fail to build.

<details>
<summary>Commands for setting the compilers </summary>

- Debian/Ubuntu/MacOS:

	Set your desired compiler (`clang`, `gcc`, etc):

	- Temporarily (only for the current shell)

		Run one of the followings in the terminal:

		- clang

				CC=clang CXX=clang++

		- gcc

				CC=gcc CXX=g++

	- Permanent:

		Open `~/.bashrc` using your text editor:

			gedit ~/.bashrc

		Add `CC` and `CXX` to point to the compilers:

			export CC=clang
			export CXX=clang++

		Save and close the file.

- Windows:

	- Permanent:

		Run one of the followings in PowerShell:

		- Visual Studio generator and compiler (cl)

				[Environment]::SetEnvironmentVariable("CC", "cl.exe", "User")
				[Environment]::SetEnvironmentVariable("CXX", "cl.exe", "User")
				refreshenv

		  Set the architecture using [vsvarsall](https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=vs-2019#vcvarsall-syntax):

				vsvarsall.bat x64

		- clang

				[Environment]::SetEnvironmentVariable("CC", "clang.exe", "User")
				[Environment]::SetEnvironmentVariable("CXX", "clang++.exe", "User")
				refreshenv

		- gcc

				[Environment]::SetEnvironmentVariable("CC", "gcc.exe", "User")
				[Environment]::SetEnvironmentVariable("CXX", "g++.exe", "User")
				refreshenv


  - Temporarily (only for the current shell):

			$Env:CC="clang.exe"
			$Env:CXX="clang++.exe"

</details>

### (2) Configure your build

To configure the project, you could use `cmake`, or `ccmake` or `cmake-gui`. Each of them are explained in the following:

#### (2.a) Configuring via cmake:
With Cmake directly:

    cmake -S . -B ./build

Cmake will automatically create the `./build` folder if it does not exist, and it wil configure the project.

#### (2.b) Configuring via ccmake:

With the Cmake Curses Dialog Command Line tool:

    ccmake -S . -B ./build

Once `ccmake` has finished setting up, press 'c' to configure the project,
press 'g' to generate, and 'q' to quit.

#### (2.c) Configuring via cmake-gui:

To use the GUI of the cmake:

2.c.1) Open cmake-gui from the project directory:
```
cmake-gui .
```
2.c.2) Set the build directory:

![build_dir](https://user-images.githubusercontent.com/16418197/82524586-fa48e380-9af4-11ea-8514-4e18a063d8eb.jpg)

2.c.3) Configure the generator:

In cmake-gui, from the upper menu select `Tools/Configure`.

**Warning**: if you have set `CC` and `CXX` always choose the `use default native compilers` option. This picks `CC` and `CXX`. Don't change the compiler at this stage!

<details>
<summary>Windows - MinGW Makefiles</summary>

Choose MinGW Makefiles as the generator:

<img src="https://user-images.githubusercontent.com/16418197/82769479-616ade80-9dfa-11ea-899e-3a8c31d43032.png" alt="mingw">

</details>

<details>
<summary>Windows - Visual Studio generator and compiler</summary>

You should have already set `C` and `CXX` to `cl.exe`.

Choose "Visual Studio 16 2019" as the generator:

<img src="https://user-images.githubusercontent.com/16418197/82524696-32502680-9af5-11ea-9697-a42000e900a6.jpg" alt="default_vs">

</details>

<details>

<summary>Windows - Visual Studio generator and Clang Compiler</summary>

You should have already set `C` and `CXX` to `clang.exe` and `clang++.exe`.

Choose "Visual Studio 16 2019" as the generator. To tell Visual studio to use `clang-cl.exe`:
- If you use the LLVM that is shipped with Visual Studio: write `ClangCl` under "optional toolset to use".

<img src="https://user-images.githubusercontent.com/16418197/82781142-ae60ac00-9e1e-11ea-8c77-222b005a8f7e.png" alt="visual_studio">

- If you use an external LLVM: write [`LLVM_v142`](https://github.com/zufuliu/llvm-utils#llvm-for-visual-studio-2017-and-2019)
 under "optional toolset to use".

<img src="https://user-images.githubusercontent.com/16418197/82769558-b3136900-9dfa-11ea-9f73-02ab8f9b0ca4.png" alt="visual_studio">

</details>
<br/>

2.c.4) Choose the Cmake options and then generate:

![generate](https://user-images.githubusercontent.com/16418197/82781591-c97feb80-9e1f-11ea-86c8-f2748b96f516.png)

### (3) Build the project
Once you have selected all the options you would like to use, you can build the
project (all targets):

    cmake --build ./build

For Visual Studio, give the build configuration (Release, RelWithDeb, Debug, etc) like the following:

    cmake --build ./build -- /p:configuration=Release


### Running the tests

You can use the `ctest` command run the tests.

```shell
cd ./build
ctest -C Debug
cd ../
```

## Troubleshooting

### Update Conan
Many problems that users have can be resolved by updating Conan, so if you are
having any trouble with this project, you should start by doing that.

To update conan:

    pip install --user --upgrade conan

You may need to use `pip3` instead of `pip` in this command, depending on your
platform.

### Clear Conan cache
If you continue to have trouble with your Conan dependencies, you can try
clearing your Conan cache:

    conan remove -f '*'

The next time you run `cmake` or `cmake --build`, your Conan dependencies will
be rebuilt. If you aren't using your system's default compiler, don't forget to
set the CC, CXX, CMAKE_C_COMPILER, and CMAKE_CXX_COMPILER variables, as
described in the 'Build using an alternate compiler' section above.

### Identifying misconfiguration of Conan dependencies

If you have a dependency 'A' that requires a specific version of another
dependency 'B', and your project is trying to use the wrong version of
dependency 'B', Conan will produce warnings about this configuration error
when you run CMake. These warnings can easily get lost between a couple
hundred or thousand lines of output, depending on the size of your project.

If your project has a Conan configuration error, you can use `conan info` to
find it. `conan info` displays information about the dependency graph of your
project, with colorized output in some terminals.

    cd build
    conan info .

In my terminal, the first couple lines of `conan info`'s output show all of the
project's configuration warnings in a bright yellow font.

For example, the package `spdlog/1.5.0` depends on the package `fmt/6.1.2`.
If you were to modify the file `conanfile.py` so that it requires an
earlier version of `fmt`, such as `fmt/6.0.0`, and then run:

```bash
conan remove -f '*'       # clear Conan cache
rm -rf build              # clear previous CMake build
cmake -S . -B ./build     # rebuild Conan dependencies
conan info ./build
```

...the first line of output would be a warning that `spdlog` needs a more recent
version of `fmt`.

## Testing
See [Catch2 tutorial](https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md)

