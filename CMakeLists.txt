cmake_minimum_required(VERSION 3.8)

project(Nowide)

enable_testing()

option(RUN_WITH_WINE "Use wine to run tests" OFF)


if(NOT LIBDIR)
  set(LIBDIR lib CACHE STRING "Library installation directory" FORCE)
endif()

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")
else()
  set(CMAKE_DEBUG_POSTFIX "-debug")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  add_compile_options(-Wall)
elseif(MSVC)
  add_compile_options(/EHsc /W3)
endif()

set(NOWIDE_TESTS
  test_convert
  test_stdio
  test_fstream
)

foreach(TEST ${NOWIDE_TESTS})
  add_executable(${TEST} test/${TEST}.cpp)
  target_include_directories(${TEST} PRIVATE ${Nowide_SOURCE_DIR})
  if(RUN_WITH_WINE)
    add_test(NAME ${TEST} WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./${TEST}.exe)
  else()
    add_test(${TEST} ${TEST})
  endif()
endforeach()

add_library(nowide SHARED src/iostream.cpp)
target_include_directories(nowide PUBLIC
  $<BUILD_INTERFACE:${Nowide_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(nowide PROPERTIES VERSION 0.0.0 SOVERSION 0)
target_compile_definitions(nowide PRIVATE DLL_EXPORT)
set_target_properties(nowide PROPERTIES
  CLEAN_DIRECT_OUTPUT 1
  OUTPUT_NAME "nowide"
)

add_library(nowide-static STATIC src/iostream.cpp)
target_include_directories(nowide-static PUBLIC
  $<BUILD_INTERFACE:${Nowide_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(nowide-static PROPERTIES
  CLEAN_DIRECT_OUTPUT 1
  OUTPUT_NAME "nowide"
)
if(MSVC)
  set_target_properties(nowide-static PROPERTIES
    PREFIX "lib"
  )
endif()

add_executable(test_iostream_shared test/test_iostream.cpp)
target_compile_definitions(test_iostream_shared PRIVATE DLL_EXPORT)
target_link_libraries(test_iostream_shared nowide)

add_executable(test_iostream_static test/test_iostream.cpp)
target_link_libraries(test_iostream_static nowide-static)

add_executable(test_system test/test_system.cpp)
target_include_directories(test_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_env_proto test/test_env.cpp)
target_include_directories(test_env_proto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_env_win test/test_env.cpp)
target_include_directories(test_env_win PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(test_env_win PRIVATE NOWIDE_TEST_INCLUDE_WINDOWS)

set(OTHER_TESTS test_iostream_shared test_iostream_static test_env_win test_env_proto)

if(RUN_WITH_WINE)
  foreach(T ${OTHER_TESTS})
    add_test(NAME ${T} WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./${T}.exe)
  endforeach()

  add_test(NAME test_system_n WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./test_system.exe "-n")
  add_test(NAME test_system_w WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./test_system.exe "-w")
else()
  foreach(T ${OTHER_TESTS})
    add_test(${T} ${T})
  endforeach()

  add_test(test_system_n test_system "-n")
  add_test(test_system_w test_system "-w")
endif()


install(TARGETS nowide nowide-static EXPORT NowideTargets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBDIR}
  ARCHIVE DESTINATION ${LIBDIR}
)
if(MSVC)
  install(FILES $<TARGET_PDB_FILE:nowide> DESTINATION bin OPTIONAL)
endif()
install(EXPORT NowideTargets
  FILE NowideTargets.cmake
  NAMESPACE Nowide::
  DESTINATION lib/cmake/Nowide
)
install(DIRECTORY nowide DESTINATION include)

