# Scrabble Board Solver

![Generic badge](https://img.shields.io/badge/build-failing-red.svg)
![Github Release](https://img.shields.io/badge/release-v0.2.3--beta-informational)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/adamcesco/scrabble-solver/graphs/commit-activity)
[![MIT-license](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE.txt)

## Usage

#### Notes About Applicability

This project uses the *Data/board.csv* file for game board input, the "rack" variable within *main.cpp* for the input of the program's current rack of letters, and the *Data/scrabble_word_list.txt* file to hold a word bank of all English words. Currently, users need to manually edit the program's rack and *board.csv* in order to use this solution throughout the course of a changing Scrabble game, this will be changed soon as I implement a U.I-based solution.

Note that this solution may not compute all possible words that are in the official Scrabble dictionary, this is due to the requirement of a Scrabble-development license to obtain an official scrabble dictionary in a text-file-based format. Obtaining a Scrabble-development license is coming soon,

#### About This Solution

I use a test-driven solution that is primarily object-oriented. In its current state, there are separate classes for a horizontal-word-filter and vertical-word-filter, and they are both derived classes from an abstract Scrabble-Vectorizer class. Throughout implementation, I continue to enhance the usability of all classes within this solution, so that future editors or users of the class can intuitively implement all classes with little to no limitations for application.

#### Time Measurements

The average time it takes this solution to compute all possible moves for a given board and rack is dependent on the number of words within the scrabble-dictionary that have at least one letter within them that is within your given rack. Essentially, the move generation time metric depends on the number of playable words that have a letter from your rack. For example, with a board pre-set with 5 words on it, and a rack of 7 different common letters, the word-generation time metric was ~8.7 seconds (on my local machine) to compute all playable moves.

Currently, I am working on improving the speed of generating moved of only intersectional words. Due to the similarity between generating intersectional and tangential moves, all speed-improving implementations applied to intersection-move generation can be applied to tangential words; so there is little use optimizing both move-generators at the same time. As of March 19th 2022, the time it takes on my local machine to compute all possible intersectional moves is ~1.1 seconds for any given rack of common letters.

My current hypothesis is that applying a data structure that holds all playable letters per a given coordinate would dramatically increase the speed for tangential move generation. As for intersectional move generation, this will still increase the speed, but the speed increase would pale in comparison.


## Road Map / To-Do List

1. ~~Support the computation of words that are purely tangible to present words.~~
    - This feature is now supported
2. ~~Implementation of word grading that accounts for word/letter multipliers.~~
    - This feature is now supported
3. ~~Add support for blank tiles to be within the program's rack~~
    - This feature is now supported
4. ~~Increase the usability of classes for future users/editors of the project~~
5. Test and Optimize solution to prime for finalization
    - I am currently improving the speed of this solution. The main way of increasing this solution's speed would be to implement a DAWG, a GADDAG, or a custom data structure. I want to explore creativity within this field, so I plan to implement a custom data structure that should reduce the number of characters parsed through during move generation. This new data structure in tandem with my custom CADS data structure should decrease the computation time of this solution to be less than 3 seconds. If the improved solution is not faster than expected, I then plan to implement a DAWG-based CADS data structure which should increase computation speed even more. My goal is to have this solution only require 2 seconds of computation time to produce all possible moves per turn.
6. Provide an in-depth tutorial for users/editors of this library
7. Implement a console-based solution with full gameplay support.
8. Implement a U.I-based solution with full gameplay support.
9. Enhance U.X features to support the user's U.I intuition.
10. Implement dynamic gameplay structures that enhance competitive and casual play for players of this Scrabble application.

## References

- [Appel, Andrew & Guy Jacobson. "The World's Fastest Scrabble Program". Princeton University & Carnegie Mellon University. 1988. Communications of the ACM, Vol 31](https://dl.acm.org/doi/10.1145/42411.42420)
- [Gordon, Steven."A Faster Scrabble Move Generation Algorithm". East Carolina University. 1994. Software - Practice and Experience, Vol 24.](https://doi.org/10.1002/spe.4380240205)
- [Jason Katz-Brown, John O'Laughlin, John Fultz, et al."Quackle - Scrabble Artificial Intelligence and Analysis Tool". 2016. Github.com & Quackle.org](https://github.com/quackle/quackle)
