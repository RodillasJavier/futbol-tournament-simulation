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
Player* createPlayer(int number, const char* position, const char* name, int rating) {
    // Allocate memory for a player
    Player* player = (Player*)malloc(sizeof(Player));

    // Null checking player
    if (player == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for player\n");
    }

    // Initialize player attributes
    player -> number = number;  // Number

    // Position
    strncpy(player -> position, position, sizeof(player -> position) - 1);
    player -> position[sizeof(player -> position) - 1] = '\0';  // Ensure null termination

    // Name
    strncpy(player -> name, name, sizeof(player -> name) - 1);
    player -> name[sizeof(player -> position) - 1] = '\0';  // Ensure null termination

    player -> rating = rating;      // Rating
    player -> goals = 0;            // Goals
    player -> assists = 0;          // Assists
    player -> injuryStatus = false; // Injury Status

    return player;
}