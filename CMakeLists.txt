# Copyright (c) 2023, RPTU Kaiserslautern-Landau
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors:
#    Thomas Psota
#    Lukas Steiner
#    Derek Christ

###############################################
###                 DRAMSys                 ###
###############################################
cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "DRAMSys")

project(${PROJECT_NAME} VERSION 5.0)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(enable_extensions)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

if(DRAMSYS_ENABLE_COVERAGE)
    include(coverage)
endif()

### Project settings ###
option(DRAMSYS_BUILD_TESTS "Build DRAMSys unit tests" OFF)
option(DRAMSYS_BUILD_BENCHMARKS "Build DRAMSys benchmarks" OFF)
option(DRAMSYS_BUILD_CLI "Build DRAMSys Command Line Tool" ${PROJECT_IS_TOP_LEVEL})
option(DRAMSYS_BUILD_TRACE_ANALYZER "Build DRAMSys Trace Analyzer" OFF)

# Use sane defaults for FetchContent:
# In case we are the top-level project, get everything by default
# In case we are included in another project, the user might want to provide their own system dependencies
option(DRAMSYS_USE_FETCH_CONTENT "Enable the FetchContent module" ${PROJECT_IS_TOP_LEVEL})
option(DRAMSYS_USE_FETCH_CONTENT_INTERNAL "Enable FetchContent to provide internal dependencies" ${DRAMSYS_USE_FETCH_CONTENT})
option(DRAMSYS_USE_FETCH_CONTENT_SYSTEMC "Enable FetchContent to provide SystemC" ${DRAMSYS_USE_FETCH_CONTENT})
option(DRAMSYS_USE_FETCH_CONTENT_SQLITE3 "Enable FetchContent to provide SQLite3" ${DRAMSYS_USE_FETCH_CONTENT})
option(DRAMSYS_USE_FETCH_CONTENT_NLOHMANN_JSON "Enable FetchContent to provide nlohmann_json" ${DRAMSYS_USE_FETCH_CONTENT})

# TODO: Remove as soon as DRAMPower is updated
set(DRAMSYS_SHARED_LIBS OFF)

if(DEFINED DRAMSYS_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${DRAMSYS_SHARED_LIBS})
endif()

### DRAMSys directories ###
set(DRAMSYS_SOURCE_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(DRAMSYS_LIBRARY_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(DRAMSYS_TESTS_DIR       "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(DRAMSYS_RESOURCE_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/configs")
set(DRAMSYS_EXTENSIONS_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/extensions")
set(DRAMSYS_3RDPARTY_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty")

###############################################
###           Library Settings              ###
###############################################

if (DRAMSYS_USE_FETCH_CONTENT)
    include(FetchContent)

    if (DRAMSYS_USE_FETCH_CONTENT_NLOHMANN_JSON)
        FetchContent_Declare(nlohmann_json
            # URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
            URL "file://${DRAMSYS_3RDPARTY_DIR}/json.tar.xz"
            OVERRIDE_FIND_PACKAGE
        )

        FetchContent_MakeAvailable(nlohmann_json)
    endif()

    if (DRAMSYS_USE_FETCH_CONTENT_SQLITE3)
        add_subdirectory(${DRAMSYS_LIBRARY_DIR}/sqlite3)
    endif()

    if (DRAMSYS_USE_FETCH_CONTENT_SYSTEMC)
        set(DISABLE_COPYRIGHT_MESSAGE TRUE)

        FetchContent_Declare(
            SystemCLanguage
            # URL https://github.com/accellera-official/systemc/archive/refs/tags/2.3.4.tar.gz
            URL "file://${DRAMSYS_3RDPARTY_DIR}/systemc-2.3.4.tar.gz"
            OVERRIDE_FIND_PACKAGE
        )

        # TODO: remove with SystemC 3.0
        set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD} CACHE STRING "")

        FetchContent_MakeAvailable(SystemCLanguage)

        # Set include directories to SYSTEM to suppress warnings
        set_target_properties(systemc PROPERTIES SYSTEM TRUE)
    endif()

    if (DRAMSYS_USE_FETCH_CONTENT_INTERNAL)
        FetchContent_Declare(
            DRAMPower
            # GIT_REPOSITORY https://github.com/tukl-msd/DRAMPower
            GIT_REPOSITORY https://gitee.com/masa-laboratory/DRAMPower
            GIT_TAG ebd9ff7
            OVERRIDE_FIND_PACKAGE
        )

        FetchContent_MakeAvailable(DRAMPower)
    endif()
endif()

find_package(SystemCLanguage REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(nlohmann_json REQUIRED)

find_package(DRAMPower)

if (DRAMPower_FOUND)
    message(STATUS "Building with DRAMPower")
else()
    message(STATUS "Building without DRAMPower")
endif()

###############################################
###           Source Directory              ###
###############################################

add_subdirectory(src/configuration)
add_subdirectory(src/libdramsys)

if(DRAMSYS_BUILD_CLI)
    add_subdirectory(src/simulator)
endif()

if(DRAMSYS_BUILD_TRACE_ANALYZER)
    add_subdirectory(src/traceAnalyzer)
endif()

dramsys_enable_extensions()

###############################################
###           Test Directory                ###
###############################################

if(DRAMSYS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

###############################################
###         Benchmark Directory             ###
###############################################

if(DRAMSYS_BUILD_BENCHMARKS)
    add_subdirectory(benches)
endif()
