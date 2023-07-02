#Cmake file for 360SafeDemo
#Author: Qi Gao(monkgau@gmail.com)
#Date: 2012/9/17

set(source_files 360safe.cpp stdafx.cpp)
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(360safedemo ${source_files})
set_target_properties(360safedemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

# 资源压缩，跟可执行文件打包到相同目录
add_custom_target(360safedemo_skinzip
    COMMAND ${CMAKE_COMMAND} -E tar "c" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/360safedemo_skin.zip --format=zip .
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/skin
)
add_dependencies(360safedemo duilib 360safedemo_skinzip)
target_link_libraries(360safedemo duilib)