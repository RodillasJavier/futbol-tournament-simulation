#include "league.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* HELPER FUNCTION PROTOTYPE(s)*/
int compareTeams(const void* a, const void* b, void* league);

/* FUNCTIONS */

// Create a new league with a name and region
League* createLeague(const char* name, const char* region, int maxTeams)
{
    // Validate input
    if (name == NULL) {
        fprintf(stderr, "Error: Tried to set league name to a NULL string.\n");
        return NULL;
    } else if (region == NULL) {
        fprintf(stderr, "Error: Tried to set league region to a NULL string.\n");
        return NULL;
    }

    // Create league & NULL check
    League* league = (League*)malloc(sizeof(League));
    if (league == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for a league.\n");
        return NULL;
    }

    // Name
    strncpy(league -> name, name, sizeof(league -> name) - 1);
    league -> name[sizeof(league -> name)] = '\0';

    // Region

    // Teams

    // Schedule

    // League table


}

// Free all memory allocated for the league
void destroyLeague(League* league);

// Add a team to the league
bool addTeamToLeague(League* league, Team* team);

// Remove a team from the league by name
bool removeTeamFromLeague(League* league, const char* teamName);

// Generate a schedule for the league (each team plays each other twice)
bool generateSchedule(League* league);

// Simulate the next matchday in the league
bool simulateMatchday(League* league);

// Simulate the entire season
void simulateSeason(League* league);

// Update the league table based on team records
void updateLeagueTable(League* league);

// Get a team from the league by name
Team* getTeamByName(const League* league, const char* name);

// Get a team's position in the league table
int getTeamPosition(const League* league, const Team* team);

// Print the league table (standings)
void printLeagueTable(League* league);

// Print all teams in the league
void printLeagueTeams(const League* league);

// Print the complete league schedule
void printLeagueSchedule(const League* league);

// Print the schedule for a specific matchday
void printMatchdaySchedule(const League* league, int matchday);

// Print the results for a completed matchday
void printMatchdayResults(const League* league, int matchday);



/* HELER FUNCTIONS */

// Helper function to compare teams for sorting the league table
int compareTeams(const void* a, const void* b, void* league);