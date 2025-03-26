#ifndef TOURNAMENT_H
#define TOURNAMENT_H

/**
 * @author Javier A. Rodillas
 * @details Header file for the tournament class file
 */



/* INCLUDE STATEMENTS & DEFINITIONS */

#include "team.h"
#include "match.h"
#include <stdbool.h>

#define MAX_TOURNAMENT_NAME_LENGTH 50
#define MAX_TEAMS_IN_TOURNAMENT 32
#define MAX_ROUNDS 5  // Enough for 32 teams (2^5 = 32)

typedef enum {
    ROUND_OF_32 = 0,
    ROUND_OF_16 = 1,
    QUARTER_FINAL = 2,
    SEMI_FINAL = 3,
    FINAL = 4
} TournamentRound;

typedef struct {
    // Name
    char name[MAX_TOURNAMENT_NAME_LENGTH];  // Name of the tournament

    // Roster of teams
    Team** teams;                           // Array of teams in the tournament
    int numTeams;                           // Number of teams in the tournament

    // Bracket
    Match*** bracket;                       // 3D array: [round][match]
    int numRounds;                          // Number of rounds in the tournament
    int* matchesPerRound;                   // Number of matches for each round
    int currentRound;                       // Current round (0-based)

    // Results
    Team* winner;                           // Tournament winner
    bool isComplete;                        // Flag if tournament is complete
} Tournament;



/* FUNCTION PROTOTYPES */

/**
 * Create a new tournament with a name
 * 
 * @param name of the tournament
 * 
 * @return pointer to the tournament we created
 */
Tournament* createTournament(const char* name);

/**
 * Free all memory allocated for the tournament
 * 
 * @param tournament pointer to the tournament that we want to destroy
 */
void destroyTournament(Tournament* tournament);

/**
 * Add a team to the tournament
 * 
 * @param tournament 
 * @param team 
 * 
 * @return boolean indicating success or failure to add the team
 */
bool addTeamToTournament(Tournament* tournament, Team* team);

/**
 * Remove a team from the tournament by name
 * 
 * @param tournament that we want to add a team to
 * @param teamName of the team we want to add to the tournament
 * 
 * @return  boolean indicating success or failure to remove a team from the 
 *          tournament
 */
bool removeTeamFromTournament(Tournament* tournament, const char* teamName);

/**
 * Seed/draw teams into the tournament bracket
 * 
 * @param tournament that we want to make team & seed drawings for
 * 
 * @return boolean indicating success or failure to make a drawing
 */
bool drawTournament(Tournament* tournament);

/**
 * Simulate a specific round of the tournament
 * 
 * @param tournament that we are simulating a round of
 * @param round that we want to simulate
 * 
 * @return boolean indicating success or faillure to simulate a round
 */
bool simulateTournamentRound(Tournament* tournament, int round);

/**
 * Simulate the entire tournament from current round to final
 * 
 * @param tournament that we want to simulate
 */
void simulateEntireTournament(Tournament* tournament);

/**
 * Get the winner of the tournament
 * 
 * @param tournament that we want to get the winner of
 * 
 * @return pointer to the team that won the tournament
 */
Team* getTournamentWinner(const Tournament* tournament);

/**
 * Print the tournament bracket
 * 
 * @param tournament that we want to print the bracket of
 */
void printTournamentBracket(const Tournament* tournament);

/**
 * Print the matches for a specific round
 * 
 * @param tournament that we want to print the matches of for a specific round
 * @param round that we want to simulate specifically
 */
void printRoundMatches(const Tournament* tournament, int round);

/**
 * Print the results for a completed round
 * 
 * @param tournament that we want to print the results of for a specific round
 */
void printRoundResults(const Tournament* tournament, int round);

/**
 * Get the name of a tournament round
 * 
 * @param round that we want to get the name of
 * 
 * @return pointer to the name of the round
 */
const char* getRoundName(int round);

#endif // TOURNAMENT_H