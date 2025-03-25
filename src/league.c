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
    strncpy(league -> name, name, MAX_LEAGUE_NAME_LENGTH - 1);
    league -> name[MAX_LEAGUE_NAME_LENGTH - 1] = '\0';

    // Region
    strncpy(league -> region, region, MAX_REGION_LENGTH - 1);
    league -> region[MAX_REGION_LENGTH - 1] = '\0';

    // Teams
    league -> numTeams = 0;
    league -> maxTeams = maxTeams;
    league -> teams = (Team**)malloc(sizeof(Team*) * maxTeams);
    if (league -> teams == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for the teams array for %s.\n", 
                league -> name);
        free(league);
        return NULL;
    }

    // Schedule
    league -> schedule = NULL;
    league -> matchesPerMatchday = NULL;
    league -> numMatchdays = 0;
    league -> currentMatchday = 0;
    league -> scheduleGenerated = false;

    // League table
    league -> leagueTable = NULL;

    return league;
}

// Free all memory allocated for the league
void destroyLeague(League* league)
{
    // NULL check league
    if (league == NULL)
    {
        return;
    }

    // Free teams array (but not the teams themselves, as they might be used elsewhere)
    free(league -> teams);

    // Free schedule if it exists
    destroySchedule(league);

    // Free league table if it exists
    if (league -> leagueTable != NULL)
    {
        // Free each team in the league table
        for (int i = 0; i < league -> numTeams; i++)
        {
            free(league -> leagueTable[i]);
        }
        free(league -> leagueTable);
    }

    // Free the league
    free(league);
}

// Add a team to the league
bool addTeamToLeague(League* league, Team* team)
{
    // Validate input
    if (league == NULL) {
        fprintf(stderr, "Error: Cannot add team to NULL league.\n");
        return false;
    } else if (team == NULL) {
        fprintf(stderr, "Error: Cannot add NULL team to league.\n");
        return false;
    
    }

    // Check if league is full
    if (league -> numTeams >= league -> maxTeams)
    {
        fprintf(stderr, "Error: The league '%s' is already at maximum capacity (%d teams).\n", 
                league -> name, league -> maxTeams);
        return false;
    }

    // Check if team already exists in the league
    for (int teamIndex = 0; teamIndex < league -> numTeams; teamIndex++)
    {
        // If we find two teams with the same name => same team
        if (strcmp(league -> teams[teamIndex] -> name, team -> name) == 0)
        {
            fprintf(stderr, "Error: Team '%s' is already in the league.\n", 
                    team -> name);
            return false;
        }
    }

    // Add team to the league and increase our num teams counter
    league -> teams[league -> numTeams] = team;
    league -> numTeams++;

    // If schedule was already generated, it's now invalid
    if (league -> scheduleGenerated == true)
    {
        fprintf(stderr, "Warning: Adding a team invalidates the current schedule. Please regenerate the schedule for the league.\n");
        league -> scheduleGenerated = false;
    }

    return true;
}

// Remove a team from the league by name
bool removeTeamFromLeague(League* league, const char* teamName)
{
    // Validate input
    if (league == NULL) {
        fprintf(stderr, "Error: Cannot remove team from NULL league.\n");
        return false;
    } else if (teamName == NULL) {
        fprintf(stderr, "Error: Cannot remove team with NULL name.\n");
        return false;
    }

    // Find the team we want to remove
    int teamIndex = -1;
    for (int i = 0; i < league -> numTeams; i++)
    {
        // If we find a team with a matching name
        if (strcmp(league -> teams[i] -> name, teamName) == 0)
        {
            teamIndex = i;
            break;
        }
    }

    // If we didn't find any teams with matching names
    if (teamIndex == -1)
    {
        fprintf(stderr, "Error: Team '%s' not found in the league '%s'.\n", 
                teamName, league -> name);
        return false;
    }

    // Shift all teams left to fill the gap
    for (int i = teamIndex; i < league -> numTeams - 1; i++)
    {
        league -> teams[i] = league -> teams[i + 1];
    }

    league -> numTeams--;

    // If schedule was already generated, it's now invalid
    if (league->scheduleGenerated)
    {
        fprintf(stderr, "Warning: Removing a team invalidates the current schedule. Please regenerate.\n");
        league -> scheduleGenerated = false;
    }

    return true;
}

