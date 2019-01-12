set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING 1)
set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_C_COMPILER "${CMAKE_TOOLCHAIN_PATH}/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "${CMAKE_TOOLCHAIN_PATH}/arm-none-eabi-g++")
SET(CMAKE_C_LINK_EXECUTABLE "${CMAKE_TOOLCHAIN_PATH}/arm-none-eabi-gcc")

set(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-nostdlib --specs=nosys.specs -fPIC -mcpu=cortex-m3 -mthumb -nostartfiles")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,-nostdlib --specs=nosys.specs -fPIC -mcpu=cortex-m3 -mthumb -nostartfiles -lbios -lm -L../../os_library/build" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-nostdlib --specs=nosys.specs -fPIC -mcpu=cortex-m3 -mthumb -nostartfiles -L../../os_library/build" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD")
set(CMAKE_C_FLAGS "-Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -Wno-psabi -MD")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

