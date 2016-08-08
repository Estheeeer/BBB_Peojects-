/* Summer 2016
 * Xinxin Zhang, Jiaqi Zhang, Heng Zhou
 * game_driver.c: generates the game_drive.ko kernel object file. This kernel enables the LCD board to connect 
 * with Beagle Bone Black through GPIOs and a shift register.
 */
#include "game_driver.h"
#include <linux/gpio.h>
#include <linux/delay.h>

/********************* FILE OPERATION FUNCTIONS ***************/

// runs on startup
// intializes module space and declares major number.
// assigns device structure data.
 static int __init driver_entry(void) {
	// REGISTERIONG OUR DEVICE WITH THE SYSTEM
	// (1) ALLOCATE DINAMICALLY TO ASSIGN OUR DEVICE
 	int ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
 	if (ret < 0) {
 		printk(KERN_ALERT "new_char: Failed to allocate a major number\n");
 		return ret;
 	}
 	printk(KERN_INFO "new_char: major number is %d\n", MAJOR(dev_num));
 	printk(KERN_INFO "Use mknod /dev/%s c %d 0 for device file\n", DEVICE_NAME, MAJOR(dev_num));

	// (2) CREATE CDEV STRUCTURE, INITIALIZING CDEV
 	mcdev = cdev_alloc();
 	mcdev->ops = &fops;
 	mcdev->owner = THIS_MODULE;

	// After creating cdev, add it to kernel
 	ret = cdev_add(mcdev, dev_num, 1);
 	if (ret < 0) {
 		printk(KERN_ALERT "new_char: unable to add cdev to kernerl\n");
 		return ret;
 	}
	// Initialize SEMAPHORE
 	sema_init(&virtual_device.sem, 1);

 	msleep(10);

 	return 0;
 }

// called up exit.
// unregisters the device and all associated gpios with it.
 static void __exit driver_exit(void) {
 	cdev_del(mcdev);
 	unregister_chrdev_region(dev_num, 1);
	//printk(KERN_ALERT "new_char: successfully unloaded\n");
 }

// Called on device file open
//	inode reference to file on disk, struct file represents an abstract
// checks to see if file is already open (semaphore is in use)
// prints error message if device is busy.
 int device_open(struct inode *inode, struct file* filp) {
 	if (down_interruptible(&virtual_device.sem) != 0) {
 		printk(KERN_ALERT "new_char: could not lock device during open\n");
 		return -1;
 	}
	//printk(KERN_INFO "new_char: device opened\n");
	//return 0;


	// Request access to the needed GPIO pins
 	gpio_request(SER, "Data");
 	gpio_request(R_CLK, "R_Clock");
 	gpio_request(SR_CLK, "SR_Clock");
 	gpio_request(RS, "RS");
    gpio_request(E, "E");
     
	// Set the direction of GPIO pins as output
 	gpio_direction_output(SER, 0);
 	gpio_direction_output(R_CLK, 0);
 	gpio_direction_output(SR_CLK, 0);
 	gpio_direction_output(RS, 0);
 	gpio_direction_output(E, 0);
 	

	// Initializes the LCD
 	initialization();
 	return 0;
 }

// Called upon close
// closes device and returns access to semaphore.
 int device_close(struct inode* inode, struct  file *filp) {
 	up(&virtual_device.sem);

 	gpio_free(SER);
 	gpio_free(R_CLK);
 	gpio_free(SR_CLK);
 	gpio_free(RS);

 	gpio_free(E);

 	printk(KERN_INFO "new_char, closing device\n");
 	return 0;	
 }

// Called when user wants to get info from device file.
 ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset) {
 	printk(KERN_INFO "new_char: Reading from device...\n");
 	return copy_to_user(bufStoreData, virtual_device.data, bufCount);
 }

// Called when user wants to send info to device.
 ssize_t device_write(struct file* filp, const char* bufSource, size_t bufCount, loff_t* curOffset) {
 	printk(KERN_INFO "new_char: writing to device...\n");

// Sends the data to LCD display
 	gameInfo(bufSource);
 	return copy_from_user(virtual_device.data, bufSource, bufCount);
 }

//Set up the LCD board for display.
void initialization() {
 	gpio_set_value(RS, 0);
 	gpio_set_value(E, 0);
 	msleep(15); 

 	fcnSet((unsigned char) 0x3f);
 	msleep(50);
 	enableVal();

 	msleep(5);
 	fcnSet((unsigned char) 0x3f);
 	enableVal();

 	msleep(1);
 	fcnSet((unsigned char) 0x3f);
 	msleep(1);
 	enableVal();

 	fcnSet((unsigned char) 0x3f);
 	msleep(1);
 	enableVal();

 	displayOff();

    fcnSet((unsigned char) 0x06); //Entry Mode Set
    msleep(1);
    enableVal();
    
    displayOn();
    
    clearDisplay();
    msleep(15);
}

void enableVal() {
	gpio_set_value(E, 1);
	msleep(1);
	gpio_set_value(E, 0);
	msleep(1);
}

// Controls the behavior and position of the cursor on LCD display
void write_com(unsigned char com) {

    gpio_set_value(RS, 0); //RS: 0
    msleep(1);

    gpio_set_value(E, 0); //E: 0
    msleep(1);
    
    fcnSet(com);
    msleep(1);
    
    gpio_set_value(E, 1); //E: 1
    msleep(1);

    gpio_set_value(E, 0); //E: 0
    msleep(1);
}

// Sends in data for diplaying on LCD display.
void write_data(unsigned char data) {
    gpio_set_value(RS, 1); //RS: 1
    msleep(1);

    gpio_set_value(E, 0); //E: 0
    msleep(1);

    fcnSet(data);
    msleep(1);
    
    gpio_set_value(E, 1); //E: 1
    msleep(1);
    
    gpio_set_value(E, 0); //E: 0
    msleep(1);
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

//RCLK is held low when data is being written to the shift register.
//When RCLK is high, the values stored in the shift register are latched to the storage register and
//outputted to pins Q0-7.
void switchRCLK() {
	gpio_set_value(R_CLK, 0);
	gpio_set_value(R_CLK, 1);
	gpio_set_value(R_CLK, 0);

}

//When SRCLK goes from Low to High the value is stored into the shift register and the existing values
//shifted to make room for new bit.
void switchSRCLK() {
	gpio_set_value(SR_CLK, 0);
	gpio_set_value(SR_CLK, 1);
	gpio_set_value(SR_CLK, 0);
}

//Reads in characters and pushes bit by bit to shift register's data bit.
void fcnSet(unsigned char num) {
	int bitArray[8];
	int i;

	for (i = 0; i < 8; i++) {
		bitArray[i] = num % 2 ;
		num  = num /2;	
	}

	for (i = 0; i < 8; i++) {
		gpio_set_value(SER, bitArray[7-i]);
		switchSRCLK();
	}

	switchRCLK();
	msleep(100);
}

//Reads in string, decides which line on LCD to use and sends data to write_data fucntion char by char.
void gameInfo(const char str[]) {
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
	msleep(999);
}



MODULE_LICENSE("GPL"); // module license: required to use some functionalities.
module_init(driver_entry); // declares which function runs on init.
module_exit(driver_exit);  // declares which function runs on exit.
