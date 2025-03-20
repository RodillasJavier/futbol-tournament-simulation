/**
 * @author Javier A. Rodillas
 * @details Implementation for the 'Team' class
 */

#include "team.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* METHODS */
// Create a new team
Team* createTeam(const char* name, const char* city, const char* coach, const char* homeStadium)
{
    // Allocate memory for a Team
    Team* team = (Team*)malloc(sizeof(Team));

    // Null checking team
    if (team == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for a team\n");
    }

    // Initialize player attributes:

    // Name of the club
    strncpy(team -> name, name, sizeof(team -> name) - 1);
    team -> name[sizeof(team -> name) - 1] = '\0';  // Ensure null termination

    // City the club is based in
    strncpy(team -> city, city, sizeof(team -> city) - 1);
    team -> city[sizeof(team -> city) - 1] = '\0';  // Ensure null termination

    // Name of the head coach
    strncpy(team -> coach, coach, sizeof(team -> coach) - 1);
    team -> coach[sizeof(team -> coach) - 1] = '\0';    // Ensure null termination

    // Name of the home stadium
    strncpy(team -> homeStadium, homeStadium, sizeof(team -> homeStadium) - 1);
    team -> homeStadium[sizeof(team -> homeStadium) - 1] = '\0';    // Ensure null termination

    // Team Roster
    team -> maxPlayers = 25;    // Default max roster size of 25
    team -> numPlayers = 0;
    team -> players = (Player**)malloc(sizeof(Player*) * team -> maxPlayers);

    // Rest of the fields & their default values
    team -> rating = 0.0;
    team -> schedule = NULL;
    team -> scheduleLength = 0;
    team -> wins = 0;
    team -> losses = 0;
    team -> draws = 0;
    team -> goalsScored = 0;
    team -> goalsConceded = 0;
    team -> goalDifferential = 0;
    team -> points = 0;

    return team;
}

// Destroy & free all memory allocated for our team
void destroyTeam(Team* team);

// Add a player to a team
bool addPlayer(Team* team, const Player* player);

// Remove a specific player from a team
bool removePlayer(Team* team, int number);

// Get a player by their jersey number
Player* getPlayerByNumber(const Team* team, int number);


// Update the record of a team
void updateRecord(Team* team, bool win, bool loss, bool draw);

// Calculate the goal differential based on the number of goals scored & conceded
void calculateGoalDifferential(Team* team, int goalsScored, int goalsConceded);

// Calculate the teams overall rating (based on team average)
float calculateTeamRating(Team* team);

// Print the information of a Team
void printTeam(const Team* team, bool showPlayers);