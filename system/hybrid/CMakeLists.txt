cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0025 NEW)
project(LA140_emulator)

set(CMAKE_SUPPRESS_REGENERATION true)
set (CMAKE_CXX_STANDARD 17)

# call "brew install sdl2"

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("include/")

file(GLOB bios_src
    "source/bios/*.cpp"
    "source/bios/*.h"
)

file(GLOB framework_src
    "source/framework/*.cpp"
    "source/framework/*.h"
)

file(GLOB gui_src
    "source/gui/*.cpp"
    "source/gui/*.h"
)

file(GLOB application_src
    "../apps_experiments/test28_shell/*.cpp"
    "../apps_experiments/test28_shell/*.h"
)

source_group("source\\main" FILES "source/sdlmain.cpp")
source_group("source\\bios" FILES ${bios_src})
source_group("source\\framework" FILES ${framework_src})
source_group("source\\gui" FILES ${gui_src})
source_group("application" FILES ${application_src})
source_group("include" FILES "include/library.h")

add_executable(application source/sdlmain.cpp include/library.h ${bios_src} ${framework_src} ${gui_src} ${application_src})

target_link_libraries(application ${SDL2_LIBRARIES})

install(TARGETS application DESTINATION bin)
