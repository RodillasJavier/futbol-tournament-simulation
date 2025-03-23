/**
 * @author  Javier A. Rodillas
 * @details Class implementation of the random number generation utility class. 
 */

/* INCLUDE STATEMENTS */

#include "random_utils.h"
#include <time.h>
#include <stdlib.h>



/* FUNCTIONS */

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
}
