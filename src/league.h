#ifndef LEAGUE_H
#define LEAGUE_H

/**
 * @author Javier A. Rodillas
 * @details Header file for the league class. 
 */

/* INCLUDE STATEMENTS & DEFINITIONS */
#include "player.h"
#include "team.h"
#include "match.h"
#include "modules/match_simulation.h"
#include <stdbool.h>

#define MAX_LEAGUE_NAME_LENGTH 50
#define MAX_REGION_LENGTH 50




/* FUNCTION PROTOTYPES */

typedef struct {
    // Name & Region
    char name[MAX_LEAGUE_NAME_LENGTH];      // Name of the league
    char region[MAX_REGION_LENGTH];         // Region/country of the league

    // Teams
    Team** teams;                           // Array of teams in the league
    int numTeams;                           // Number of teams in the league
    int maxTeams;                           // Maximum number of teams allowed

    // Schedule
    Match*** schedule;                      // 2D array: [matchday][match]
    int* matchesPerMatchday;                // Number of matches for each matchday
    int numMatchdays;                       // Number of matchdays in the season
    int currentMatchday;                    // Current matchday (0-based)
    bool scheduleGenerated;                 // Flag if schedule has been generated

    // League table
    int** leagueTable;                      // Sorted indices of teams by rank
} League;


/**
 * Create a new league with a name and region
 * 
 * @param name of the league
 * @param region that the league is in
 * @param maxTeams in the league
 * 
 * @return pointer to our newly created league
 */
League* createLeague(const char* name, const char* region, int maxTeams);

/**
 * Free all memory allocated for the league
 * 
 * @param league that we want to free from memory
 */
void destroyLeague(League* league);
 
/**
 * Add a team to the league
 * 
 * @param league that we want to add a team to
 * @param team that we want to add to the league
 * 
 * @return boolean indicating success or failure to add to league
 */
bool addTeamToLeague(League* league, Team* team);

/**
 * Remove a team from the league by name
 * 
 * @param league that we want to remove a team from
 * @param teamName of the team that we want to remove
 * 
 * @return boolean indicating success or failure to remove from league
 */
bool removeTeamFromLeague(League* league, const char* teamName);

/**
 * Generate a schedule for the league (each team plays each other twice)
 * 
 * @param league that we want to generate the schedule for
 * 
 * @return boolean indicating success or failure to generate a schedule
 */
bool generateSchedule(League* league);

/**
 * Simulate the next matchday in the league
 * 
 * @param league that we want to simulate the next matchday for
 * 
 * @return boolean indicating success or failure of simulation
 */
bool simulateMatchday(League* league);

/**
 * Simulate the entire season
 * 
 * @param league that we want to simulate the season of
 */
void simulateSeason(League* league);

/**
 * Update the league table based on team records
 * 
 * @param league that we want to update the table of
 */
void updateLeagueTable(League* league);

/**
 * Get a team from the league by name
 * 
 * @param league that we are searching in for a team
 * @param name that we are searching a team for 
 * 
 * @return pointer to the team that we found (if any) with the name we searched
 */
Team* getTeamByName(const League* league, const char* name);

/**
 * Get a team's position in the league table
 * 
 * @param league owning the table we are searching in
 * @param team that we are looking for the position of
 * 
 * @return integer representing the place/position of the team
 */
int getTeamPosition(const League* league, const Team* team);

/**
 * Print the league table (standings)
 * 
 * @param league that we are printing the table of
 */
void printLeagueTable(League* league);

/**
 * Print all teams in the league
 * 
 * @param league that we are printing all the teams of
 */
void printLeagueTeams(const League* league);

/**
 * Print the complete league schedule
 * 
 * @param league that we want to print the schedule of
 */
void printLeagueSchedule(const League* league);

/**
 * Print the schedule for a specific matchday
 * 
 * @param league that we are printing the matchday schedule of
 * @param matchday representing the exact matchday we are printing
 */
void printMatchdaySchedule(const League* league, int matchday);

/**
 * Print the results for a completed matchday
 * 
 * @param league that we are printing the results of a matchday of
 * @param matchday that we are printing the results of
 */
void printMatchdayResults(const League* league, int matchday);

#endif // LEAGUE_H