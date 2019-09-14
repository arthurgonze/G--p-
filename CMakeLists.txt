cmake_minimum_required(VERSION 3.10)
project(Compilador_2019_3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Compilador_2019_3 main.c token.h SymbolTable.cpp SymbolTable.h)