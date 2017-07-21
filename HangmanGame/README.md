Summer 2016
Hangman Game
Contributors: Xinxin Zhang, Jiaqi Zhang, Heng Zhou

## Synopsis

This lab introduces us to more capabilities of the BBB system. We also investigated the probabilities of LCD.

All codes are written in c and executed on Beaglebone.

For the first part, we tested built-in scheduler of our system. The voltage of a GPIO pin is made to go low and high in loops. We used an oscilloscope to detect and display the voltage on the designated GPIO pin. 

Secondly, we run through an initialization routine to set up the board for display.

Then we implemented a Hangman game that would allow user A to choose  a word and user B to guess the word character by character.

## Hardware Setup
1.VDD is set to 5V.
2.VCC is wired to a 100K Ohm Potentiometer that is used to control the brighness of the board.
3.VSS is set to ground.
4.Bits 0-7 are all used and connected to GPIO pins.
    DB0 ---- GPIO_PIN_66
    DB1 ---- GPIO_PIN_67
    DB2 ---- GPIO_PIN_68
    DB3 ---- GPIO_PIN_69
    DB4 ---- GPIO_PIN_44
    DB5 ---- GPIO_PIN_45
    DB6 ---- GPIO_PIN_46
    DB7 ---- GPIO_PIN_47
5. Enable, RS and RW are also wired to GPIO pins.
    RS ---- GPIO_PIN_48
    R/W ---- GPIO_PIN_115
    E ---- GPIO_PIN_20

## Included Fiels

initialization.c
    Set up the board and displays a cursor at the left top corner of the board. This code has to be run before displaying any other info on the board.
    
game.c
    Plays the Hangman game. User inputs from terminal, game instructions show up on both Terminal and LCD board. Codes in initialization.c are inplemented in this program to get the board ready for display. No need to run initialization.c before running game.c.

initialization.h
    Header file for initialization.c and game.c.

