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
    "../apps/test41_alchemy/app.lds"
)
endif()

file(GLOB application_src
    "../apps/test41_alchemy/source/*.cpp"
    "../apps/test41_alchemy/source/*.h"
)

file(GLOB application_devices_src
    "../apps/test41_alchemy/source/devices/*.cpp"
    "../apps/test41_alchemy/source/devices/*.h"
)

file(GLOB application_layouts_src
    "../apps/test41_alchemy/source/layouts/*.cpp"
    "../apps/test41_alchemy/source/layouts/*.h"
)

file(GLOB application_layouts_file_src
    "../apps/test41_alchemy/source/layouts/file/*.cpp"
    "../apps/test41_alchemy/source/layouts/file/*.h"
)

file(GLOB application_layouts_meas_src
    "../apps/test41_alchemy/source/layouts/meas/*.cpp"
    "../apps/test41_alchemy/source/layouts/meas/*.h"
)

file(GLOB application_layouts_modem_src
    "../apps/test41_alchemy/source/layouts/modem/*.cpp"
    "../apps/test41_alchemy/source/layouts/modem/*.h"
)

file(GLOB application_utils_src
    "../apps/test41_alchemy/source/utils/*.cpp"
    "../apps/test41_alchemy/source/utils/*.h"
)

file(GLOB application_controls_src
    "../apps/test41_alchemy/source/controls/*.cpp"
    "../apps/test41_alchemy/source/controls/*.h"
)

if (DESKTOP)           	
source_group("source\\main" FILES "source/sdlmain.cpp")
source_group("source\\bios" FILES ${bios_src})
source_group("source\\framework" FILES ${framework_src})
source_group("source\\gui" FILES ${gui_src})
endif()

source_group("application" FILES ${application_src})
source_group("application\\devices" FILES ${application_devices_src})
source_group("application\\layouts" FILES ${application_layouts_src})
source_group("application\\layouts\\file" FILES ${application_layouts_file_src})
source_group("application\\layouts\\meas" FILES ${application_layouts_meas_src})
source_group("application\\layouts\\modem" FILES ${application_layouts_modem_src})
source_group("application\\utils" FILES ${application_utils_src})
source_group("application\\controls" FILES ${application_controls_src})
source_group("include" FILES "include/library.h")

add_executable(application ${main_src} ${bios_src} ${framework_src} ${gui_src} ${application_src} ${application_devices_src} ${application_layouts_src} ${application_utils_src} ${application_controls_src} ${application_layouts_file_src} ${application_layouts_meas_src} ${application_layouts_modem_src})

if (ARM)
target_link_libraries(application m)
target_link_libraries(application bios)
set_target_properties(application PROPERTIES LINK_DEPENDS ${linker_script})
set_target_properties(application PROPERTIES LINK_FLAGS "-T ${linker_script}")
endif()

target_link_libraries(application ${SDL2_LIBRARIES})

install(TARGETS application DESTINATION bin)
