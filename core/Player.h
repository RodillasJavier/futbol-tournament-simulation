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

typedef struct
{
    int number;         // Number on the back of their jersey
    char position[4];   // Position can be represented by atmost a 3 character string (extra byte for null terminator)
    char name[50];      // Last name of the player (i.e. 'Lionel Messi' as 'Messi')
    int rating;         // Player rating (1 - 100)
    int goals;          // Goals scored
    int assists;        // Assists provided
    bool injuryStatus;  // True if injured, False otherwise
} Player;

/**
 * Create a new player with all of their attributes
 * 
 * @param number on the back of the player's jersey
 * @param position -> Pointer to the position of the player on the roster
 * @param name -> Pointer to the last name of the player
 * @param rating of the player (1 - 100)
 */
Player* createPlayer(int number, const char* position, const char* name, int rating);

/**
 * Free up memory allocated for a player
 * 
 * @param pointer to the player we are freeing from memory
 */
void destroyPlayer(Player* player);

/**
 * Record a scored goal for a player
 * 
 * @param pointer to the player who scored a goal
 */
void scoreGoal(Player* player);

/**
 * Record an assist for a player
 * 
 * @param pointer to the player who recorded an assist
 */
void assist(Player* player);

/**
 * Update the injury status for a player who got injured
 * 
 * @param pointer to the player who got injured
 * @param bool representing the new injury status for the player (True for injured, False otherwise)
 */
void updateInjuryStatus(Player* player, bool status);

/**
 * Print players information to stdout
 * 
 * @param pointer to the player we are interested in
 */
void printPlayer(const Player* player);

/**
 * Create a deep copy of a player from a source player to a 'destination' player pointer
 * 
 * @param pointer to our source player that we are copying
 * @param pointer to our 'destination' player
 */
Player* copyPlayer(const Player* src, const Player* dest);

#endif /* PLAYER_H */