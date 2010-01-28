# Copyright 2006-2008 The FLWOR Foundation.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Adds a test to the project.
#
# Syntax:
#
#   ZORBA_ADD_TEST_SIMPLE(NAME TARGET ...) 
#   
#     NAME - the name of the added test
#     TARGET - target that will be executed when the test is run
#     ... - arguments that will be passed to the target
#
MACRO(ZORBA_ADD_TEST_SIMPLE NAME TARGET ...)
  SET(ARGS ${ARGV2} ${ARGN})
  IF (WIN32)
    GET_TARGET_PROPERTY(OUTPUT_NAME "${TARGET}" OUTPUT_NAME)
    IF (${OUTPUT_NAME} STREQUAL "OUTPUT_NAME-NOTFOUND")
      SET(OUTPUT_NAME "${TARGET}")
    ENDIF (${OUTPUT_NAME} STREQUAL "OUTPUT_NAME-NOTFOUND")
    ADD_TEST("${NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${OUTPUT_NAME}" ${ARGS})
  ELSE (WIN32)
    GET_TARGET_PROPERTY(TARGET_LOCATION "${TARGET}" LOCATION)
    ADD_TEST("${NAME}" "${TARGET_LOCATION}" ${ARGS})
  ENDIF (WIN32)
  IF (WIN32)
    GET_TARGET_PROPERTY(OUTPUT_NAME "${TARGET}" OUTPUT_NAME)
    IF (${OUTPUT_NAME} STREQUAL "OUTPUT_NAME-NOTFOUND")
      SET(OUTPUT_NAME "${TARGET}")
    ENDIF (${OUTPUT_NAME} STREQUAL "OUTPUT_NAME-NOTFOUND")
    ADD_TEST("${NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${OUTPUT_NAME}" ${ARGS})
  ELSE (WIN32)
    GET_TARGET_PROPERTY(TARGET_LOCATION "${TARGET}" LOCATION)
    ADD_TEST("${NAME}" ${TARGET_LOCATION} ${ARGS})
  ENDIF (WIN32)
  SET_TESTS_PROPERTIES ("${NAME}" PROPERTIES TIMEOUT "${NORMAL_TIMEOUT_VALUE}")
ENDMACRO(ZORBA_ADD_TEST_SIMPLE)

# This macro easies the process of adding test for store dependent executables 
# (e.g. if the rbkt tests must be added for the executables testdriver and testdriver_firststore). 
# It adds one test for each store registered. It has the same naming convection for the target as 
# the macro that is used to generated store dependent executables (thus, 
# 'testdriver' must be passed as TARGET if the test should be executed with the executables
# testdriver and testdriver_firststore).
#
# Syntax:
#
#   ZORBA_ADD_TEST_SIMPLE(NAME TARGET ...) 
#   
#     NAME - the name of the added test
#     TARGET - target that will be executed when the test is run. For each registered store, 
#              a suffix will be added to the target 
#              (similar convention than in ZORBA_GENERATE_STORE_SPECIFIC_EXES)
#     ... - arguments that will be passed to the target
#
MACRO(ZORBA_ADD_TEST_ALLSTORES NAME TARGET ...)
  SET(ARGS ${ARGV2} ${ARGN})
  FOREACH(ZORBA_STORE_NAME ${ZORBA_STORE_NAMES})
    # simplestore executable doesn't need an extension
    SET(SUFFIX)
    IF (NOT ${ZORBA_STORE_NAME} STREQUAL "simplestore")
      SET(SUFFIX "_${ZORBA_STORE_NAME}")
    ENDIF (NOT ${ZORBA_STORE_NAME} STREQUAL "simplestore")
    ZORBA_ADD_TEST_SIMPLE("${NAME}${SUFFIX}" "${TARGET}${SUFFIX}" ${ARGS})
  ENDFOREACH(ZORBA_STORE_NAME ${ZORBA_STORE_NAMES})
ENDMACRO(ZORBA_ADD_TEST_ALLSTORES)

