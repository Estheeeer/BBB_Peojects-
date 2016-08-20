# Beagle Bone Black Final Project:  

Contributors: Xinxin Zhang, Jiaqi Zhang, Heng Zhou

## Synopis  
We eliminate GPIOs usage on BBB to transfer data to LCD display by using the shift register. We connect driver motor with the BBB to move the car. We also connect a bluetooth with BBB to remotly control the movement of the car. 

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
(R)SER---(G)GPIO_PIN_26;  
(R)OE---(L)GND---(G)DGND;  
(R)SRCLK---(G)GPIO_PIN_27;    
(R)RCLK---(G)GPIO_PIN_65;  
(R)MR---(G)SYS_5V;  
(R)GND---(G)DGND; 

(L)VSS---(G)DGND;  
(L)VDD/VCC---(G)SYS_5V;  
(L)RS---(G)GPIO_PIN_46;  
(L)RW---(G)DGND;  
(L)E---(G)GPIO_PIN_61; 

(S)WHITE—(G)AIN0;  
(S)BLACK---(G)DGND;  
(S)RED---(G)SYS_5V;  

(MD)PWMA---(G)PWM1A;  
(MD)AIN2---(G)GPIO_PIN_67;  
(MD)AIN1---(G)GPIO_PIN_66;  
(MD)STBY---(G)SYS_5V;  
(MD)BIN1---(G)GPIO_PIN_68;  
(MD)BIN2---(G)GPIO_PIN_69;  
(MD)PWMB---(G)PWM1B;  
(MD)GND---(G)GND;  

(B)VCC---(G)SYS_5V;  
(B)GND---(G)DGND;  
(B)TX01---(G)UART1_RXD;
(B)RX01---(G)UART1_TXD


## Code Excecution

1. SSH protocol is used to operate the Beaglebone
2. Allow a bluetooth to transfer the data from a android device to the BBB: python bluetooth.py. 
3. Run the game function by calling a python file: python install.py.
4. Two service file is inserted into BBB to autostart the operation on BBB by excecuting two python code files when rebooting the BBB:
    cd /lib/systemd/system/
    systemctl --system daemon-reload
    systemctl enable p1.service
    systemctl enable p1.service
    systemctl start p2.service
    systemctl start p2.service


## Files  
#### bluetooth.py
This file is used to connect the android device with the BBB to allow user remotly operate the car which holds up the BBB(mainly control the movement of the car).

#### install.py  
This file is used to run the game function without manually type in the terminal.

#### driver.c  
This file is written in user space C and communicate with periferal devices while executing it.  

#### driver.h  
This is the header file of the driver.c, which which contains the used libraries and some defined variables and functions.

#### p1.service 
This file is inserted in BBB, which is used to automatically run bluetooth.py. 

#### p2.service
This file is inserted in BBB, which is used to automatically run install.py. 



