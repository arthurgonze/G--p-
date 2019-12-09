cmake_minimum_required(VERSION 3.5)
project(Compilador_2019_3)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CODELITE_USE_TARGETS ON)

add_executable(Compilador_2019_3 main.cpp token.h analyzer.cpp analyzer.h symboltable.cpp symboltable.h parser.cpp parser.h ast.cpp ast.h visitor.cpp visitor.h semantic.cpp semantic.h symbol.cpp symbol.h translator.cpp translator.h)