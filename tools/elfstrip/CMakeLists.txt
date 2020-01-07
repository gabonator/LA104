cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0025 NEW)
project(LA140_emulator)

set(CMAKE_SUPPRESS_REGENERATION true)
set (CMAKE_CXX_STANDARD 17)

# call "brew install sdl2"

## Global files

file(GLOB main_src 
    "../../tools/elfstrip/main.cpp"
)


source_group("platform\\main" FILES ${main_src})


file(GLOB elfstrip_exec
  ${main_src} 
)

add_executable(elfstrip ${elfstrip_exec})


install(TARGETS elfstrip DESTINATION bin)

