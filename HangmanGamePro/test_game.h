/* Summer 2016
* Xinxin Zhang, Jiaqi Zhang, Heng Zhou
* test_game.h: header file for test_game.c.
*/
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEW_CHAR_DIR "/dev/game_driver"
#define ALL_PINS 3
#define BLUE 47
#define RED 27
#define GREEN 45 

static FILE *value[ALL_PINS];
static int win(int letterFound[]);
static int researchLetter(char letter, char secretWord[], int letterFound[]);
static char readCharacter();
static void red(int i);
static void green(int i);
static void playNote(FILE*, FILE*, int, int);
