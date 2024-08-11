cmake_minimum_required(VERSION 3.15...3.26)
project(${SKBUILD_PROJECT_NAME} LANGUAGES C)

find_package(
  Python
  COMPONENTS Interpreter Development.Module Development.Embed
  REQUIRED)

python_add_library(crc
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/crc.c
                   WITH_SOABI)
python_add_library(p01
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/p01.c
                   ${CMAKE_SOURCE_DIR}/src/e2e/crc.c
                   WITH_SOABI)
python_add_library(p02
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/p02.c
                   ${CMAKE_SOURCE_DIR}/src/e2e/crc.c
                   WITH_SOABI)
python_add_library(p05
                   MODULE
                   ${CMAKE_SOURCE_DIR}/src/e2e/p05.c
                   ${CMAKE_SOURCE_DIR}/src/e2e/crc.c
                   WITH_SOABI)

install(TARGETS crc p01 p02 p05 LIBRARY DESTINATION e2e)
