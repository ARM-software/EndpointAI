cmake_minimum_required (VERSION 3.14)

project(MyLib)

include(configLib)
include(configDsp)


add_library(MyLib STATIC)
## Add your source files here
target_sources(MyLib PRIVATE Source/mylib.c)


configLib(MyLib ${ROOT})
configDsp(MyLib ${ROOT})

### Includes
target_include_directories(MyLib PUBLIC "Include")


target_link_libraries(MyLib PRIVATE CMSISDSP)
