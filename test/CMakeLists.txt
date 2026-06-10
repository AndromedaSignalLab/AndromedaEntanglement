#Build script for the tests of Andromeda Entanglement Project
#Copyright (C) 2026 Volkan Orhan

#This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

#This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

#You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

cmake_minimum_required(VERSION 3.5)
project("Andromeda Entanglement Test")
if(APPLE)
    enable_language(OBJCXX)
endif()
enable_language(C)
set(CMAKE_CXX_STANDARD 20)


find_library(FOUNDATION_FRAMEWORK Foundation)
find_library(APPKIT_FRAMEWORK AppKit)
find_library(CORESERVICES_FRAMEWORK CoreServices)
find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)

file(GLOB AndromedaEntanglementSrc
    "../src/*.hpp"
    "../src/*.cpp"
    "../src/*.mm"
)

file(GLOB AndromedaEntanglementTestSrc
        "./*.hpp"
        "./*.cpp"
        "./*.mm"
)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(AndromedaEntanglementTest
        ${AndromedaEntanglementTestSrc}
        ${AndromedaEntanglementSrc}
)

target_link_libraries(
        AndromedaEntanglementTest
        PRIVATE
        ${FOUNDATION_FRAMEWORK}
        ${APPKIT_FRAMEWORK}
        ${CORESERVICES_FRAMEWORK}
        ${COREFOUNDATION_FRAMEWORK}
)

message(STATUS "Foundation: ${FOUNDATION_FRAMEWORK}")
message(STATUS "AppKit: ${APPKIT_FRAMEWORK}")
message(STATUS "CoreServices: ${CORESERVICES_FRAMEWORK}")
message(STATUS "CoreFoundation: ${COREFOUNDATION_FRAMEWORK}")