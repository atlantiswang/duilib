#Cmake file for RichListDemo
#Author: Qi Gao(monkgau@gmail.com)
#Created: 2012/9/17

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})                           


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(RichListDemo ${SRC_LIST} RichListDemo.rc)
# 资源压缩，跟可执行文件打包到相同目录
add_custom_target(RichListDemo_skipzip
    COMMAND ${CMAKE_COMMAND} -E tar "c" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RichListDemo_skin.zip --format=zip .
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/skin
)
add_dependencies(RichListDemo duilib RichListDemo_skipzip)
set_target_properties(RichListDemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
target_link_libraries(RichListDemo duilib)
