#ifndef TEAM_H
#define TEAM_H

/**
 * @author Javier A. Rodillas
 * @details Header file for the team class file
 */

#include "player.h"
#include <stdbool.h>

/**
 * Team @struct to represent a club football team
 */
typedef struct
{
    //  Club details
    char name[50];          // Name of the club
    char city[50];          // City the club is based in
    char coach[50];         // Name of the head coach
    char homeStadium[50];   // Name of the home stadium

    // Roster
    Player** players;       // Array of pointers to Player structs
    int numPlayers;         // Number of players currently on the team
    int maxPlayers;         // Maximum number of players a team can have

    // Schedule
    // @attention: NO METHOD IN THIS CLASS TO CHANGE THIS
    int* schedule;          // Array of indices (of teams) in the order they play
    int scheduleLength;     // Length of schedule (alternatively, number of games)

    // Rating
    float rating;           // Overall rating (average) of the team

    // Record
    int wins;               // Games won
    int losses;             // Games lost
    int draws;              // Games drawn

    // Goals
    int goalsScored;        // Total scored on opposing teams
    int goalsConceded;      // Total allowed by opposing teams
    int goalDifferential;   // Result of (goalsScored - goalsConceded)

    // Points
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
Team* createTeam(const char* name, const char* city, const char* coach, const char* homeStadium);

/**
 * Destroy & free all memory allocated for our team
 * 
 * @param pointer to the team that we want to 'destroy'
 */
void destroyTeam(Team* team);

/**
 * Add a player to a team
 * 
 * @param Team to add a player to
 * @param Player to add to our team
 * 
 * @return bool (true => added to team, false otherwise)
 */
bool addPlayer(Team* team, const Player* player);

/**
 * Remove a specific player from a team, searching for them by number
 * 
 * @param Team to remove a player from
 * @param int jersery number of the player we want to remove from the team
 * 
 * @return bool (true => removed from team, false otherwise)
 */
bool removePlayerByNumber(Team* team, int number);

/**
 * Remove a specific player from a team, searching for them by name
 * 
 * @param Team to remove a player from
 * @param name of the player that we are looking to remove from the team
 * 
 * @return bool (true => removed from team, false otherwise)
 */
bool removePlayerByName(Team* team, char* name);

/**
 * Get a player by their jersey number
 * 
 * @param Team that we are searching in
 * @param int jersey number that we are looking for
 * 
 * @return  pointer to the player that we have found. CAUTION changing this 
 *          player affects the original player that it points at
 */
Player* getPlayerByNumber(const Team* team, int number);

/**
 * Calculate the teams overall rating (based on team average)
 * 
 * @param Team that we are calculating the rating of
 */
float calculateTeamRating(Team* team);

/**
 * Update the record of a team
 * 
 * @param Team that we want to update the record of
 * @param bool win
 * @param bool loss
 * @param bool draw
 */
void updateRecord(Team* team, bool win, bool loss, bool draw);

/**
 * Update the points of a team
 * 
 * @param Team that we want to update the points of
 */
void updatePoints(Team* team);

/**
 * Update the goal differential, goals scored & goals conceded
 * 
 * @param Team that we are calculating the differential of
 * @param int goalsScored
 * @param int goalsConceded
 */
void updateGoals(Team* team, int goalsScored, int goalsConceded);

/**
 * Print the information of a Team
 * 
 * @param Team that we are interested in\
 * @param bool showPlayers (true => print roster, false => exclude roster)
 */
void printTeam(const Team* team, bool showPlayers);

#endif  // TEAM_H