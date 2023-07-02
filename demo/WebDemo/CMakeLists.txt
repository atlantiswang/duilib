# WebDemo sources.
file(GLOB WEBDEMO_SRCS "*.h" "*.hpp" "*.cpp" "*.cc" "*.c" "*.rc")
source_group(WebDemo FILES ${WEBDEMO_SRCS})

# Target executable names.
set(CEF_TARGET "WebDemo")
# Logical target used to link the libcef library.
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

# Determine the target output directory.
set(CEF_TARGET_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Executable target.
add_executable(${CEF_TARGET} WIN32 ${WEBDEMO_SRCS})
target_include_directories(${CEF_TARGET} PRIVATE
${PROJECT_SOURCE_DIR}
)

# 资源压缩，跟可执行文件打包到相同目录
add_custom_target(${CEF_TARGET}_skinzip
    COMMAND ${CMAKE_COMMAND} -E tar "c" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEF_TARGET}_skin.zip --format=zip .
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/skin
)

add_dependencies(${CEF_TARGET} libcef_dll_wrapper duilib ${CEF_TARGET}_skinzip)

# 修复编译错误, CEF 114.2.11使用 c++17，与duilib默认使用c++14冲突，这里将c++17编译选项去掉
# GdiplusPath.h(145,1): error C2872: 'byte': ambiguous symbol
list(REMOVE_ITEM CEF_CXX_COMPILER_FLAGS /std:c++17)

# 修复编译错误，可能有隐患
# UIDefine.h(105,13): error C2220: the following warning is treated as an error
# UIDefine.h(105,13): warning C4121: 'DuiLib::DUI_MSGMAP_ENTRY': alignment of a member was sensitive to packing
list(REMOVE_ITEM CEF_COMPILER_FLAGS /WX)


SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
target_link_libraries(${CEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS} duilib)

if(USE_SANDBOX)
  # Logical target used to link the cef_sandbox library.
  ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
  target_link_libraries(${CEF_TARGET} cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
endif()

# Add the custom manifest files to the executable.
ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")

# Copy binary and resource files to the target output directory.
COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
