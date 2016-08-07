LKMs and Shift Registers  
Contributors: Xinxin Zhang, Jiaqi Zhang, Heng Zhou

## Synopis  
The purpose of this lab is to be familiar with the shift register and kernal module.  
In part 1, we eliminate GPIOs usage on BBB by using the shift register.    
In part 2, we build a single kernel-mode driver which accesses fewer GPIOs to operate the physical hardware. 

## Hardware Setup  
Registers(R), LCD(L), GPIO_PINS(G), LED(LED)
(R)QA---(L)DB0;  
(R)QB---(L)DB1;  
(R)QC---(L)DB2;  
(R)QD---(L)DB3;  
(R)QE---(L)DB4;  
(R)QF---(L)DB5;  
(R)QG---(L)DB6;  
(R)QH---(L)DB7;  
(R)VCC---(G)SYS_5V;  
(R)SER---(G)GPIO_PIN_67;  
(R)OE---(L)GND---(G)DGND;  
(R)SRCLK---(G)GPIO_PIN_68;    
(R)RCLK---(G)GPIO_PIN_69;  
(R)MR---(G)SYS_5V;  
(R)GND---(G)DGND;  
(L)VSS---(G)DGND;  
(L)VDD/VCC---(G)SYS_5V;  
(L)RS---(G)GPIO_PIN_46;  
(L)RW---(G)DGND;  
(L)E---(G)GPIO_PIN_61; 
(LED)RED---(G)GPIO_PIN_27;  
(LED)GREEN---(G)GPIO_PIN_45;  


## Code Excecution
1. SSH protocol is used to operate the Beaglebone
2. A shift register is used to transfer data from the Beaglebone to the LCD screens for conserving GPIO pins.
2. Generate a .ko file on station use command: Make.
3. Transfer the .ko file and test_game file to the BBB.
4. On BBB insert kernel module with command: insmod game_driver.
5. Create the dev file with command: mknod /dev/game_driver c 239 0.
6. Operate the game: gcc test_game.c and ./a.out.


## Files  
#### game_driver.c
This file acts as a LCD and register driver. It mainly writes the data from user space in dev with properly communicating with LCD and shift register. 

#### game_driver.h  
This is the header file of the game_drive.c, which contains the included libraries and some defined variables, functions and operations.

#### test_game.c  
This file is written in user space C and used the kernel module above to communicate with periferal devices while executing the Hangman game. Users are allowed to guess a word within 5 chances. Two leds and a audio speaker will be operated automatically during the game to show the status of the game. 

#### test_game.h  
This is the header file of the test_game.c, which which contains the included libraries and some defined variables, functions.

#### Makefile 
This is used to generate the .ko file that will be inserted as a kernel module into the BBB. 


