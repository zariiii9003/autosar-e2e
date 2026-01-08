cmake_minimum_required(VERSION 3.30.3)
project(${SKBUILD_PROJECT_NAME} LANGUAGES C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Python
             REQUIRED
             COMPONENTS
             Interpreter
             Development.Module
             ${SKBUILD_SABI_COMPONENT})

if(NOT "${SKBUILD_SABI_COMPONENT}" STREQUAL "")
    set(PY_ABI_OPTIONS "WITH_SOABI" "USE_SABI" "3.11")
else()
    set(PY_ABI_OPTIONS "WITH_SOABI")
endif()

python_add_library(crc
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/crc.c
                   ${PY_ABI_OPTIONS})
python_add_library(p01
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/p01.c
                   ${PY_ABI_OPTIONS})
python_add_library(p02
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/p02.c
                   ${PY_ABI_OPTIONS})
python_add_library(p04
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/p04.c
                   ${PY_ABI_OPTIONS})
python_add_library(p05
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/p05.c
                   ${PY_ABI_OPTIONS})
python_add_library(p06
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/p06.c
                   ${PY_ABI_OPTIONS})
python_add_library(p07
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/p07.c
                   ${PY_ABI_OPTIONS})

# Add libraries
add_library(crclib STATIC ${CMAKE_SOURCE_DIR}/src/e2e/crclib.c)
add_library(util STATIC ${CMAKE_SOURCE_DIR}/src/e2e/util.c)

# Link against util
target_link_libraries(crc PRIVATE crclib)
target_link_libraries(p01 PRIVATE crclib)
target_link_libraries(p02 PRIVATE crclib)
target_link_libraries(p04 PRIVATE crclib util)
target_link_libraries(p05 PRIVATE crclib util)
target_link_libraries(p06 PRIVATE crclib util)
target_link_libraries(p07 PRIVATE crclib util)

install(TARGETS crc p01 p02 p04 p05 p06 p07 LIBRARY DESTINATION e2e)
