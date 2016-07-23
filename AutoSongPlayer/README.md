Lab 1: Meeting the Beaglebone Black and the Linux
Operating System
EE 474 Summer 2016
Workstation #93
Contributors: Jiaqi Zhang, Xinxin Zhang, Heng Zhou

## Synopsis

The purpose of this lab is to get introduced to the Beaglebone and the Linux OS.

In the first part of the lab we wrote a C program to display an 8-bit counter
by using 3 LED on the Beaglebone, in order to show the number 0-7 in binary expression. 

In part 2, we used PWM pins on the Beaglebone to play music. 
The code is based on the program from part 1 and for each count value the PWM plays a
note with its frequency and duration spectified in an array. Finally, we were able to 
generate the music ‘Canon’ in PWM version.

## Files
creativeSong.c
    The main program used to blink the LED and play the song.

creativeSong.h
    The header file for the functions used in creativeSong.c.

Makefile
    The automatic compiler and cleaner.

README.md
    User's manual.

## Code Execution
1. SSH protocol is used to configure the GPIO and PWM pins.
2. GPIO pins 60, 48, and 49 are used.
3. In order to transfer the file to the board use command:
	scp music.c root@192.168.7.2:~
4. To access the pins use commands:
	echo PIN_NUMBER > /sys/class/gpio/export
	
## Notes - creativeSong.c
Firstly, we created pointers to get access and write to GPIOs and PWM pins by using fprintf(). The three GPIOs 
we use are GPIO_60, GPIO_48 and GPIO49. fflush() is used to push values on the stack to be executed. Next, we 
created a counter and used bitwise operation to compare the count value with binary versions of 1, 2 and 4 in 
order to blink the 3 lEDs in a order from 0 to 7. 

Also, we assigned a unique sound/note to each value of the counter.

The period (calculated from frequency: period = 1/frequency * 10^6) were picked to be to emulate the music,
noteC4 means note c in the 4th octave, noteC5s is the C sharp note in the 5th octave etc.
e.g.: 
    const int noteC4  = 3822191;
	const int noteC5s = 1803849;

The frequncies and lengths of all the notes of a single song are hard coded in the program.	We can switch to 
different songs by implementing different notes. The song loops over and is played again after the end of its 
notes.







