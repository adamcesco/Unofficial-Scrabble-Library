# Scrabble Board Solver

![Generic badge](https://img.shields.io/badge/build-passing-color.svg)
![Github Release](https://img.shields.io/badge/release-v0.2.3--beta-informational)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/adamcesco/scrabble-solver/graphs/commit-activity)
[![MIT-license](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE.txt)

## Usage

#### Notes About Applicability

This project uses the *Data/board.csv* file for game board input, the "hand" variable within *main.cpp* for the input of the program's current hand of letters, and the *Data/scrabble_word_list.txt* file to hold a word bank of all English words. Currently, users need to manually edit the program's hand and *board.csv* in order to use this solution throughout the course of a changing Scrabble game, this will be changed soon as I implement a U.I-based solution.

Note that this solution may not compute all possible words that are in the official Scrabble dictionary, this is due to the requirement of a Scrabble-development license to obtain an official scrabble dictionary in a text-file-based format.

#### About This Solution

I use a test-driven solution that is primarily object-oriented. In its current state, there are separate classes for a horizontal-word-filter and vertical-word-filter, they are both derived classes from an abstract Scrabble-Vectorizer class. Throughout implementation, I continue to enhance the usability of all classes within this solution, so that future editors or users of the class can intuitively implement all classes with little to no limitations for application.

The average time it takes this solution to compute all possible moves for a given board and hand is dependent on the number of words within the scrabble-dictionary that have at least one letter within them that is within your given hand. Essentially, the move generation time metric depends on the number of different letters with respect to the number of playable words that have those letters. For example, with a board pre-set with 5 words on it, and a hand of 7 different common letters, the word-generation time metric is 15 seconds (on my local machine).


## Road Map / To-Do List

1. ~~Support the computation of words that are purely tangible to present words.~~
    - This feature is now supported
2. ~~Implementation of word grading that accounts for word/letter multipliers.~~
    - This feature is now supported
3. ~~Add support for blank tiles to be within the program's hand~~
    - This feature is now supported
4. ~~Increase the usability of classes for future users/editors of the project~~
5. Test and Optimize solution to prime for finalization
6. Provide an in-depth tutorial for users/editors of this library
7. Implement a console-based solution with full gameplay support.
8. Implement a U.I-based solution with full gameplay support.
9. Enhance U.X features to support the user's U.I intuition.
10. Implement dynamic gameplay structures that enhance competitive and casual play for players of this Scrabble application.

## Inspiration

- [Gordon, Steven, A. (1994), A Faster Scrabble Move Generation Algorithm. East Carolina University. Software - Practice and Experience, Vol 24: 219-232.](https://doi.org/10.1002/spe.4380240205)
- [Jason Katz-Brown, John O'Laughlin, et al. (2016), Quackle - Scrabble Artificial Intelligence and Analysis Tool.](https://people.csail.mit.edu/jasonkb/quackle/doc/how_quackle_plays_scrabble.html)