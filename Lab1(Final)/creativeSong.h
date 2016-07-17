/* Date: 07/10/2016
 * Authors: Xinxin Zhang, Jiaqi Zhang, Heng Zhou
 *
 * music.h
 *
 * Method headers for creativeSong.c
 */

#include <stdio.h>

	// Read in the period and duty cycle for each note and play it.
	void playNote(FILE *noteperiod, FILE *noteduty, int period, int duty);
