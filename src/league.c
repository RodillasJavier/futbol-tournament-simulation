/**
 * @author Javier A. Rodillas
 * @details Implementation for the 'League' class
 * 
 * @cite:   Inspiration/algorithm for the round robin implementation:
 *          https://stackoverflow.com/questions/6648512/scheduling-algorithm-for-a-round-robin-tournament
 */



/* INCLUDE STATEMENTS */

#include "league.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/* HELPER FUNCTION PROTOTYPE(s)*/
int compareTeams(const void* a, const void* b, void* league);
void destroySchedule(League* league);
bool generateRound(League* league, int* teamIndices, int startMatchday, bool swapHomeAway);




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

    /*
     * For a round-robin tournament, each team plays against every other team
     * If there are n teams, each team plays (n-1) matches, so total matchdays = (n-1)
     * Total matches per round = n/2 (each matchday has n/2 matches)
     * For a double round-robin, we have 2*(n-1) matchdays total
     */

    int matchdaysPerRound = league -> numTeams - 1;
    league -> numMatchdays = 2 * matchdaysPerRound; // Double round-robin
    league -> currentMatchday = 0;

    // Allocate memory for the schedule & NULL check
    league -> schedule = (Match***)malloc(league -> numMatchdays * sizeof(Match**));
    if (league -> schedule == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for schedule.\n");
        return false;
    }

    // Allocate memory for matches per matchday array & NULL check
    league -> matchesPerMatchday = (int*)malloc(league -> numMatchdays * sizeof(int));
    if (league -> matchesPerMatchday == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for matchesPerMatchday.\n");
        free(league -> schedule);
        league -> schedule = NULL;
        return false;
    }

    // Fill matchesPerMatchday
    for (int i = 0; i < league -> numMatchdays; i++)
    {
        league -> matchesPerMatchday[i] = league -> numTeams / 2;
    }

    // Create a temporary array of team indices for scheduling & NULL check
    int* teamIndices = (int*)malloc(league -> numTeams * sizeof(int));
    if (teamIndices == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for teamIndices.\n");
        free(league -> schedule);
        free(league -> matchesPerMatchday);
        league -> schedule = NULL;
        league -> matchesPerMatchday = NULL;
        return false;
    }

    // Fill teamIndices
    for (int i = 0; i < league -> numTeams; i++)
    {
        teamIndices[i] = i;
    }

    // Generate the first half of the season
    bool firstRoundGenerated = generateRound(league, teamIndices, 0, false);
    if (firstRoundGenerated == false)
    {
        fprintf(stderr, "Error: Encountered an error generating the first half of the season for %s\n", 
                league -> name);
        free(teamIndices);
        return false;
    }
    
    // Generate the second half of the season
    bool secondRoundGenerated = generateRound(league, teamIndices, matchdaysPerRound, true);
    if (secondRoundGenerated == false)
    {
        fprintf(stderr, "Error: Encountered an error generating the second half of the season for %s\n", 
            league -> name);
        free(teamIndices);
        return false;
    }
    

    free(teamIndices);
    league -> scheduleGenerated = true;
    
    // Initialize league table
    updateLeagueTable(league);

    return true;
}

// Simulate the next matchday in the league
bool simulateMatchday(League* league)
{
    // NULL check league
    if (league == NULL)
    {
        fprintf(stderr, "Error: Cannot simulate matchday for NULL league.\n");
        return false;
    }

    // Make sure that a schedule exists in order for us to simulate a matchday
    if (league -> scheduleGenerated == false)
    {
        fprintf(stderr, "Error: Cannot simulate matchday without a generated schedule.\n");
        return false;
    }

    // Make sure that the matchday we are on is during the season
    if (league -> currentMatchday >= league -> numMatchdays)
    {
        fprintf(stderr, "Error: Season is already complete. No more matchdays to simulate.\n");
        return false;
    }

    fprintf(stdout, "Simulating Matchday %d of %s...\n", 
            league -> currentMatchday + 1, league -> name);

    // Simulate all matches for the current matchday
    for (int i = 0; i < league -> matchesPerMatchday[league -> currentMatchday]; i++)
    {
        // Get a match from the matchday schedule
        Match* match = league -> schedule[league -> currentMatchday][i];
        
        // Skip already played matches
        if (match -> isCompleted == true) 
        {
            printf("Match between %s and %s was already played.\n", 
                    match -> homeTeam -> name, match -> awayTeam -> name);
            continue;
        }
        
        // Simulate the match
        simulateMatch(match);
        
        // Print the result
        printf("  ");
        printMatchResult(match);
    }

    // Advance to next matchday
    league -> currentMatchday++;

    // Update league table
    updateLeagueTable(league);

    return true;
}

// Simulate the entire season
void simulateSeason(League* league)
{
    // Validate input
    if (league == NULL) {
        fprintf(stderr, "Error: Cannot simulate season for NULL league.\n");
        return;
    } else if (league -> scheduleGenerated == false) {
        fprintf(stderr, "Error: Cannot simulate season without a generated schedule.\n");
        return;
    }

    fprintf(stdout, "Simulating entire season for %s...\n",
            league -> name);

    // Reset current matchday if needed
    if (league -> currentMatchday > 0)
    {
        fprintf(stdout, "Resetting league to start of season for %s.\n", 
                league -> name);
        
        // Reset all teams' stats
        for (int i = 0; i < league -> numTeams; i++)
        {
            Team* team = league -> teams[i];
            team -> wins = 0;
            team -> losses = 0;
            team -> draws = 0;
            team -> goalsScored = 0;
            team -> goalsConceded = 0;
            team -> goalDifferential = 0;
            team -> points = 0;
        }
        
        // Reset all matches
        for (int i = 0; i < league->numMatchdays; i++)
        {
            for (int j = 0; j < league->matchesPerMatchday[i]; j++)
            {
                Match* match = league->schedule[i][j];
                match -> isCompleted = false;
                match -> homeScore = 0;
                match -> awayScore = 0;
                match -> numScorers = 0;
                free(match -> scorers);
                free(match -> scorerTeamIndices);
                free(match -> scoringMinutes);
                match -> scorers = NULL;
                match -> scorerTeamIndices = NULL;
                match -> scoringMinutes = NULL;
            }
        }
        
        league -> currentMatchday = 0;
    }

    // Simulate each matchday
    while (league -> currentMatchday < league -> numMatchdays)
    {
        simulateMatchday(league);
        
        // Print current standings after each matchday
        fprintf(stdout, "\nStandings after Matchday %d:\n", league -> currentMatchday);
        printLeagueTable(league);
        fprintf(stdout, "\n");
    }

    fprintf(stdout, "\nSeason complete! Final standings:\n");
    printLeagueTable(league);
}

