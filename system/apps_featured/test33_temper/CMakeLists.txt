# for use in LA104/system/hybrid folder

cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0025 NEW)
project(LA140_emulator)

set(CMAKE_SUPPRESS_REGENERATION true)
set (CMAKE_CXX_STANDARD 17)

# call "brew install sdl2"

if (ARM)
include_directories("../os_library/include")
endif()

include_directories("../../os_host/source")

if (DESKTOP)           	
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("include/")
include_directories("../../os_platform/common/include")

file(GLOB main_src 
    "../../os_platform/apple/source/sdlmain.cpp"
    "../../os_platform/apple/source/sdlhal.h"
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


file(GLOB gui_src
    "../../os_platform/common/source/gui/*.cpp"
    "../../os_platform/common/source/gui/*.h"
)
endif()

if (ARM)
file(GLOB main_src 
    "../os_host/source/framework/Wnd.cpp"
)

file(GLOB linker_script
    "../apps_experiments/test33_temper/app.lds"
)
endif()

file(GLOB application_src
    "*.cpp"
    "*.h"
)

file(GLOB application_graph_src
    "graph/*.cpp"
    "graph/*.h"
)

file(GLOB application_sensor_src
    "sensor/*.cpp"
    "sensor/*.h"
)

if (DESKTOP)           	
source_group("source\\main" FILES "source/sdlmain.cpp")
source_group("source\\bios" FILES ${bios_src})
source_group("source\\framework" FILES ${framework_src})
source_group("source\\gui" FILES ${gui_src})
endif()

source_group("application" FILES ${application_src})
source_group("application\\graph" FILES ${application_graph_src})
source_group("application\\sensor" FILES ${application_sensor_src})
source_group("include" FILES "include/library.h")

add_executable(application ${main_src} ${bios_src} ${framework_src} ${gui_src} ${application_src} ${application_graph_src} ${application_sensor_src})

add_definitions(-DLA104)

if (ARM)
target_link_libraries(application m)
target_link_libraries(application bios)
set_target_properties(application PROPERTIES LINK_DEPENDS ${linker_script})
set_target_properties(application PROPERTIES LINK_FLAGS "-T ${linker_script}")
endif()

target_link_libraries(application ${SDL2_LIBRARIES})

install(TARGETS application DESTINATION bin)
