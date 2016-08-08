/*
Date: 07/07/2016
Authors: Xinxin Zhang, Jiaqi Zhang, Heng Zhou
Project: This program controls the LEDs using GPIOs on the beaglebone interface. It displays a looping count 
of 0 to 7 in binary between 3 LEDs. Further more, it reads in the frequencies of music notes and play it using
 a speaker embedded on the breadboard.

*/


#include "creativeSong.h" // for header file
#include <stdio.h>        // for File IO and printf
#include <time.h>         // for usleep
#include <unistd.h>       // for sleep


#define GPIO_PIN_60  60   // LED pin #60
#define GPIO_PIN_48  48   // LED pin #48
#define GPIO_PIN_49  49   // LED pin #49


// Sets period of notes used in nanoseconds as constants. Here are the notes from the 4th octave to the 6th octave.

	const int noteC4  = 3822191;
	const int noteC4s = 3607764;
	const int noteD4  = 3405299;
	const int noteD4s = 3214090;
	const int noteE4  = 3033704;
	const int noteF4  = 2863442;
	const int noteF4s = 2702775;
	const int noteG4  = 2551020;
	const int noteG4s = 2407897;
	const int noteA4  = 2272727;
	const int noteB4  = 2024783;

	const int noteC5  = 1911132;
	const int noteC5s = 1803849;
	const int noteD5  = 1702620;
	const int noteD5s = 1607071;
	const int noteE5  = 1517451;
	const int noteF5  = 1431721;
	const int noteF5s = 1351369;
	const int noteG5  = 1275526;
	const int noteG5s = 1203934;
	const int noteA5  = 1136363;
	const int noteA5s = 1072581;
	const int noteB5  = 1012381;

	const int noteC6  = 955566;
	const int noteC6s = 901957;
	const int noteD6  = 821581;
	const int noteD6s = 803535;
	const int noteE6  = 758437;
	const int noteF6  = 715871;
	const int noteF6s = 675675;
	const int noteG6  = 637755;
	const int noteG6s = 601974;
	const int noteA6  = 568181;
	const int noteA6s = 536279;
	const int noteB6  = 506201;
	const int notepause = 0;

	void playNote(FILE*, FILE*, int, int);


