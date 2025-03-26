/**
 * @author Javier A. Rodillas
 * @details Implementation of the 'tournament' class
 */



/* INCLUDE STATEMENTS */

#include "tournament.h"
#include <stdlib.h>
#include <stdio.h>



/* HELPER FUNCTION PROTOTYPES */

bool advanceTeam(Tournament* tournament, int round, int matchIndex);



/* FUNCTIONS */

// Create a new tournament with a name
Tournament* createTournament(const char* name)
{
    // Allocate memory for tournament & NULL check
    Tournament* tournament = (Tournament*)malloc(sizeof(Tournament));
    if (tournament == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for tournament.\n");
        return NULL;
    }

    // Initialize tournament attributes
    strncpy(tournament -> name, name, MAX_TOURNAMENT_NAME_LENGTH - 1);
    tournament -> name[MAX_TOURNAMENT_NAME_LENGTH - 1] = '\0';  // Ensure null termination

    // Allocate memory for teams & NULL check
    tournament -> teams = (Team**)malloc(MAX_TEAMS_IN_TOURNAMENT * sizeof(Team*));
    if (tournament -> teams == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for teams array.\n");
        free(tournament);
        return NULL;
    }

    // Teams
    tournament -> numTeams = 0;

    // Bracket
    tournament -> bracket = NULL;
    tournament -> numRounds = 0;
    tournament -> matchesPerRound = NULL;
    tournament -> currentRound = 0;

    // Results
    tournament -> winner = NULL;
    tournament -> isComplete = false;

    return tournament;
}

// Free all memory allocated for the tournament
void destroyTournament(Tournament* tournament)
{
    // Null check the tournament
    if (tournament == NULL) { return; }

    // Free teams array (but not the teams themselves, as they might be used elsewhere)
    free(tournament->teams);

    // Free bracket if it exists
    if (tournament->bracket != NULL)
    {
        // Look at each round in the tournament
        for (int i = 0; i < tournament->numRounds; i++)
        {
            // If the bracket exists for that round, free it
            if (tournament->bracket[i] != NULL)
            {
                // Free each match in the round
                for (int j = 0; j < tournament->matchesPerRound[i]; j++)
                {
                    destroyMatch(tournament->bracket[i][j]);
                }

                // Free the ith round of the bracket
                free(tournament->bracket[i]);
            }
        }

        // Free the bracket and the matches per round array
        free(tournament->bracket);
        free(tournament->matchesPerRound);
    }

    // Free the tournament struct
    free(tournament);
}

// Add a team to the tournament
bool addTeamToTournament(Tournament* tournament, Team* team);

// Remove a team from the tournament by name
bool removeTeamFromTournament(Tournament* tournament, const char* teamName);

// Seed/draw teams into the tournament bracket
bool drawTournament(Tournament* tournament);

// Simulate a specific round of the tournament
bool simulateTournamentRound(Tournament* tournament, int round);

// Simulate the entire tournament from current round to final
void simulateEntireTournament(Tournament* tournament);

// Get the winner of the tournament
Team* getTournamentWinner(const Tournament* tournament);

// Print the tournament bracket
void printTournamentBracket(const Tournament* tournament);

// Print the matches for a specific round
void printRoundMatches(const Tournament* tournament, int round);

// Print the results for a completed round
void printRoundResults(const Tournament* tournament, int round);

// Get the name of a tournament round
const char* getRoundName(int round);



/* HELPER FUNCTIONS */

// Helper function to determine if a team advances to the next round
bool advanceTeam(Tournament* tournament, int round, int matchIndex);