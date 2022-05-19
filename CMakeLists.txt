cmake_minimum_required(VERSION 3.16.3)
project(SCL)

set(CMAKE_CXX_STANDARD 14)

set (input01 "Data/Board.csv")
set (input02 "Data/scrabble_word_list.txt")
set (input03 "Test_Folder/Board.csv")

##########################################

foreach(file IN LISTS input01 input02 input03)
    configure_file(${file} ${file} COPYONLY)
endforeach()

add_executable(scrabble-solver main.cpp Utils/CADS/CADS.cpp Utils/CADS/CADS.h Utils/HorizontalScrabbleVectorizer/HorizontalScrabbleVectorizer.cpp Utils/HorizontalScrabbleVectorizer/HorizontalScrabbleVectorizer.h Utils/Tstring/TString.cpp Utils/Tstring/Tstring.h Utils/VerticalScrabbleVectorizer/VerticalScrabbleVectorizer.cpp Utils/VerticalScrabbleVectorizer/VerticalScrabbleVectorizer.h Utils/ScrabbleVectorizer/ScrabbleVectorizer.cpp Utils/ScrabbleVectorizer/ScrabbleVectorizer.h Test_Folder/tests.cpp Utils/Tstring/Tile.cpp Utils/Tstring/Tile.h Utils/RMAC/RMAC.cpp Utils/RMAC/RMAC.h scl.h)