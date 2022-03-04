cmake_minimum_required(VERSION 2.8)
project(scrabble-solver)

set(CMAKE_CXX_STANDARD 14)

add_executable(scrabble-solver main.cpp HorizontalBoardReader/HorizontalBoardReader.cpp HorizontalBoardReader/HorizontalBoardReader.h LString/LString.cpp LString/LString.h VerticalBoardReader/VerticalBoardReader.cpp VerticalBoardReader/VerticalBoardReader.h ScrabbleReader/ScrabbleReader.cpp ScrabbleReader/ScrabbleReader.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/englishWords.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)