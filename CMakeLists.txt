cmake_minimum_required(VERSION 2.8)
project(scrabble-solver)

set(CMAKE_CXX_STANDARD 14)

add_executable(scrabble-solver main.cpp HandManager/HandManager.cpp HandManager/HandManager.h BoardReader/BoardReader.cpp BoardReader/BoardReader.h Letter/Letter.cpp Letter/Letter.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/englishWords.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)