#Cmake file for ListDemo
#Author: Qi Gao(monkgau@gmail.com)
#Created: 2012/9/17

set(source_files Main.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(ListDemo ${source_files})
target_include_directories(ListDemo PRIVATE
${PROJECT_SOURCE_DIR}
)
# 资源压缩，跟可执行文件打包到相同目录
add_custom_target(ListDemo_skinzip
    COMMAND ${CMAKE_COMMAND} -E tar "c" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ListDemo_skin.zip --format=zip .
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/skin
)
add_dependencies(ListDemo duilib ListDemo_skinzip)
target_link_libraries(ListDemo duilib)
set_target_properties(ListDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")