int main() {
	
	// Creats an array of note frequency. The song used here is Canon by Pachelbel.
	int tune[] = {noteD5, noteG5, noteE5, noteA5, notepause,
noteA5, noteF5s, noteG5, noteA5, noteF5s, noteG5, noteA5, noteA4, noteB4, noteC5s, noteD5, noteE5, noteF5s, noteG5, 
noteF5s, noteD5, noteE5, noteF5s, noteF5s, noteG5, noteA5, noteB5, noteA5, noteG5, noteA5, noteF5s, noteG5, noteA5, 
noteG5, noteB5, noteA5, noteG5, noteF5s, noteE5, noteF5s, noteE5, noteD5, noteE5, noteF5s, noteG5, noteA5, noteB5,
noteG5, noteB5, noteA5, noteB5, noteC6s, noteD6, noteA5, noteB5, noteC6s, noteD6, noteE6, noteF6s, noteG6, noteA6,
noteF6s, noteD6, noteE6, noteF6s ,noteE6, noteD6, noteE6, noteC6s ,noteD6, noteE6, noteF6s, noteE6, noteD6, noteC6s,
noteD6, noteB5, noteC6s, noteD6, noteD5, noteE5,noteF5s, noteG5, noteF5s, noteE5, noteF5s, noteD6, noteC6s, noteD6,
noteB5, noteD6, noteC6s, noteB5, noteA5, noteG5, noteA5, noteG5, noteF5s, noteG5,noteA5, noteB5, noteC6s, noteD6,
noteB5, noteD6, noteC6s, noteD6, noteC6s, noteB5, noteC6s, noteD6, noteE6, noteD6, noteC6s, noteD6, noteB5, noteC6s,
noteD6, notepause, noteC6s, notepause,
noteB5, notepause, noteD6, notepause
} ;


	// Ceates an array of length of note. 4 for a half note. 2 for a fourth note. 1 for a eighth note.
	int duration[] = {4, 4, 4, 4, 1,
	2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	4, 4, 4, 4,
	4, 4, 4, 4 
	};

	// Creates the File pointers to create the gpio file system,
	// Set the direction of the GPIO, and change the data stored there.
	FILE *sys, *sys2, *dir60, *dir48, *dir49, *val60, *val48, *val49, *noteduty, *noteperiod, *run;

	// Specifies the file that the pointer will be used for (w = //write)
	sys = fopen("/sys/class/gpio/export", "w");
   	sys2 = fopen("/sys/devices/bone_capemgr.9/slots", "w");


	// Sets the file position of the sys to the beginning of file
	// Sets the file position of the sys2 to the end of file
	fseek(sys, 0, SEEK_SET);
	fseek(sys2, 0, SEEK_END);
	
	// Writes the value corresponding to the GPIO digital pins used
	fprintf(sys, "%d", GPIO_PIN_60);
	fflush(sys);
	fprintf(sys, "%d", GPIO_PIN_48);
	fflush(sys);
	fprintf(sys, "%d", GPIO_PIN_49);
	fflush(sys);

	// Writes the PWM used and the corresponding PWM output pin
	fprintf(sys2, "am33xx_pwm");
	fflush(sys2);
	fprintf(sys2, "bone_pwm_P8_19");
	fflush(sys2);

	// Sets the gpio60 to output
	dir60 = fopen("/sys/class/gpio/gpio60/direction", "w");
	fseek(dir60, 0, SEEK_SET);
	fprintf(dir60, "%s", "out");
	fflush(dir60);

	// Sets the gpio48 to output
	dir48 = fopen("/sys/class/gpio/gpio48/direction", "w");
	fseek(dir48, 0, SEEK_SET);
	fprintf(dir48, "%s", "out");
	fflush(dir48);

	// Sets the gpio49 to output
	dir49 = fopen("/sys/class/gpio/gpio49/direction", "w");
	fseek(dir49, 0, SEEK_SET);
	fprintf(dir49, "%s", "out");
	fflush(dir49);

	// Opens the file that controls if the pin is high or low
	val60 = fopen("/sys/class/gpio/gpio60/value", "w");
	fseek(val60, 0, SEEK_SET);
	val48 = fopen("/sys/class/gpio/gpio48/value", "w");
	fseek(val48, 0, SEEK_SET);
	val49 = fopen("/sys/class/gpio/gpio49/value", "w");
	fseek(val49, 0, SEEK_SET);

	// Sets the pointers to the appropriate duty, period and run files
	noteduty = fopen("/sys/devices/ocp.3/pwm_test_P8_19.15/duty", "w");
	noteperiod = fopen("/sys/devices/ocp.3/pwm_test_P8_19.15/period", "w");

	// This while loop blinks the LED from 0 to 7 and then start again from 0 while music plays.
    //Each music note lasts for one blink of LED.
	while(1) {
		int i = 0;
		int count = 0;
		for(i=0;i < sizeof(tune)/sizeof(tune[0]); i++) {
			playNote(noteperiod, noteduty, tune[i], tune[i]/3);
			int length = duration[i];
			if(count > 7) {
				count = 0;			
			}
			int digit1 = 0, digit2 = 0, digit3 = 0;
			digit3 = (count & 4);
			digit2 = (count & 2);
			digit1 = (count & 1);
			
			fprintf(val60, "%d", digit3);
			fflush(val60);
			fprintf(val48, "%d", digit2);
			fflush(val48);
			fprintf(val49, "%d", digit1);
			fflush(val49);	
				
			count++;
			usleep(250000*length);
		}	
	}

   // Closes all files and returns 0.
	fclose(sys);
	fclose(sys2);
	fclose(dir60);
	fclose(dir48);
	fclose(dir49);
	fclose(val60);
	fclose(val48);
	fclose(val49);
	fclose(noteduty);
	fclose(noteperiod);
	return 0;
}

 	// changes the period and dutycycle of each note
	void playNote(FILE *noteperiod, FILE *noteduty, int period, int duty) {
		fprintf(noteperiod, "%d", period);
		fflush(noteperiod);
		fprintf(noteduty, "%d", duty);
		fflush(noteduty);		
	}


	 





