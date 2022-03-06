# Scrabble Board Solver

##### Project Name: Scrabble Solver
##### Author: Adam C. Escobedo
##### Start Date: Feb 10th, 2022

## Notes About Applicability

This project uses the Data/board.csv file for game board input, the "hand" variable within main.cpp for the input of the program's current hand of letters, and the Data/scrabble_word_list.txt file to hold a word bank of all English words. Currently, users need to manually edit the program's hand and board.csv in order to use this solution throughout the course of a changing Scrabble game, this will be changed soon as I implement a U.I-based solution.

Note that this solution in its current state does not account for word/letter multipliers within word grading, but this is soon to be implemented. This solution may not compute words that are possible within the official Scrabble dictionary, this is due to the requirement of a Scrabble-development license to obtain an official scrabble dictionary in a text-file-based format.

## About This Solution

I use a test-driven solution that is primarily object-oriented. In its current state, there are separate classes for a horizontal-word-filter and vertical-word-filter, they are both derived classes from an abstract Scrabble-Reader class. Throughout implementation I continue to enhance the usability of all classes within this solution, so that future editors or users of the class can intuitively implement all classes with little to no limitations for application.

## Road Map / To-Do List

1. ~~Support the computation of words that are purely tangible to present words.~~
2. Implementation of word grading that accounts for word/letter multipliers.
3. Optimize solution
4. Increase the usability of classes for future users/editors of the project
5. Provide an in-depth tutorial for users/editors of the class
6. Implement a console-based solution with full gameplay support.
7. Implement a U.I-based solution with full gameplay support.
8. Enhance U.X features to support the user's U.I intuition.
9. Implement dynamic gameplay structures that enhance competitive and casual play for players of this Scrabble application.
