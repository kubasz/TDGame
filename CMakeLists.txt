cmake_minimum_required(VERSION 2.8)
project(TDGame)

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
endif ()
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
	include_directories("${CMAKE_SOURCE_DIR}/ext/win-include")
endif()
include_directories("${CMAKE_SOURCE_DIR}/ext/include")
find_package(SFML 2.3 REQUIRED COMPONENTS graphics window system)
include_directories(${SFML_INCLUDE_DIR})

find_package(SFGUI 0.3 REQUIRED)
include_directories(${SFGUI_INCLUDE_DIR})

find_package(Thor REQUIRED)
include_directories(${THOR_INCLUDE_DIR})

# Build targets should be placed in the root build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(doc)
