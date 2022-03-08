cmake_minimum_required(VERSION 2.8)
project(scrabble-solver)

set(CMAKE_CXX_STANDARD 14)

set (input01 "Data/Board.csv")
set (input02 "Data/scrabble_word_list.txt")
set (input03 "Test_Folder/Board.csv")

#
##########################################

# Copy data files to build dir
foreach(file IN LISTS input01 input02 input03)
    configure_file(${file} ${file} COPYONLY)
endforeach()

add_executable(scrabble-solver main.cpp HorizontalScrabbleVectorizer/HorizontalScrabbleVectorizer.cpp HorizontalScrabbleVectorizer/HorizontalScrabbleVectorizer.h LString/LString.cpp LString/LString.h VerticalScrabbleVectorizer/VerticalScrabbleVectorizer.cpp VerticalScrabbleVectorizer/VerticalScrabbleVectorizer.h ScrabbleVectorizer/ScrabbleVectorizer.cpp ScrabbleVectorizer/ScrabbleVectorizer.h Test_Folder/tests.cpp)