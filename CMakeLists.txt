cmake_minimum_required(VERSION 3.10)
project(Compilador_2019_3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Compilador_2019_3 main.cpp token.h lexical/analyzer.cpp lexical/analyzer.h token.c lexical/error.c lexical/error.h SymbolTable.cpp SymbolTable.h lexical/io.c lexical/io.h)