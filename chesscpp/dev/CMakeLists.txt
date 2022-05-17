cmake_minimum_required(VERSION 3.14)

project(chesscpp)
set(CMAKE_CXX_STANDARD 20)


add_executable(
    chesscpp 
    ../src/types.cpp
    ../src/bitboard.cpp
    ../src/pieces.cpp
    ../src/moves.cpp
    ../src/main.cpp
    devutils.cpp
    main.cpp
)
target_link_libraries(
  chesscpp
  
)

