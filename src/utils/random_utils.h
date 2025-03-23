#ifndef RANDOM_H
#define RANDOM_H

/**
 * @author Javier A. Rodillas
 * @details Header file for the random number generation utility module. 
 */



/* INCLUDE STATEMENTS */
#include <stdbool.h>



/* FUNCTION PROTOTYPES */

/**
 * Initialize random seed if not yet done
 */
void initRandom();

/**
 * Generate a random double
 * 
 * @return a double in the range of 0.0 and 1.0
 */
double randomProbability();

/**
 * Generate a random integer
 * 
 * @param min lower bound for our random number
 * @param max upper bound for our random number
 * 
 * @return an int within the range of (min, max) inclusive
 */
int randomInt(int min, int max);

#endif