#Build script of Anhromeda Entanglement Project
#Copyright (C) 2026 Volkan Orhan

#This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

#This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

#You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

cmake_minimum_required(VERSION 3.16)
project(AndromedaEntanglement)

set(CMAKE_CXX_STANDARD 20)

file(GLOB AndromedaEntanglementSrc
    "src/*.hpp"
    "src/*.cpp"
    "src/*.mm"
    "src/Utils/*.hpp"
    "src/Utils/*.cpp"
    "src/Utils/*.mm"
)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_library(DSP
    ${AndromedaEntanglementSrc}
)
