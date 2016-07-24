/*
 
 Summer 2016
 EE474
 Lab2
 Authors: Xinxin Zhang, Jiaqi Zhang, Heng Zhou
 Program Introduction: This code will do the initialization on LCD display. Code should be run on Beaglebone connected with LCD pins.
 
 */


//Header File
#include "initialization.h"

//Function headers.
static FILE *value[ALL_PINS];
static void enableVal();
static void write_com(unsigned char com);
static void write_data(unsigned char data);
static void fcnSet(unsigned char num);
static void displayOff();
static void displayOn();
static void clearDisplay();
static void initialization();

unsigned char table1[]="Hello";
unsigned char table2[]="World";
unsigned char num;


int main() {
    // Creates the File pointers to create the gpio file system
    FILE *sys;
    FILE *dir[ALL_PINS];
    char path1[50];
    char path2[50];
    
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
    // Specifies the vaule file is used to write in
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
    
    initialization();
    
    write_com(0x80); //Display cursor - top left corner.
    write_com(0x0F); //Blinks cursor.
}

// Sets value for DB0 - DB7 using 8 digits from the input.
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

// Enables the values to be pushed to the board.
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

// Sends in data for diplaying on LCD display.
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

// display off
void displayOff() {
    write_com((unsigned char) 0x08);
}

// display on
void displayOn() {
    write_com((unsigned char) 0x0F);
}

// clear display
void clearDisplay() {
    write_com((unsigned char) 0x01);
}

// Initializes the board.
// A blinking cursor will be displayed on the left bottom corner of the lCD.
void initialization() {
    fprintf(value[0], "%d", 0); // Function Set
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

