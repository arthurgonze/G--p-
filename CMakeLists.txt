cmake_minimum_required(VERSION 3.5)
project(Compilador_2019_3)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CODELITE_USE_TARGETS ON)

add_executable(Compilador_2019_3 main.cpp token.h lexical/analyzer.cpp lexical/analyzer.h SymbolTable.cpp SymbolTable.h syntactical/Parser.cpp syntactical/Parser.h syntactical/AST.cpp syntactical/AST.h)