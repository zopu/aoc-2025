cmake_minimum_required(VERSION 3.20)
project(aoc2025 C)

set(CMAKE_C_STANDARD 17)

add_executable(day1
    src/day1.c
)

add_executable(day2
    src/day2.c
)

add_executable(day3
    src/day3.c
)

add_executable(day4
    src/day4.c
    src/common/grid.c
)

add_executable(day5
    src/day5.c
)

add_executable(day6
    src/day6.c
)

add_executable(day7
    src/day7.c
)

add_executable(day8
    src/day8.c
)

add_executable(day9
    src/day9.c
)
