/**
 * @author Javier A. Rodillas
 * @details Class implementation of the match simulation module. 
 */



/* INCLUDE STATEMENTS */

#include "match_simulation.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>



/* HELPER FUNCTIONS */

//  Initialize random seed if not yet done
static bool randomInitialized = false;
void initRandom()
{
    if (randomInitialized == false)
    {
        srand((unsigned int)time(NULL));
        randomInitialized = true;
    }
    
}

// Generate a random number (double) between 0 and 1
double randomProbability()
{
    // Ensure random seed is initialized
    initRandom();

    // Generate a number b/w (0 & RAND_MAX) and div by RAND_MAX ~> Number b/w (0 & 1)
    return (double)rand() / RAND_MAX;
}

// Generate a random number (int) in the range (min, max) inclusive
int randomInt(int min, int max)
{
    // Ensure random seed is initialized
    initRandom();

    // Generate a number between min and max (inclusive)
    return (rand() % (max - min + 1)) + min;

    // implementation from: https://www.quora.com/How-do-I-get-a-random-number-from-1-to-100-in-the-C-language
}



/* FUNCTIONS */
