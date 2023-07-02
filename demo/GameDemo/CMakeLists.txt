#Cmake file for GameDemo
#Author: Qi Gao(monkgau@gmail.com)
#Created: 2012/9/17

set(source_files GameDemo.cpp MiniDumper.cpp stdAfx.cpp)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(GameDemo ${source_files})

# 资源压缩，跟可执行文件打包到相同目录
add_custom_target(GameDemo_skinzip
    COMMAND ${CMAKE_COMMAND} -E tar "c" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GameDemo_skin.zip --format=zip .
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/skin
)

add_dependencies(GameDemo duilib GameDemo_skinzip)
target_link_libraries(GameDemo duilib)
set_target_properties(GameDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")