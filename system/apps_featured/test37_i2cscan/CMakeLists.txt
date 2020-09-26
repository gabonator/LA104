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

if (DESKTOP)           	
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("include/")

file(GLOB main_src 
    "source/sdlmain.cpp"
    "include/library.h"
)

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
endif()

if (ARM)
file(GLOB main_src 
    "../os_host/source/framework/Wnd.cpp"
)

file(GLOB linker_script
    "../apps/test37_i2cscan/app.lds"
)
endif()

file(GLOB application_src
    "../apps/test37_i2cscan/*.cpp"
    "../apps/test37_i2cscan/*.h"
)

#file(GLOB application_graph_src
#    "../apps/test34_scope/graph/*.cpp"
#    "../apps/test34_scope/graph/*.h"
#)

#file(GLOB application_sensor_src
#    "../apps/test34_scope/sensor/*.cpp"
#    "../apps/test34_scope/sensor/*.h"
#)

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

if (ARM)
target_link_libraries(application m)
target_link_libraries(application bios)
set_target_properties(application PROPERTIES LINK_DEPENDS ${linker_script})
set_target_properties(application PROPERTIES LINK_FLAGS "-T ${linker_script}")
endif()

target_link_libraries(application ${SDL2_LIBRARIES})

install(TARGETS application DESTINATION bin)
