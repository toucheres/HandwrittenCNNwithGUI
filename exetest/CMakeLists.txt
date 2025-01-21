# exetest/CMakeLists.txt

cmake_minimum_required(VERSION 3.8)
project(exetest LANGUAGES CXX)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)


set(FORMS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/forms")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB SOURCES
    "${FORMS_DIR}/*.ui"
    "${FORMS_DIR}/*.qrc"
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

message("hhhhhhhhhhhhhh")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS ${FORMS_DIR})
message(STATUS ${SOURCES})
message("hhhhhhhhhhhhhh")

# 添加可执行目标
add_executable(exetest ${SOURCES})

#
target_include_directories(exetest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 查找 dlltest 包
find_package(dlltest REQUIRED PATHS C:/work/vscoderope/HandwrittenCNNwithGUI/build/dlltest NO_DEFAULT_PATH)

# 链接库
target_link_libraries(exetest PRIVATE dlltest::dlltest Qt6::Widgets)
# Add the target includes for MY_PROJECT 
target_include_directories(exetest PRIVATE ${FORMS_DIR})
target_include_directories(exetest PRIVATE ${INCLUDE_DIR})
target_include_directories(exetest PRIVATE ${SOURCE_DIR})



# 设置 C++ 标准
set_target_properties(exetest PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 自定义命令：复制 DLL 文件到可执行文件目录
add_custom_command(TARGET exetest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:dlltest>"
    $<TARGET_FILE_DIR:exetest>
)
