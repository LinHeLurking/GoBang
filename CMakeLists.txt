cmake_minimum_required(VERSION 3.12)
project(GoBang C)

set(CMAKE_C_STANDARD 99)

add_executable(GoBang main.c icld.h drawBoard.c drawBoard.h boardEvaluate.c boardEvaluate.h statusBoard.c statusBoard.h interact.c interact.h statusSearch.c statusSearch.h mathFunc.c mathFunc.h AC_Automachine.c AC_Automachine.h)