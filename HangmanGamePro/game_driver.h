/* Summer 2016
 * Xinxin Zhang, Jiaqi Zhang, Heng Zhou
 * game_driver.h: libraries, global variables and functions headers used in game_drive.h.
 *
 */
#ifndef NEW_CHAR_H_
#define NEW_CHAR_H_
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <linux/unistd.h>

#define DEVICE_NAME "game_driver"


#define SER 67      // series inputs
#define R_CLK 68    // clock input of the memory 
#define SR_CLK 69   // clock input of the shift register
#define RS 46       // change function under writing mode
#define E 61



/* data structures */
// contains data about the device.
// data : buffer for character data stored.
// sem  : semaphore.
 struct fake_device {
 	char data[100];
 	struct semaphore sem;
 } virtual_device;

/* global variables */
// stores info about this char device.
 static struct cdev* mcdev;
// holds major and minor number granted by the kernel
 static dev_t dev_num;

/* function prototypes */
// file operations
 static int __init driver_entry(void);
 static void __exit driver_exit(void);
 static int  device_open(struct inode*, struct file*);
 static int device_close(struct inode*, struct file *);
 static ssize_t device_read(struct file*, char*, size_t, loff_t*);
 static ssize_t device_write(struct file*, const char*, size_t, loff_t*);
 static void enableVal(void);
 static void write_com(unsigned char);
 static void write_data(unsigned char);
 static void fcnSet(unsigned char);
 static void displayOff(void);
 static void displayOn(void);
 static void clearDisplay(void);
 static void initialization(void);
 static void switchRCLK(void);
 static void switchSRCLK(void);
 static void gameInfo(const char[]); 

/* operations usable by this file. */
 static struct file_operations fops = {
 	.owner = THIS_MODULE,
 	.read = device_read,
 	.write = device_write,
 	.open = device_open,
 	.release = device_close,
 };
#endif