// Generate a schedule for the league (each team plays each other twice)
bool generateSchedule(League* league)
{
    // Null check league
    if (league == NULL)
    {
        fprintf(stderr, "Error: Cannot generate schedule for NULL league.\n");
        return false;
    }

    // Make sure there are at least 2 teams to create a schedule
    if (league -> numTeams < 2)
    {
        fprintf(stderr, "Error: Need at least 2 teams to generate a schedule.\n");
        return false;
    }

    // Clean up existing schedule if there is one
    destroySchedule(league);

    // For a round-robin tournament, each team plays against every other team
    // If there are n teams, each team plays (n-1) matches, so total matchdays = (n-1)
    // Total matches per round = n/2 (each matchday has n/2 matches)
    // For a double round-robin, we have 2*(n-1) matchdays

    int matchdaysPerRound = league->numTeams - 1;
    league->numMatchdays = 2 * matchdaysPerRound; // Double round-robin
    league->currentMatchday = 0;

    // Allocate memory for the schedule
    league->schedule = (Match***)malloc(league->numMatchdays * sizeof(Match**));
    if (league->schedule == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for schedule.\n");
        return false;
    }

    league->matchesPerMatchday = (int*)malloc(league->numMatchdays * sizeof(int));
    if (league->matchesPerMatchday == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for matchesPerMatchday.\n");
        free(league->schedule);
        league->schedule = NULL;
        return false;
    }

    // Initialize matchesPerMatchday
    for (int i = 0; i < league->numMatchdays; i++) {
        league->matchesPerMatchday[i] = league->numTeams / 2;
    }

    // Create a temporary array of team indices for scheduling
    int* teamIndices = (int*)malloc(league->numTeams * sizeof(int));
    if (teamIndices == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for teamIndices.\n");
        free(league->schedule);
        free(league->matchesPerMatchday);
        league->schedule = NULL;
        league->matchesPerMatchday = NULL;
        return false;
    }

    for (int i = 0; i < league->numTeams; i++) {
        teamIndices[i] = i;
    }

    // Generate first half of the season (first round-robin)
    for (int matchday = 0; matchday < matchdaysPerRound; matchday++) {
        // Allocate memory for matches on this matchday
        league->schedule[matchday] = (Match**)malloc(league->matchesPerMatchday[matchday] * sizeof(Match*));
        if (league->schedule[matchday] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for matchday %d.\n", matchday);
            free(teamIndices);
            // Clean up already allocated memory
            for (int i = 0; i < matchday; i++) {
                for (int j = 0; j < league->matchesPerMatchday[i]; j++) {
                    destroyMatch(league->schedule[i][j]);
                }
                free(league->schedule[i]);
            }
            free(league->schedule);
            free(league->matchesPerMatchday);
            league->schedule = NULL;
            league->matchesPerMatchday = NULL;
            return false;
        }

        // Create matches for this matchday
        for (int i = 0; i < league->numTeams / 2; i++) {
            int homeIdx = teamIndices[i];
            int awayIdx = teamIndices[league->numTeams - 1 - i];
            
            // Create a date string (placeholder)
            char dateStr[20];
            sprintf(dateStr, "MD%d", matchday + 1);
            
            // Create the match
            Match* match = createMatch(league->teams[homeIdx], league->teams[awayIdx], dateStr);
            if (match == NULL) {
                fprintf(stderr, "Error: Failed to create match for matchday %d.\n", matchday);
                free(teamIndices);
                // Clean up
                for (int j = 0; j < i; j++) {
                    destroyMatch(league->schedule[matchday][j]);
                }
                for (int j = 0; j < matchday; j++) {
                    for (int k = 0; k < league->matchesPerMatchday[j]; k++) {
                        destroyMatch(league->schedule[j][k]);
                    }
                    free(league->schedule[j]);
                }
                free(league->schedule);
                free(league->matchesPerMatchday);
                league->schedule = NULL;
                league->matchesPerMatchday = NULL;
                return false;
            }
            
            league->schedule[matchday][i] = match;
        }

        // Rotate teams for next matchday (keeping first team fixed)
        int temp = teamIndices[1];
        for (int i = 1; i < league->numTeams - 1; i++) {
            teamIndices[i] = teamIndices[i + 1];
        }
        teamIndices[league->numTeams - 1] = temp;
    }

    // Generate second half of the season (second round-robin with home/away swapped)
    for (int matchday = 0; matchday < matchdaysPerRound; matchday++) {
        int secondHalfMatchday = matchday + matchdaysPerRound;
        
        // Allocate memory for matches on this matchday
        league->schedule[secondHalfMatchday] = (Match**)malloc(league->matchesPerMatchday[secondHalfMatchday] * sizeof(Match*));
        if (league->schedule[secondHalfMatchday] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for matchday %d.\n", secondHalfMatchday);
            free(teamIndices);
            // Clean up
            for (int i = 0; i <= matchday + matchdaysPerRound - 1; i++) {
                if (league->schedule[i] != NULL) {
                    for (int j = 0; j < league->matchesPerMatchday[i]; j++) {
                        destroyMatch(league->schedule[i][j]);
                    }
                    free(league->schedule[i]);
                }
            }
            free(league->schedule);
            free(league->matchesPerMatchday);
            league->schedule = NULL;
            league->matchesPerMatchday = NULL;
            return false;
        }

        // Create matches for this matchday (swap home/away from first half)
        for (int i = 0; i < league->matchesPerMatchday[matchday]; i++) {
            Match* firstHalfMatch = league->schedule[matchday][i];
            
            // Create a date string (placeholder)
            char dateStr[20];
            sprintf(dateStr, "MD%d", secondHalfMatchday + 1);
            
            // Create the match with home/away swapped
            Match* match = createMatch(firstHalfMatch->awayTeam, firstHalfMatch->homeTeam, dateStr);
            if (match == NULL) {
                fprintf(stderr, "Error: Failed to create match for matchday %d.\n", secondHalfMatchday);
                free(teamIndices);
                // Clean up
                for (int j = 0; j < i; j++) {
                    destroyMatch(league->schedule[secondHalfMatchday][j]);
                }
                for (int j = 0; j <= secondHalfMatchday - 1; j++) {
                    for (int k = 0; k < league->matchesPerMatchday[j]; k++) {
                        destroyMatch(league->schedule[j][k]);
                    }
                    free(league->schedule[j]);
                }
                free(league->schedule);
                free(league->matchesPerMatchday);
                league->schedule = NULL;
                league->matchesPerMatchday = NULL;
                return false;
            }
            
            league->schedule[secondHalfMatchday][i] = match;
        }
    }

    free(teamIndices);
    league->scheduleGenerated = true;
    
    // Initialize league table
    updateLeagueTable(league);

    return true;
}

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

// Helper function to free/clean up the schedule of a league
void destroySchedule(League* league)
{
    // Free schedule if it exists
    if (league -> schedule != NULL)
    {
        // Look at each matchday
        for (int matchday = 0; matchday < league -> numMatchdays; matchday++)
        {
            // If there is a game (or more) on that day
            if (league -> schedule[matchday] != NULL)
            {
                // Look at each match during that matchday
                for (int match = 0; match < league -> matchesPerMatchday[matchday]; match++)
                {
                    destroyMatch(league -> schedule[matchday][match]);
                }
                free(league -> schedule[matchday]);
            }
        }
        free(league -> schedule);
        free(league -> matchesPerMatchday);
    }
}