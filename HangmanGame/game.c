/*
 
 Summer 2016
 Authors: Xinxin Zhang, Jiaqi Zhang, Heng Zhou
 Project Introduction: This program will let users play Hangman game with the bash, BBB and LCD display.
 
 */


// Header File
#include "initialization.h"
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

// Function headers.
static FILE *value[ALL_PINS];
static void enableVal();
static void write_com(unsigned char com);
static void write_data(unsigned char data);
static void fcnSet(unsigned char num);
static void displayOff();
static void displayOn();
static void clearDisplay();
static void initialization();
static void gameInfo();
static int win(int letterFound[]);
static int researchLetter(char letter, char secretWord[], int letterFound[]);
static char readCharacter();
static int totallength;
unsigned char num;


int main() {
    // Creates the File pointers points the gpio file system
	FILE *sys;
	FILE *dir[ALL_PINS];
    
	// Creates an array for easier GPIO access
	int gpioPins[ALL_PINS] = {GPIO_PIN_48, GPIO_PIN_115, GPIO_PIN_20, GPIO_PIN_66, GPIO_PIN_67, GPIO_PIN_68, GPIO_PIN_69, GPIO_PIN_44, GPIO_PIN_45, GPIO_PIN_46, GPIO_PIN_47};

	// Specifies the file that the pointer will be used to write in.
	sys = fopen("/sys/class/gpio/export", "w");
	fseek(sys, 0, SEEK_SET);

	// Writes the value corresponding to the GPIO digital pins used
	int i;
	for(i = 0; i < ALL_PINS; i++) {
		fprintf(sys, "%d", gpioPins[i]);
		fflush(sys);
	}

	// Sets the direcion of each GPIO digital pin to output
	// Specifies the file that is used to write in the values
    char path1[50];
    char path2[50];
	int j;
	for(j = 0; j < ALL_PINS; j++) {
		sprintf(path1, "/sys/class/gpio/gpio%d/direction", gpioPins[j]);
		sprintf(path2, "/sys/class/gpio/gpio%d/value", gpioPins[j]);
		dir[j] = fopen(path1, "w");
		value[j] = fopen(path2, "w");
		fseek(dir[j], 0, SEEK_SET);
		fseek(value[j], 0, SEEK_SET);
		fprintf(dir[j], "%s", "out");
		fflush(dir[j]);
		fflush(value[j]);
	}
	
	initialization(); // Initializes the LCD diplay

    write_com(0x80); // Displays cursor at the top left corner
	write_com(0x0F); // Blinks the cursor
    
    // Lets the user choose a word for guessing
	char letter = 0; // Stores the word of choice
	unsigned char table1[] = "Please choose a word:";
	gameInfo(table1);
  	printf("Please choose a word: \n");
  	char secretWord[50];
  	gets(secretWord);
  	int totalCh = (unsigned)strlen(secretWord); // Stores the length of the word that the user typed in
  	totallength = totalCh;
 
	unsigned char table2[] = "Welcome to our Hangman game!";
	gameInfo(table2);	
  	("Welcome to our Hangman game!");
	
	unsigned char table4[100] = "The length of the word is: ";
	char length[1];
	sprintf(length, "%d", totallength);
	strcat(table4, length);
	gameInfo(table4);
    printf("\nThe length of the word is%d", totallength);
	
	unsigned char table10[100] = "What is the word?";
	gameInfo(table10);
    printf("\nWhat is the word? \n");
    
    // Plays the game: the user starts to type in the guessing char
    int letterFound[100] = {0}; // 0: Not guessed character. 1: Guessed character.
    int leftTimes = 7; // Chances left.
    int m = 0;
    while(leftTimes > 0 && !win(letterFound)) {
		unsigned char table3[100] = "You have ";
		char strLeftTime[1];
		sprintf(strLeftTime, "%d", leftTimes);
		strcat(table3, strLeftTime);
		strcat(table3, " chances left.");
		gameInfo(table3);
	
		unsigned char current[100] = "";
        for (m = 0; m < totallength; m++) {
            if (letterFound[m]) { // If the mth char is guessed.
                char secret[1];
                sprintf(secret, "%c", secretWord[m]);
                strcat(current, secret);
                printf("%c", secretWord[m]); // Show the char on screen
            } else {
                strcat(current, "_");
                printf("_"); // mth character not guessed:hold the place with "_"._
            }
        }
        
		gameInfo(current); // Shows the current status of the game (e.g ___le <- apple)

		unsigned char table5[100] = "Please input a character : ";
		gameInfo(table5);
        printf("\nPlease input a character : ");
        letter = readCharacter();
        
        // Checks whether the gussing char is in the word or not
        if (!researchLetter(letter, secretWord, letterFound)) {
  			leftTimes--; // Chances decrease by 1
			unsigned char table8[100] = "Try again!";
			gameInfo(table8);
			printf("Try again! \n"); // Guesses incorrectly
        } else {
			unsigned char table9[100] = "Genius!";
			gameInfo(table9);
			printf("Genius! \n");	// Guesses correctly
		}
			
  	}

    // Determines the use win or lose the game
	if (win(letterFound)) {
		unsigned char table6[100] = "You win! The word is :";
		strcat(table6, secretWord);
		gameInfo(table6);
	    //printf("\n\nYou win! The word is : %s\n", secretWord);
	} else {
		unsigned char table7[100] = "You lost! The word is :";
		strcat(table7, secretWord);
		gameInfo(table7);
        //printf("\n\nYou lost! The word is : %s\n", secretWord);
	}
    return 0;
}

