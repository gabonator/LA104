cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0025 NEW)
project(LA140_emulator)

set(CMAKE_SUPPRESS_REGENERATION true)
set (CMAKE_CXX_STANDARD 17)

# call "brew install sdl2"


if (ARM)
include_directories("../../os_platform/common/include")
endif()

if (DESKTOP)           	
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
#include_directories("include/")
include_directories("../../os_platform/common/include")

file(GLOB main_src 
    "../../os_platform/mac_sdl/source/sdlmain.cpp"
    "../../os_platform/mac_sdl/source/sdlhal.h"
    "../../os_platform/common/include/library.h"
)

file(GLOB bios_src
    "../../os_platform/common/source/bios/*.cpp"
    "../../os_platform/common/source/bios/*.h"
)

file(GLOB framework_src
    "../../os_platform/common/source/framework/*.cpp"
    "../../os_platform/common/source/framework/*.h"
)

endif()

if (ARM)
file(GLOB main_src 
    "../../os_host/source/framework/Wnd.cpp"
)

file(GLOB linker_script
    "../../apps_ds213/55_gabosc/app.lds"
)
endif()

include_directories("../../apps_ds213/55_gabosc/")

file(GLOB application_src
    "../../apps_ds213/55_gabosc/*.cpp"
    "../../apps_ds213/55_gabosc/*.h"
)

file(GLOB application_framework_src
    "../../apps_ds213/55_gabosc/Framework/*.cpp"
    "../../apps_ds213/55_gabosc/Framework/*.h"
)

file(GLOB application_windows_src
    "../../apps_ds213/55_gabosc/Windows/*.cpp"
    "../../apps_ds213/55_gabosc/Windows/*.h"
)

file(GLOB application_oscilloscope_src
    "../../apps_ds213/55_gabosc/Oscilloscope/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/*.h"
    "../../apps_ds213/55_gabosc/Oscilloscope/Controls/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/Controls/*.h"
    "../../apps_ds213/55_gabosc/Oscilloscope/Core/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/Core/*.h"
    "../../apps_ds213/55_gabosc/Oscilloscope/Disp/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/Disp/*.h"
    "../../apps_ds213/55_gabosc/Oscilloscope/Input/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/Input/*.h"
    "../../apps_ds213/55_gabosc/Oscilloscope/Marker/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/Marker/*.h"
    "../../apps_ds213/55_gabosc/Oscilloscope/Mask/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/Mask/*.h"
    "../../apps_ds213/55_gabosc/Oscilloscope/Math/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/Math/*.h"
    "../../apps_ds213/55_gabosc/Oscilloscope/Meas/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/Meas/*.h"
    "../../apps_ds213/55_gabosc/Oscilloscope/Meas/Decoders/*.cpp"
    "../../apps_ds213/55_gabosc/Oscilloscope/Meas/Decoders/*.h"
)

if (DESKTOP)           	

#file(GLOB application_sdl
#    "../os_platform/mac_sdl/sdlmain.cpp"
#    "../os_platform/mac_sdl/sdlhal.h"
#)

source_group("platform\\main" FILES ${main_src})
source_group("platform\\bios" FILES ${bios_src})
source_group("platform\\framework" FILES ${framework_src})
#source_group("source\\gui" FILES ${gui_src})
endif()

source_group("application" FILES ${application_src})
source_group("application\\framework" FILES ${application_framework_src})
source_group("application\\windows" FILES ${application_windows_src})
source_group("application\\oscilloscope" FILES ${application_oscilloscope_src})
source_group("include" FILES "../../os_platform/common/include")

file(GLOB application_exec
  ${main_src} ${bios_src} ${framework_src} 
  ${gui_src} ${application_src} ${application_framework_src} ${application_windows_src} 
  ${application_oscilloscope_src} 
)

add_executable(application ${application_exec})

add_definitions(-DEMULATED)
add_definitions(-DDS203)

if (ARM)
target_link_libraries(application m)
target_link_libraries(application bios)
set_target_properties(application PROPERTIES LINK_DEPENDS ${linker_script})
set_target_properties(application PROPERTIES LINK_FLAGS "-T ${linker_script}")
endif()

target_link_libraries(application ${SDL2_LIBRARIES})

install(TARGETS application DESTINATION bin)
