#ifndef PLAYER_H
#define PLAYER_H

/**
 * @author Javier A. Rodillas
 * @details Header file for the Player class file
 */

#include <stdbool.h>

/**
 * Player @struct representing a football player
 */
typedef struct Player
{
    int number;         // Number on the back of their jersey
    char position[4];   // Position can be represented by atmost a 3 character string (extra byte for null terminator)
    char name[50];      // Last name of the player (i.e. 'Lionel Messi' as 'Messi')
    int rating;         // Player rating (1 - 100)
    int goals;          // Goals scored
    int assists;        // Assists provided
    bool injuryStatus;  // True if injured, False otherwise
};

/**
 * Create a new player with all of their attributes
 * 
 * 
 */
Player* createPlayer(int number, const char* position, const char* name, int rating);

#endif /* PLAYER_H */