# ====================================================================
# Project:   myproject
# Author:    Giovanni Santini
# Mail:      giovanni.santini@proton.me
# Github:    @San7o
# License:   MIT
# ====================================================================

cmake_minimum_required(VERSION 3.16)

project(design-patterns
  VERSION 1.0
  DESCRIPTION "design patterns examples in C++17"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # no gnu extensions

find_program(CCACHE_PROGRAM ccache) # use ccache if available
if(CCACHE_PROGRAM)
  message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

if(MSVC)
  set(WARNING_FLAGS /W4 /permissive-)
  set(DEBUG_FLAGS /Zi /Od)
  set(RELEASE_FLAGS /O2)
  set(DEBUG_DEFINES _DEBUG)
else()
  set(WARNING_FLAGS -Wall -Wextra -Wpedantic)
  set(DEBUG_FLAGS -O0 -ggdb -fsanitize=address)
  set(RELEASE_FLAGS -O3)
  set(DEBUG_DEFINES DEBUG)
  set(DEBUG_LINK_FLAGS -fsanitize=address)
endif()

#
# Options
#

#option(BUILD_SHARED_LIBS "Build libraries as shared" ON)
option(ENABLE_TESTING "Enable unit tests" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release)" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#
# Targets
#

# Abstract factory
add_executable(abstract-factory abstract-factory.cpp)
target_compile_definitions(abstract-factory PRIVATE ${DEBUG_DEFINES})
target_compile_options(abstract-factory
  PRIVATE ${DEBUG_FLAGS} ${WARNING_FLAGS})
if(NOT MSVC)
  target_link_options(abstract-factory PRIVATE ${DEBUG_LINK_FLAGS})
endif()

# Builder
add_executable(builder builder.cpp)
target_compile_definitions(builder PRIVATE ${DEBUG_DEFINES})
target_compile_options(builder
  PRIVATE ${DEBUG_FLAGS} ${WARNING_FLAGS})
if(NOT MSVC)
  target_link_options(builder PRIVATE ${DEBUG_LINK_FLAGS})
endif()

# Factory method
add_executable(factory-method factory-method.cpp)
target_compile_definitions(factory-method PRIVATE ${DEBUG_DEFINES})
target_compile_options(factory-method
  PRIVATE ${DEBUG_FLAGS} ${WARNING_FLAGS})
if(NOT MSVC)
  target_link_options(factory-method PRIVATE ${DEBUG_LINK_FLAGS})
endif()

# Prototype
add_executable(prototype prototype.cpp)
target_compile_definitions(prototype PRIVATE ${DEBUG_DEFINES})
target_compile_options(prototype
  PRIVATE ${DEBUG_FLAGS} ${WARNING_FLAGS})
if(NOT MSVC)
  target_link_options(prototype PRIVATE ${DEBUG_LINK_FLAGS})
endif()
set_property(TARGET prototype PROPERTY CXX_STANDARD 20)

# Singleton
add_executable(singleton singleton.cpp)
target_compile_definitions(singleton PRIVATE ${DEBUG_DEFINES})
target_compile_options(singleton
  PRIVATE ${DEBUG_FLAGS} ${WARNING_FLAGS})
if(NOT MSVC)
  target_link_options(singleton PRIVATE ${DEBUG_LINK_FLAGS})
endif()


#
# End of CMakeLists.txt
# ====================================================================
