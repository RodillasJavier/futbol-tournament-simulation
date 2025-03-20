/**
 * @author Javier A. Rodillas
 * @details Header file for the Player class file
 */



/* INCLUDE STATEMENTS */

#include "match.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* METHODS */

// Constructor for the Match class
Match* createMatch(Team* homeTeam, Team* awayTeam, const char* date)
{
    // Allocate memory for match
    Match* match = (Match*)malloc(sizeof(Match));

    // Null check match
    if (match == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for a match.\n");
        return NULL;
    }

    // Null check home & away teams
    if (homeTeam == NULL)
    {
        fprintf(stderr, "Error: Passed in a home team that doesn't exist.\n");
        return NULL;
    } else if (awayTeam == NULL) {
        fprintf(stderr, "Error: Passed in an away team that doesn't exist.\n");
        return NULL;
    }

    // Null check date
    if (date == NULL)
    {
        fprintf(stderr, "Error: Passed in a date that doesn't exist.\n");
        return NULL;
    }    

    // Set home & away teams
    match -> homeTeam = homeTeam;
    match -> awayTeam = awayTeam;

    // Score
    match -> homeScore = 0;
    match -> awayScore = 0;
    match -> numScorers = 0;
    match -> scorers = NULL;
    match -> scorerTeamIndices = NULL;
    match -> scoringMinutes = NULL;

    // Set match date
    strncpy(match -> date, date, sizeof(date) - 1);
    match -> date[sizeof(date) - 1] = '\0';

    // Match status
    match -> isCompleted = false;

    return match;
}

void destroyMatch(Match* match);
void recordGoal(Match* match, Player* scorer, int teamIndex, int minute);
void printMatchResult(Match* match);
void printMatchReport(Match* match);
void updateTeamRecords(Match* Match);
int getMatchWinner(Match* match);