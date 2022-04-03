# Scrabble Board Solver

![Generic badge](https://img.shields.io/badge/build-passing-color.svg)
![Github Release](https://img.shields.io/badge/release-v0.2.5--beta-informational)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/adamcesco/scrabble-solver/graphs/commit-activity)
[![MIT-license](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE.txt)

## Usage

- To run the scrabble-board solver, provide the following command line argument:
 
   >*../Data/Board.csv*

- To run the CATCH tests, provide no command line arguments.

#### Notes About Applicability

This project uses the *Data/board.csv* file for game board input, the "rack" variable within *main.cpp* for the input of the program's current rack of letters, and the *Data/scrabble_word_list.txt* file to hold a word bank of all English words. Currently, users need to manually edit the program's rack and *board.csv* in order to use this solution throughout the course of a changing Scrabble game, this will be changed soon as I implement a U.I-based solution.

Note that this solution may not compute all possible words that are in the official Scrabble dictionary, this is due to the requirement of a Scrabble-development license to obtain an official scrabble dictionary in a text-file-based format. Obtaining a Scrabble-development license is coming soon.

## About This Solution

I use a test-driven solution that is primarily object-oriented. In its current state, there are separate classes for a horizontal-word-filter and vertical-word-filter, and they are both derived classes from an abstract Scrabble-Vectorizer class. Throughout implementation, I continue to enhance the usability of all classes within this solution, so that future editors or users of the class can intuitively implement all classes with little to no limitations for application. I want the usability of this class to be so versatile that I could publish this project as a Scrabble-solver library for other C++ users who want to develop their own scrabble-based projects.

#### Time Measurements

The average time it takes this solution to compute all possible moves for a given board and rack is dependent on the number of words that can be made from the current rack. For example, with a board pre-set with 5 words on it, and a rack of 7 different common letters, the word-generation time metric was ~0.67 seconds (on my local machine) to compute all playable moves.

I have optimized my solution to meet my personal goal of needing less than 2 seconds for all playable move generation. I am very happy to have met this goal, but I am still looking for ways to optimize this solution further. I am constantly trying to optimize/improve the logic within my program, and I am investigating the potential use of a database to handle queries for board/rack related pre-computed information.

Another main way of increasing this solution's speed would be to implement a DAWG, a GADDAG, or a custom data structure of similar nature. I want to explore creativity within this field, so I plan to implement a custom data structure. I am drafting the logic and use-case map for an improved CADS data structure. This new approach would feature a DAWG-based CADS system that would dramatically reduce the amount of characters traversed over throughout computation.

## Road Map / To-Do List

1. ~~Support the computation of words that are purely tangible to present words.~~
    - This feature is now supported.
2. ~~Implementation of word grading that accounts for word/letter multipliers.~~
    - This feature is now supported.
3. ~~Add support for blank tiles to be within the program's rack~~
    - This feature is now supported.
4. ~~Increase the usability of classes for future users/editors of the project~~
5. Test and Optimize solution to prime for finalization.
6. Publish this project as a C++ library for C++ Scrabble-solver developers and users.
7. Provide an in-depth tutorial for users/editors of this library.
8. Implement a console-based solution with full gameplay support.
9. Implement a U.I-based solution with full gameplay support.
10. Enhance U.X features to support the user's U.I intuition.
11. Implement dynamic gameplay structures that enhance competitive and casual play for players of this Scrabble application.

## References

- [Appel, Andrew & Guy Jacobson. "The World's Fastest Scrabble Program". Princeton University & Carnegie Mellon University. 1988. Communications of the ACM, Vol 31](https://dl.acm.org/doi/10.1145/42411.42420)
- [Gordon, Steven."A Faster Scrabble Move Generation Algorithm". East Carolina University. 1994. Software - Practice and Experience, Vol 24.](https://doi.org/10.1002/spe.4380240205)
- [Jason Katz-Brown, John O'Laughlin, John Fultz, et al."Quackle - Scrabble Artificial Intelligence and Analysis Tool". 2016. Github.com & Quackle.org](https://github.com/quackle/quackle)
