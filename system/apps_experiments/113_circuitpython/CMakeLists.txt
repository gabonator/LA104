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
include_directories("source/py")
include_directories("source/extmod")
include_directories("source/")

if (DESKTOP)           	
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("include/")
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


file(GLOB spf_src
    "../../os_host/library/spf/*.cpp"
    "../../os_host/library/spf/*.h"
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
    "source/*.cpp"
    "source/*.c"
    "source/*.h"
)

file(GLOB application_graph_src
    "source/py/*.c"
    "source/py/*.h"
)

file(GLOB application_sensor_src
    "source/extmod/*.c"
    "source/extmod/*.h"
)

file(GLOB application_frozen_src
    "source/frozen/*.c"
    "source/frozen/*.h"
)

file(GLOB application_utils_src
    "source/lib/utils/*.c"
    "source/lib/utils/*.h"
)

if (DESKTOP)           	
source_group("source\\main" FILES ${main_src})
source_group("source\\bios" FILES ${bios_src})
source_group("source\\framework" FILES ${framework_src})
source_group("source\\gui" FILES ${gui_src})
endif()

source_group("application" FILES ${application_src})
source_group("application\\py" FILES ${application_graph_src})
source_group("application\\extmod" FILES ${application_sensor_src})
source_group("application\\frozen" FILES ${application_frozen_src})
source_group("application\\lib\\utils" FILES ${application_utils_src})
source_group("include" FILES "include/library.h")

add_executable(application ${main_src} ${bios_src} ${framework_src} ${gui_src} ${application_src} ${application_graph_src} ${application_sensor_src} ${application_frozen_src} ${application_utils_src})

add_definitions(-DLA104)
add_definitions(-DCIRCUITPY_ENABLE_MPY_NATIVE=0)
add_definitions(-DCIRCUITPY_FULL_BUILD=0)
add_definitions(-DMICROPY_ENABLE_COMPILER=1)
#add_definitions(-DMICROPY_PORT_BUILTINS=0)
add_definitions(-DMICROPY_ENABLE_EXTERNAL_IMPORT=0)


if (ARM)
target_link_libraries(application m)
target_link_libraries(application bios)
set_target_properties(application PROPERTIES LINK_DEPENDS ${linker_script})
set_target_properties(application PROPERTIES LINK_FLAGS "-T ${linker_script}")
endif()

target_link_libraries(application ${SDL2_LIBRARIES})

install(TARGETS application DESTINATION bin)
