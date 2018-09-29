cmake_minimum_required(VERSION 3.12)
project(GoBang C)

set(CMAKE_C_STANDARD 99)

add_executable(GoBang main.c icld.h mathFunction.c mathFunction.h drawBoard.c drawBoard.h boardEvaluate.c boardEvaluate.h)