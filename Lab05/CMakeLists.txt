cmake_minimum_required(VERSION 3.10.0)
project(Tradutor)

# Configuração do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Release)
set(SOURCES ast.cpp gen.cpp checker.cpp parser.cpp symtable.cpp error.cpp tradutor.cpp)

# Configuração do Flex
find_package(FLEX REQUIRED)
FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc COMPILE_FLAGS "-+")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR})

add_executable(tradutor ${SOURCES} ${FLEX_Lexer_OUTPUTS})
