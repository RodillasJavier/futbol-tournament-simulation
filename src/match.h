#ifndef MATCH_H
#define MATCH_H

/**
 * @author Javier A. Rodillas
 * @details Header file for the Player class file
 */



/* INCLUDE STATEMENTS */

#include "player.h"
#include "team.h"
#include <stdbool.h>



/* FUNCTION PROTOTYPES */

/**
 * Match @struct representing a game between two football teams
 */
typedef struct {
    // Teams
    Team* homeTeam;             // (0 => home)
    Team* awayTeam;             // (1 => away)

    // Score
    int homeScore;
    int awayScore;
    int numScorers;
    Player** scorers;           // Array of pointers to players who have scored
    int* scorerTeamIndices;     // Array of indices representing for what team the ith goal was for (0 => home, 1 => away)
    int* scoringMinutes;

    // Game status
    char* date[20];

    // Match status
    bool isCompleted;
} Match;

/**
 * Constructor for the Match class
 * 
 * @param homeTeam
 * @param awayTeam
 * @param date that the match is/was played on
 */
Match* createMatch(Team* homeTeam, Team* awayTeam, const char* date);

/**
 * Destructor for the Match class
 * 
 * @param match that we want to free the memory of
 */
void destroyMatch(Match* match);

/**
 * Record a goal scored in the match
 * 
 * @param match that we are recording a scored goal in
 * @param scorer that made the goal
 * @param teamIndex to indicate if it was a home or away team goal (0 => home; 1 => away)
 * @param minute that the goal was scored in
 */
void recordGoal(Match* match, Player* scorer, int teamIndex, int minute);

/**
 * Print the results of the match
 * 
 * @param Match that we are printing the results of
 */
void printMatchResult(Match* match);

/**
 * Print a detailed match report with all of the minutes & scorers
 * 
 * @param Match that we are reporting on
 */
void printMatchReport(Match* match);

/**
 * Update the records/tables of the teams involved in the match
 * 
 * @param Match that we are interested in
 */
void updateTeamRecords(Match* match);

/**
 * Get match winner
 * 
 * @param Match pointer to the match we are interested in
 * 
 * @return (0 for home; 1 for away; -1 for draw)
 */
int getMatchWinner(Match* match);

#endif /* MATCH_H */