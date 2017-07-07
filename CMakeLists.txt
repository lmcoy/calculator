# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

set(SRC
Equation.cpp
Factor.cpp
Function.cpp
Lexer.cpp
Number.cpp
Parser.cpp
Power.cpp
State.cpp
Summand.cpp
Token.cpp
TwoOp.cpp
UnaryMinus.cpp
Variable.cpp
NumberRepr.cpp
UserFunction.cpp
)

set(HEADER
Equation.hpp
Factor.hpp
Function.hpp
Lexer.hpp
Node.hpp
Number.hpp
Parser.hpp
Power.hpp
State.hpp
Summand.hpp
Token.hpp
TwoOp.hpp
UnaryMinus.hpp
Variable.hpp
Error.h
NumberRepr.hpp
UserFunction.hpp
)

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

option(SANITIZE_ADDRESS "Enable AddressSanitizer." Off)
option(SANITIZE_THREAD "Enable ThreadSanitizer." Off)
option(SANITIZE_MEMORY "Enable MemorySanitizer." Off)

if ( (SANITIZE_ADDRESS AND SANITIZE_THREAD) OR (SANITIZE_ADDRESS AND SANITIZE_MEMORY) OR (SANITIZE_THREAD AND SANITIZE_MEMORY))
    message(FATAL_ERROR  "Cannot combine two sanitizer")
endif ()

if(SANITIZE_ADDRESS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb -fsanitize=address -fsanitize-coverage=trace-pc")
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -std=c++11 -ggdb -fsanitize=address -fsanitize-coverage=trace-pc")
endif()

if(SANITIZE_MEMORY)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdc=c++11 -ggdb -fsanitize=memory")
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -std=c++11 -ggdb -fsanitize=memory")
endif()

if(SANITIZE_THREAD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb -fsanitize=thread")
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -std=c++11 -ggdb -fsanitize=thread")
endif()

add_library(calc STATIC ${SRC} ${HEADER})

add_executable(calculator calculator.cpp)
target_link_libraries(calculator calc)

add_executable(calculatorf calculatorf.cpp)
target_link_libraries(calculatorf calc)

add_executable(writetree writetree.cpp)
target_link_libraries(writetree calc)

add_executable(tests tests.cpp)
target_link_libraries(tests gtest_main calc)
add_test(NAME example_tests COMMAND tests)
