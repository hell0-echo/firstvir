cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TypeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 必须添加 Multimedia 模块
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
)

add_executable(typegame ${SOURCES})
# 链接 Multimedia
target_link_libraries(typegame PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Multimedia)

if(WIN32)
    set_target_properties(typegame PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(MSVC)
    # Visual Studio / MSVC
    target_compile_options(typegame PRIVATE /utf-8)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # MinGW / GCC / Clang
    target_compile_options(typegame PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()