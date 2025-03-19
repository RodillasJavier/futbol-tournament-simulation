#include "Player.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @author Javier A. Rodillas
 * @details Class implementation of our Player class/struct
 */

/**
 * Create a new player with all of their attributes
 */
Player* createPlayer(int number, const char* position, const char* name, int rating)
{
    // Allocate memory for a player
    Player* player = (Player*)malloc(sizeof(Player));

    // Null checking player
    if (player == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for player\n");
    }

    // Initialize player attributes
    player -> number = number;      // Number

    // Position
    strncpy(player -> position, position, sizeof(player -> position) - 1);
    player -> position[sizeof(player -> position) - 1] = '\0';  // Ensure null termination

    // Name
    strncpy(player -> name, name, sizeof(player -> name) - 1);
    player -> name[sizeof(player -> position) - 1] = '\0';  // Ensure null termination

    player -> rating = rating;      // Rating
    player -> goals = 0;            // Goals
    player -> assists = 0;          // Assists
    player -> injuryStatus = false; // Injury Status, default to false upon creation

    return player;
}

/**
 * Free up memory allocated for a player
 */
void destroyPlayer(Player* player)
{
    if (player != NULL) {
        free(player);
    }
}

/**
 * Record a scored goal for a player
 */
void scoreGoal(Player* player)
{
    if (player != NULL)
    {
        player -> goals++;
    }
    
}

/**
 * Record an assist for a player
 */
void assist(Player* player)
{
    if (player != NULL)
    {
        player -> assists++;
    }
}

/**
 * Update the injury status for a player who got injured
 */
void updateInjuryStatus(Player* player, bool status)
{
    if (player != NULL)
    {
        player -> injuryStatus = status;
    }
}

/**
 * Print players information to stdout
 */
void printPlayer(const Player* player)
{
    if (player != NULL)
    {
        char injuryReport[8];

        if (player -> injuryStatus == true)
        {
            strncpy(injuryReport, "Injured", 7);
            injuryReport[7] = '\0';
        }
        else    // player -> injuryStatus == false
        {
            strncpy(injuryReport, "Healthy", 7);
            injuryReport[7] = '\0';
        }

        fprintf(
            stdout, 
            "%s, number %d: \n\t%d overall %s \n\t%d Goals, %d Assists \n\tInjury Status: %s\n", 
            player -> name, player -> number, player -> rating, 
            player -> position, player -> goals, player -> assists, 
            injuryReport
        );
    }
}

/**
 * Create a deep copy of a player from a source player to a 'destination' player pointer
 */
Player* copyPlayer(const Player* src, Player* dest)
{
    // Null checking player
    if (src == NULL)
    {
        fprintf(stderr, "Error: null pointer to a 'source' player when trying to make a copy.\n");
    }
    
    // Null checking player
    if (dest == NULL)
    {
        fprintf(stderr, "Error: null pointer to a 'destination' player when trying to make a copy.\n");
    }

    // Copying Attributes
    dest -> number = src -> number;             // Number

    // Position
    strncpy(dest -> position, src -> position, sizeof(src -> position) - 1);
    dest -> position[sizeof(src -> position) - 1] = '\0';

    // Name
    strncpy(dest -> name, src -> name, sizeof(src -> name) - 1);
    dest -> name[sizeof(src -> name) - 1] = '\0';

    dest -> rating = src -> rating;             // Rating
    dest -> goals = src -> goals;               // Goals
    dest -> assists = src -> assists;           // Assists
    dest -> injuryStatus = src -> injuryStatus; // Injury Status, default to false upon creation
}