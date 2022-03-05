# Scrabble Board Solver

##### Project Name: Scrabble Solver
##### Author: Adam C. Escobedo
##### Start Date: Feb 1st, 2022

## Notes About Applicability

This project uses the Data/board.csv file for game board input, the "hand" variable within main.cpp for the input of the program's current hand of letters, and the Data/scrabble_word_list.txt file to hold a word bank of all english words. Currently, users need to manually edit the programs hand and board.csv in order to use this solution throughout the course of a changing Scrabble game, this will be changed soon as I implement a U.I based solution.

Note that this solution in it's current state does not account for word/letter multipliers within word grading, but this is soon to be implemented. This solution also may compute words that are not officially playable with Scrabble, this is due to the requirement of a Scrabble-development license to obtain an offical scrabble dictionary in a text-file-based format. This solution also only computes words that are intersecting with current words, meaning that words that are purely tangible to present words are not calculated, this is soon to change.

## About This Solution

I use a test-driven solution that is primarily object oriented. In it's current state, there are seprate classes for a horizontal-word-filter and vertical-word-filter, they are both derived classes from an abstract Scrabble-Reader class. Throughout implementation I continue to enhance the usability of all classes within this soltuion, so that future editors or users of the class can intuitively implement all classes with little to no limitations for application.

## Road Map / To-Do List

1. Increase usability of classes for future users/editors of the project
2. The computation of words that are purely tangible to present words.
3. 1. Implementation of word grading that accounts for word/letter multipliers.
4. Console-based full game play support.
5. U.I-based full game play support.
6. Inhanced U.X features that support user's U.I intuition.
7. Dynamic game play structures that inhance competitive and casual play for players of this Scrabble application.
