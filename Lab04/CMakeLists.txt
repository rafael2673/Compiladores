cmake_minimum_required(VERSION 3.10.0)
project(Sample)

# Configuração do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Release)
set(SOURCES parser.cpp sample.cpp)

# Configuração do Flex
find_package(FLEX REQUIRED)
FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc COMPILE_FLAGS "-+")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_GENERATOR "Unix Makefiles")
# Executável depende também da saída do Flex
add_executable(sample ${SOURCES} ${FLEX_Lexer_OUTPUTS})
