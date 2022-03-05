cmake_minimum_required(VERSION 2.8)
project(scrabble-solver)

set(CMAKE_CXX_STANDARD 14)

set (input01 "Data/Board.csv")
set (input02 "Data/scrabble_word_list.txt")

#
##########################################

# Copy data files to build dir
foreach(file IN LISTS input01 input02)
    configure_file(${file} ${file} COPYONLY)
endforeach()

add_executable(scrabble-solver main.cpp HorizontalBoardReader/HorizontalBoardReader.cpp HorizontalBoardReader/HorizontalBoardReader.h LString/LString.cpp LString/LString.h VerticalBoardReader/VerticalBoardReader.cpp VerticalBoardReader/VerticalBoardReader.h ScrabbleReader/ScrabbleReader.cpp ScrabbleReader/ScrabbleReader.h)