# Unofficial Scrabble C++ Library (scl)

![Github Release](https://img.shields.io/badge/release-v1.0.2-informational)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/adamcesco/scl-solver/graphs/commit-activity)
[![MIT-license](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE.txt)

## Installation

1. Install the "Utils" folder and the "scl.h" file into your repository; with each being placed in the same folder as each other.
2. Then include "scl.h" in your project source code to get access to all "scl" tools

    > `#include "scl.h"` <br> or <br>
    `#include "../directory/scl.h"`

#### All "scl" tools and objects:
 
- Tstring
- Tile
- HorizontalScrabbleVectorizer
- VerticalScrabbleVectorizer
- ScrabbleVectorizer
- CADS
- RMAC

## About This Solution

I use a test-driven solution that is primarily object-oriented. In its current state, there are separate classes for horizontal-word-vectorizers and vertical-word-vectorizers, and they are both derived classes from an abstracted Scrabble-vectorizer class - utilizing the "Prototype" Design Pattern. Throughout implementation, I continue to enhance the usability and ease of use of all classes within this solution, so that future editors or users of the class can intuitively implement all classes with little to no limitations for application.

#### Notes About This Solution's Applicability

The scrabble vectorizers within the "scl" library base their word/move generation off of the dictionary you provide to them. If you notice the absence of an otherwise viable move, that move may not be present within your provided dictionary.

#### Time Measurements and Speed

- Time Metrics on My Local Machine to Compute All Playable Moves:
  - On a board pre-set with 5 words placed on it:
    - With a rack of 0 blanks and 7 common letters: **~0.18 seconds**
    - With a rack of 1 blank and 6 common letters: **~0.47 seconds**
    - With a rack of 2 blanks and 5 common letters: **~0.88 seconds**

The average time it takes the "scl" scrabble vectorizers to compute all possible moves for a given board and rack is dependent on the number of moves that can be made from the current rack, therefor, racks with blanks take more time to compute.

I have optimized my solution to meet my personal goal of needing less than 2 seconds for all playable move generation. I am very happy to have met this goal, but I am still looking for ways to optimize this solution further. I am constantly trying to optimize/improve the logic within my program.

A main way of increasing this solution's speed was found by implementing a DAWG/GADDAG-like data structure. I wanted to explore creativity within this field, so I authored and implement two custom data structures: a "scl::CADS" object and an "scl::RMAC" object. Descriptions of these objects will be provided soon.

## Usage Of This Demo

- To run this demo of the "scl" library, provide the following command line arguments:

  >`../Data/Board.csv ../Data/scrabble_word_list.txt`

- To run the CATCH2 tests within this demo, provide no command line arguments.
- This project uses the *Data/board.csv* file for game board input.
- This project uses the "rack" variable within *main.cpp* for the input of the program's current rack of letters.
- This project uses the *Data/scrabble_word_list.txt* file as a dictionary of playable English words.

## Road Map / To-Do List

1. ~~Support the computation of words that are purely tangible to present words.~~
    - This feature is now supported.
2. ~~Implementation of word grading that accounts for word/letter multipliers.~~
    - This feature is now supported.
3. ~~Add support for blank tiles to be within the program's rack~~
    - This feature is now supported.
4. ~~Increase the usability of classes for future users/editors of the project~~
5. ~~Test and Optimize solution to prime for finalization.~~

## References

- [Appel, Andrew & Guy Jacobson. "The World's Fastest Scrabble Program". Princeton University & Carnegie Mellon University. 1988. Communications of the ACM, Vol 31](https://dl.acm.org/doi/10.1145/42411.42420)
- [Gordon, Steven."A Faster Scrabble Move Generation Algorithm". East Carolina University. 1994. Software - Practice and Experience, Vol 24.](https://doi.org/10.1002/spe.4380240205)
- [Jason Katz-Brown, John O'Laughlin, John Fultz, et al."Quackle - Scrabble Artificial Intelligence and Analysis Tool". 2016. Github.com & Quackle.org](https://github.com/quackle/quackle)