// Sets the value for DB0 - DB7 using 8 digits from the input
void fcnSet(unsigned char num){
	fprintf(value[3], "%d", (num % 2));
	fflush(value[3]);
	num  = num /2;
	fprintf(value[4], "%d", (num % 2));
	fflush(value[4]);
	num  = num /2;
	fprintf(value[5], "%d", (num % 2));
	fflush(value[5]);
	num  = num /2;
	fprintf(value[6], "%d", (num % 2));
	fflush(value[6]);
	num  = num /2;
	fprintf(value[7], "%d", (num % 2));
	fflush(value[7]);
	num  = num /2;
	fprintf(value[8], "%d", (num % 2));
	fflush(value[8]);
	num  = num /2;
	fprintf(value[9], "%d", (num % 2));
	fflush(value[9]);
	num  = num /2;
	fprintf(value[10], "%d", (num % 2));
	fflush(value[10]);
	usleep(1000);
}

// Enables the values to be pushed to the board
void enableVal() {
	fprintf(value[2], "%d", 1);
	fflush(value[2]);
	usleep(1000);
	fprintf(value[2], "%d", 0);
	fflush(value[2]);
	usleep(1000);
}

// Controls the behavior and position of the cursor on LCD display
void write_com(unsigned char com) {
	fprintf(value[0], "%d", 0); // RS:0
	fflush(value[0]);
	usleep(1000);

	fprintf(value[1], "%d", 0); // RW:0
	fflush(value[1]);
	usleep(1000);
    
	fprintf(value[2], "%d", 0); // E:0
	fflush(value[2]);
	usleep(1000);
    
	fcnSet(com);				  
	usleep(1000);

	fprintf(value[2], "%d", 1); // E:1
	fflush(value[2]);
	usleep(1000);		 

	fprintf(value[2], "%d", 0); // E:0
	fflush(value[2]);
	usleep(1000);
}

// Sends in data for diplaying on LCD display
void write_data(unsigned char data) {
    fprintf(value[0], "%d", 1);
    fflush(value[0]);
    usleep(1000);
    
    fprintf(value[1], "%d", 0);
   	fflush(value[1]);
    usleep(1000);
    
	fprintf(value[2], "%d", 0);
	fflush(value[2]);
	usleep(1000);

	fcnSet(data);				  
	usleep(1000);

	fprintf(value[2], "%d", 1);
	fflush(value[2]);
	usleep(1000);		 

	fprintf(value[2], "%d", 0);
	fflush(value[2]);
	usleep(1000);
}

// Display off
void displayOff() {
	write_com((unsigned char) 0x08); 
}

// Display on
void displayOn() {
	write_com((unsigned char) 0x0F);
}

// Clear display
void clearDisplay() {
	write_com((unsigned char) 0x01); 
}

// Initializes the board
// A blinking cursor will be displayed on the left bottom corner of the lCD display
void initialization() {
	fprintf(value[0], "%d", 0);
	fflush(value[0]);
	usleep(1000);

	fprintf(value[1], "%d", 0);
	fflush(value[1]);
	usleep(1000);
	
   	fprintf(value[2], "%d", 0);
   	fflush(value[2]);
    usleep(1000);

	fcnSet((unsigned char) 0x3f);
	usleep(50000);
	enableVal();

	usleep(4101);
	fcnSet((unsigned char) 0x3f);
	enableVal();

	usleep(100);	
	fcnSet((unsigned char) 0x3f);
	usleep(500);
	enableVal();

	fcnSet((unsigned char) 0x3f); 
	usleep(1000);
	enableVal();

	displayOff();

	fcnSet((unsigned char) 0x06); //Entry Mode Set
	usleep(500);
	enableVal();

	displayOn();
    
	clearDisplay();
}

// Returns win if the user guesses the correct word with limited chances
int win(int letterFound[]) {
    int i = 0;
    int win = 1; // 1 for win, 0 for lost
    for (i = 0; i < totallength; i++) {
        if (letterFound[i] == 0)
            win = 0;
    }
    return win;
}

// Returns the correct char that the user guesses
int researchLetter(char letter, char secretWord[], int letterFound[]) {
    int i = 0;
    int correctLetter = 0;      // 0: char not in the word. 1: chat is in word.
    for (i = 0; secretWord[i] != '\0'; i++) {
        if (letter == secretWord[i]) {      // char is in word
            correctLetter = 1;  // one correct char
            letterFound[i] = 1; // sets its coresponding position to 1
        }
    }
    return correctLetter;
}

// Returns the yppercased typed-in char
char readCharacter() {
	char character = 0;
	character = getchar();            // Reads in a char
	character = toupper(character);   // Uppercase all letters.
    while(getchar() != '\n');         // Dose not accept '\n' as an input.
	return character; 
}

// Sends the data to LCD display
void gameInfo(unsigned char str[]) {
	clearDisplay();
	int i;
	if(strlen(str) < 16) {
		for (i = 0; str[i] != '\0'; i++){
			write_data(str[i]);	
		}		
	} else {
		for(i = 0; i < 16;i++) {
			write_data(str[i]);	
		}	
		write_com(0xC0);
		for (i = 16; str[i] != '\0'; i++){
			write_data(str[i]);	
		}
	}	
	usleep(2000000);
}



