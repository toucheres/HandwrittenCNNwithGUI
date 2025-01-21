# dlltest/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(dlltest VERSION 1.0.0 LANGUAGES CXX)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

# 添加动态库目标
add_library(dlltest SHARED ${SRC})

# 定义包含目录，使用 PUBLIC 关键字
target_include_directories(dlltest
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 安装目标
install(TARGETS dlltest
  EXPORT dlltestTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# 安装头文件
install(DIRECTORY include/ DESTINATION include)

# 导出目标以供其他项目使用
install(EXPORT dlltestTargets
  FILE dlltestTargets.cmake
  NAMESPACE dlltest::
  DESTINATION lib/cmake/dlltest
)

# 生成并安装配置文件
include(CMakePackageConfigHelpers)

# 写入版本文件
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/dlltestConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# 配置配置文件
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/dlltestConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/dlltestConfig.cmake"
  INSTALL_DESTINATION lib/cmake/dlltest
)

# 安装配置文件
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/dlltestConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/dlltestConfigVersion.cmake"
  DESTINATION lib/cmake/dlltest
)

# 在构建树中导出目标
export(EXPORT dlltestTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/dlltestTargets.cmake"
  NAMESPACE dlltest::
)
