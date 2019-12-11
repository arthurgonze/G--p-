cmake_minimum_required(VERSION 3.5)
project(Compilador_2019_3)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CODELITE_USE_TARGETS ON)

add_executable(Compilador_2019_3 main.cpp token.h lexycal.cpp lexycal.h symbol_table.cpp symbol_table.h
        parser.cpp parser.h ast.cpp ast.h visitor_ast.cpp visitor_ast.h semantic.cpp semantic.h
        symbol.cpp symbol.h intermediate_code.cpp intermediate_code.h ict.cpp ict.h
        visitor_ict.cpp visitor_ict.h translator.cpp translator.h)