// Update the league table based on team records
void updateLeagueTable(League* league)
{
    // Validate input
    if (league == NULL) {
        fprintf(stderr, "Error: Can't update league table for a NULL league.\n");
        return;
    } else if (league->numTeams == 0) {
        fprintf(stderr, "Error: Can't update league table for '%s' with 0 teams.\n", 
                league -> name);
        return;
    }
    

    // Initialize or clear the league table
    if (league -> leagueTable == NULL)
    {
        // Allocate memory for the leage & NULL check
        league -> leagueTable = (int**)malloc(league -> numTeams * sizeof(int*));
        if (league -> leagueTable == NULL)
        {
            fprintf(stderr, "Error: Failed to allocate memory for league table.\n");
            return;
        }
        
        // Fill league table
        for (int i = 0; i < league -> numTeams; i++)
        {
            // Allocate memory for an entry in the league table & NULL check
            league -> leagueTable[i] = (int*)malloc(2 * sizeof(int));
            if (league -> leagueTable[i] == NULL)
            {
                fprintf(stderr, "Error: Failed to allocate memory for league table entry.\n");
                // Clean up
                for (int j = 0; j < i; j++)
                {
                    free(league -> leagueTable[j]);
                }
                free(league -> leagueTable);
                league -> leagueTable = NULL;
                return;
            }

            league -> leagueTable[i][0] = i;        // Team index
            league -> leagueTable[i][1] = 0;        // Points
        }
    }

    // Update points in the table for each table
    for (int i = 0; i < league -> numTeams; i++)
    {
        Team* team = league -> teams[league -> leagueTable[i][0]];
        league -> leagueTable[i][1] = team -> points;
    }

    // Sort the table by points (descending)
    qsort_r(league -> leagueTable, league -> numTeams, sizeof(int*), compareTeams, league);
}

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
int compareTeams(const void* a, const void* b, void* leaguePtr)
{
    League* league = (League*)leaguePtr;
    int idxA = *((int*)a);
    int idxB = *((int*)b);
    
    Team* teamA = league -> teams[idxA];
    Team* teamB = league -> teams[idxB];
    
    // Primary sort by points (descending)
    if (teamA -> points != teamB -> points)
    {
        return teamB -> points - teamA -> points;
    }
    
    // Secondary sort by goal difference (descending)
    if (teamA -> goalDifferential != teamB -> goalDifferential)
    {
        return teamB -> goalDifferential - teamA -> goalDifferential;
    }
    
    // Tertiary sort by goals scored (descending)
    if (teamA -> goalsScored != teamB -> goalsScored)
    {
        return teamB -> goalsScored - teamA -> goalsScored;
    }
    
    // If all else is equal, sort alphabetically by name
    return strcmp(teamA -> name, teamB -> name);
}

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

// Helper function to generate a round of a round-robin tournament
bool generateRound(League* league, int* teamIndices, int startMatchday, bool swapHomeAway)
{
    int matchdaysPerRound = league -> numTeams - 1;

    // Generate FIRST HALF of the season (first round-robin)
    for (int matchday = 0; matchday < matchdaysPerRound; matchday++)
    {
        int currentMatchday = startMatchday + matchday;

        // Allocate memory for matches on this matchday
        league -> schedule[currentMatchday] = (Match**)malloc(league -> matchesPerMatchday[currentMatchday] * sizeof(Match*));
        if (league->schedule[currentMatchday] == NULL)
        {
            fprintf(stderr, "Error: Failed to allocate memory for matchday %d.\n", currentMatchday);
            return false;
        }
        
        // Create matches for this matchday
        for (int i = 0; i < league -> numTeams / 2; i++)
        {
            int homeIdx = teamIndices[i];
            int awayIdx = teamIndices[league->numTeams - 1 - i];

            // Swap home/away for the second round
            if (swapHomeAway)
            {
                int temp = homeIdx;
                homeIdx = awayIdx;
                awayIdx = temp;
            }

            // Create a date string (placeholder)
            char dateStr[20];
            sprintf(dateStr, "MD%d", currentMatchday + 1);

            // Create the new match
            Match* match = createMatch(league -> teams[homeIdx], league -> teams[awayIdx], dateStr);
            if (match == NULL)
            {
                fprintf(stderr, "Error: Failed to create match for matchday %d.\n", currentMatchday);
                return false;
            }

            // Add new match to the schedule
            league->schedule[currentMatchday][i] = match;
        }

        // Rotate teams for next matchday (keeping first team fixed)
        int temp = teamIndices[1];
        for (int i = 1; i < league -> numTeams - 1; i++)
        {
            teamIndices[i] = teamIndices[i + 1];
        }
        teamIndices[league -> numTeams - 1] = temp;
    }
}