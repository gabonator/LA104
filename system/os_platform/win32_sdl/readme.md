# Building on Windows platform
Here is a short description on how to build on Windows platform using Cygwin, SDL, CMake and Visual Studio.

## Install cygwin
First you need to download cygwin installer : https://www.cygwin.com/ and run an standard installation.

You'll also need to add the following additional packages :
- binutils
- cmake

## Install arm toolchain
Then you need to download and install the latest arm toolchain for windows (use installer) : https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

Make sure to add the bin directory in you Windows PATH.

## Download SDL
Now you need to download the latest SDL2 Windows development library for Visual C++ (SDL2-devel-2.x.x-VC.zip) : https://www.libsdl.org/download-2.0.php

Unzip it somwhere on your computer.

Create a file named `sdl2-config.cmake` under the root of the SDL library with the following content :

	set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
	# Support both 32 and 64 bit builds
	if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
		set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2main.lib")
	else ()
		set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2main.lib")
	endif ()
	string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

## Install CMake gui
Now you'll need CMake gui to create Visual Studio project files.

Download and install CMake (use the installer) : https://cmake.org/download/

## Generate Visual Studio project files
You can now lauch CMmake GUI and generate the Visual Studio project files as follows :
- Set source code directory to `<xxx>/LA104/system/os_platform/win32_sdl`
- Set build directory to `<xxx>/LA104/system/os_platform/win32_sdl/_build`
- Hit configure and select the appropriate version of Visual Studio (tested with VS 2017), leave defaut values for other options.

CMake will now create default parameters for your build, but you beed to add the follwing entry :
- `DESKTOP` of type boolean and set to true

Hit configure again and CMake will ask for the destination of your SDL library installation :
- `SDL2_DIR` : `path-where-you-unziped-sdl2-library`

Now you can hit configure then generate and finally Open Project.
