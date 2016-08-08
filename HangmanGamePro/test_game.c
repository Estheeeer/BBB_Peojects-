/* Summer 2016
* Xinxin Zhang, Jiaqi Zhang, Heng Zhou
* test_game.c: Plays the Hangman game on the LCD board driven by BBB.
*/


#include "test_game.h" //Header file for test_game.c
static int totallength;
unsigned char num;

const int noteC5  = 1911132; //Musical notes i  frequencies.
const int noteD5  = 1702620;
const int noteE5  = 1517451;
const int noteF5  = 1431721;

int main(void) {
	FILE *sys, *sys2, *noteduty, *noteperiod;
	FILE *dir[ALL_PINS];
    
	// Creates an array for easier GPIO access
	int gpioPins[ALL_PINS] = {BLUE, RED, GREEN};

	// Specifies the file that the pointer will be used to write in.
	sys = fopen("/sys/class/gpio/export", "w");
	fseek(sys, 0, SEEK_SET);
	sys2 = fopen("/sys/devices/bone_capemgr.9/slots", "w");
	fseek(sys2, 0, SEEK_END);

	// Writes the value corresponding to the GPIO digital pins used
	int i;
	for(i = 0; i < ALL_PINS; i++) {
		fprintf(sys, "%d", gpioPins[i]);
		fflush(sys);
	}

	// Writes the PWM used and the corresponding PWM output pin
	fprintf(sys2, "am33xx_pwm");
	fflush(sys2);
	fprintf(sys2, "bone_pwm_P8_19");
	fflush(sys2);

	// Sets the pointers to the appropriate duty, period and run files
	noteduty = fopen("/sys/devices/ocp.3/pwm_test_P8_19.15/duty", "w");
	noteperiod = fopen("/sys/devices/ocp.3/pwm_test_P8_19.15/period", "w");

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
	
    //Defines the kernel that is read from.
    int fd;
	fd = open(NEW_CHAR_DIR, O_RDWR);
    if (fd < 0)
	{
		printf("File %s cannot be opened\n", NEW_CHAR_DIR);
		exit(1);
	}
    // Lets the user choose a word for guessing
	char letter = 0; 				// Stores the word of choice
	char write_buf[100];			// Pass in the char to kernel module

    //Keeps the red and green light shut down.
	red(0);
	green(0);

	strcpy(write_buf, "Welcome to our Hangman game!"); // copy the string to the address that write_buf points to
	write(fd, write_buf, sizeof(write_buf)); 		   // write the data into the kernel module 
  	printf("Welcome to our Hangman game!");            // prints out the message on the terminal


    // asks user A to think of a random word and store the word into an array.
	strcpy(write_buf, "Please choose a word:");
	write(fd, write_buf, sizeof(write_buf));
  	printf("\nPlease choose a word: \n");
  	
  	char secretWord[50];
  	gets(secretWord);
  	int k = 0;
  	while (secretWord[k] != '\0'){
  		secretWord[k] = toupper(secretWord[k]);
  		k++;
  	}
  	int totalCh = (unsigned)strlen(secretWord); // Stores the length of the word that the user typed in
  	totallength = totalCh;

	unsigned char table4[100] = "The length of the word is: ";
	char length[1];
	sprintf(length, "%d", totallength);
	strcat(table4, length);

    // asks user B to guess what the word is, char by char.
	strcpy(write_buf, table4);
	write(fd, write_buf, sizeof(write_buf));
    printf("\nThe length of the word is %d.", totallength);

	

	strcpy(write_buf, "What is the word?");
	write(fd, write_buf, sizeof(write_buf));
    printf("\nWhat is the word?");



    // Plays the game: the user starts to type in the guessing char
    int letterFound[100] = {0}; // 0: Not guessed character. 1: Guessed character.
    int leftTimes = 5; // Chances left.
    int m = 0;
    while(leftTimes > 0 && !win(letterFound)) {
		
		unsigned char table3[100] = "You have ";
		char strLeftTime[1];
		sprintf(strLeftTime, "%d", leftTimes);
		strcat(table3, strLeftTime);
		strcat(table3, " chances left.");
		strcpy(write_buf, table3);
		printf("\nYou have %d chances left. \n", leftTimes);
		write(fd, write_buf, sizeof(write_buf));
		
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
        
		strcpy(write_buf, current);
		write(fd, write_buf, sizeof(write_buf)); // Shows the current status of the game (e.g ___le <- apple)

		printf("\nPlease input a character: ");
		strcpy(write_buf, "Please input a character: ");
		write(fd, write_buf, sizeof(write_buf));

        letter = readCharacter();

        // Checks whether the gussing char is in the word or not.
        // Green light on for right, red light on for wrong.
        if (!researchLetter(letter, secretWord, letterFound)) {
  			leftTimes--; // Chances decrease by 1
			strcpy(write_buf, "Try again!");
			printf("Try again! \n"); // Guesses incorrectly
			write(fd, write_buf, sizeof(write_buf));
			green(0);
			red(1);
        } else {
			strcpy(write_buf, "Genius!");
			printf("Genius! \n");	// Guesses correctly
			write(fd, write_buf, sizeof(write_buf));
			green(1);
			red(0);
		}
  	}

    // Determines if the user win or lose the game
    // Both green and red light on for winning.
    // Both lights off for losing.
	if (win(letterFound)) {
		unsigned char table6[100] = "You win! The word is :";
		strcat(table6, secretWord);
		strcpy(write_buf, table6);
	   	printf("\nYou win! The word is : %s \n", secretWord);
		write(fd, write_buf, sizeof(write_buf));

		green(1);
        red(1);
	    int win[] = {noteC5, noteD5, noteE5, noteF5};
	    int winDuration[] = {2, 1, 1, 1};
	    int i;
	    while(1){
		    for(i=0;i < 4; i++) {
		    	int length = winDuration[i];
			playNote(noteperiod, noteduty, win[i], win[i]/3);
			usleep(250000*length);
		    }
	    }
	} else {
		unsigned char table7[100] = "You lost! The word is :";
		strcat(table7, secretWord);
		strcpy(write_buf, table7);
		write(fd, write_buf, sizeof(write_buf));
        printf("\nYou lost! The word is : %s \n", secretWord);

	    green(0);
	    red(0);
	    int lose[] = {noteF5, noteE5, noteD5, noteC5};
	    int loseDuration[] = {2, 2, 2, 2};
	    int i;
	    while(1){
		    for(i=0;i < 4; i++) {
		    	int length = loseDuration[i];
				playNote(noteperiod, noteduty, lose[i], lose[i]/3);
				usleep(250000*length);
		    }
	   }
	}

	close(fd);
    return 0;
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

// Turns on red light.
void red(int i) {
	fprintf(value[1], "%d", i);
	fflush(value[1]);
}
// Turns on green light.
void green(int i) {
	fprintf(value[2], "%d", i);
	fflush(value[2]);
}

// changes the period and dutycycle of each note
void playNote(FILE *noteperiod, FILE *noteduty, int period, int duty) {
	fprintf(noteperiod, "%d", period);
	fflush(noteperiod);
	fprintf(noteduty, "%d", duty);
	fflush(noteduty);		
}

