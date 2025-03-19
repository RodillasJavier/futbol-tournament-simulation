#ifndef TEAM_H
#define TEAM_H

/**
 * @author Javier A. Rodillas
 * @details Header file for the Player class file
 */

#include "player.h"

/**
 * Team @struct to represent a club football team
 */
typedef struct
{
    char name[50];          // Name of the club
    char city[50];          // City the club is based in
    char coach[50];         // Name of the head coach
    char homeStadium[50];   // Name of the home stadium

    Player** players;       // Array of pointers to Player structs
    int numPlayers;         // Number of players currently on the team
    int maxPlayers;         // Maximum number of players a team can have

    int* schedule;          // Array of indices (of teams) in the order they play
    int scheduleLength;     // Length of schedule (alternatively, number of games)

    int wins;               // Games won
    int losses;             // Games lost
    int draws;              // Games drawn

    int goalsScored;        // Total scored on opposing teams
    int goalsConceded;      // Total allowed by opposing teams
    int goalDifferential;   // Result of (goalsScored - goalsConceded)

    int points;             // Points (3 for win, 1 for draw)
} Team;

/**
 * Create a new team
 * 
 * @param name for the new club
 * @param city that the club is based in
 * @param coach name
 * @param homeStadium name the club plays in
 * 
 * @return pointer to the new Team we just created
 */
Team* createTeam(char name[50], char city[50], char coach[50], char homeStadium[50]);

/**
 * Destroy & free all memory allocated for our team
 * 
 * @param pointer to the team that we want to 'destroy'
 */
void destroyTeam(Team* team);

// add player
// remove player
// updateRecord()
// calculateGoalDifferential
// get player by number
// get team rating
// print team

#endif  // TEAM_H