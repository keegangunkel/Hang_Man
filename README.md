# Hang Man
* Application Development 1 Final Project at Dunwoody College of Technology
### Contributers
* Keegan Gunkel
* Cooper Hanson
* Anthony Steffl
* George Shea
* Namduk Tsering
---

## Introduction
The goal of this project is to create a command-line implementation of the classic game Hangman using C.
The game will allow users to guess words, track their progress, and display the hangman figure,
refreshing as the user guesses incorrectly.

* [Project Overview](#introduction)
* [Installation](#installation)
* [How to Play](#how-to-play)
* [Contribution Roles](#contribution-roles)
* [Feature Wishlist](#feature-wishlist)
* [Conclusion](#conclusion)

## Installation
### Dependencys
`sudo apt-get -y install git libcurl4-openssl-dev libjansson-dev buildessential`
### Build
* Clone the repository
* Run `make`to automatically compile

## How to Play
* Run the `./main` file within the repository to play the game in a terminal
* The program will select a random word and display the length with `_` underscores
* The program will then wait for the user to input one character at a time
* The program will replace the `_`'s with the input character where it occurs in the word
* If there character has no occurences in the word, a new part of the hangman will be drawn
* The game ends when either the entire word is revealed or the hangman has been fully drawn
* There is a bank to show which characters have already been guessed
* Enter `<CTRL> + c` at any time to exit
* Have fun !

## Contribution Roles
### Cooper Hanson
* Word selection
### George Shea
* Collecting user input and scoring
### Keegan Gunkel
* Project manager
* Technical Writer
* Game sound design and playback
### Anthony Steffl
* User Interface
* Graphics
### Namduk Tsering
* Hints
* Power-Ups

## Feature Wishlist
- [x] Random words
- [ ] Time limit
- [ ] Multiplayer mode -- take turns guessing. Player who either guesses the word or __does not__ fail the last guess wins
    * Take turns guessing
    * Player who guesses the word wins
    * Player who __does not__ fail the last guess wins
    * -- or -- Player with the most correct guesses wins
    * If guess exceeds time limit, player loses
- [ ] Hints -- partial definition, synonyms?
- [ ] Power Ups
    * Bomb -- remove one of the incorrect letters from the character bank
    * Hint -- described above
    * Skip -- Skip your turn in multiplayer
    * Double Guess -- guess two characters, if either are correct the hangman will not be drawn
    * Wild card -- can be placed anywhere on the word, only replaced one occurence of the letter
- [ ] Difficulties

## Conclusion
We hope to show off the C programming skills we have learned throughout our semester and even deepen our understanding and develop our skills in programming, design principles, problem solving, and collaboration on a software project.

