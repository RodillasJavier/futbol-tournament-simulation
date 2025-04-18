/**
 * @author Javier A. Rodillas
 * @details Header file for the Player class file
 */



/* INCLUDE & MACRO STATEMENTS */

// Include
#include "match.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* HELPER FUNCTION PROTOYPE(s) */
void printScoreBoard(Match* match);



/* FUNCTIONS */

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
    strncpy(match -> date, date, sizeof(match -> date) - 1);
    match -> date[sizeof(match -> date) - 1] = '\0';

    // Match status
    match -> isCompleted = false;

    return match;
}

// Destructor for the Match class
void destroyMatch(Match* match)
{
    // Null check match
    if (match != NULL)
    {
        // Free scorers list if it exists
        if (match -> scorers != NULL) { free(match -> scorers); }

        // Free scorerTeamIndices if it exists
        if (match -> scorerTeamIndices != NULL) { free(match -> scorerTeamIndices); }
        
        // Free scoringMinutes if it exists
        if (match -> scoringMinutes != NULL) { free(match -> scoringMinutes); }
        
        // Free the team
        free(match);
    }
}

// Record a goal scored in the match (0 => home; 1 => away)
void recordGoal(Match* match, Player* scorer, int teamIndex, int minute)
{
    // Validate input
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried to record a goal for a match that doesn't exist.\n");
        return;
    }
    else if (scorer == NULL)
    {
        fprintf(stderr, "Error: Tried to record a goal by a player that doesn't exist.\n");
        return;
    }   // Make sure the team index is either 0 (home) or 1 (away)
    else if (teamIndex != 0 && teamIndex != 1)
    {
        fprintf(stderr, "Error: Invalid team index of %d, must be either 0 (home) or 1 (away).\n", 
                teamIndex);
        return;
    }
    else if (minute < 0)
    {
        fprintf(stderr, "Error: Invalid scoring minute of %d, must be non-negative.\n", 
                minute);
        return;
    }
    
    // Number of scorers
    match -> numScorers++;

    // Resizing the arrays of scorers, team indices and minutes that we created
    match -> scorers = (Player**)realloc(match -> scorers, 
                                        match -> numScorers * sizeof(Player*));
    match -> scorerTeamIndices = (int*)realloc(match -> scorerTeamIndices, 
                                                match -> numScorers * sizeof(int));
    match -> scoringMinutes = (int*)realloc(match -> scoringMinutes, 
                                            match -> numScorers * sizeof(int));

    // Update scorers, team indices, and minutes with most goal details
    match -> scorers[match -> numScorers - 1] = scorer;
    match -> scorerTeamIndices[match -> numScorers - 1] = teamIndex;
    match -> scoringMinutes[match -> numScorers - 1] = minute;

    // Home team scored
    if (teamIndex == 0) { match -> homeScore++; }

    // Away team scored => (teamIndex == 1)
    else { match -> awayScore++; }

    // Update players stats
    scoreGoal(scorer);

    // For live score updates
    // printScoreBoard(match);
}

// Print the results of the match
void printMatchResult(Match* match)
{
    // Null check match
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried to print match result for a match that doesn't exist.\n");
        return;
    }
    
    // The match hasn't been played yet
    if (match -> isCompleted == false) {
        fprintf(stdout, 
                "%s vs %s: Match not yet played\n", 
                match->homeTeam->name, match->awayTeam->name);
    }

    // The match HAS been played
    else 
    {
        // Scoreboard
        printScoreBoard(match);
    }
}

// Print a detailed match report with all of the minutes & scorers
void printMatchReport(Match* match)
{
    // Null check match
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried to print match result for a match that doesn't exist.\n");
        return;
    }
    
    // The match hasn't been played yet
    if (match -> isCompleted == false) {
        fprintf(stdout, 
                "%s vs %s: Match not yet played\n", 
                match->homeTeam->name, match->awayTeam->name);
    }

    // The match HAS been played
    else 
    {
        // Header
        fprintf(stdout, "Match Report: %s\n", match->date);

        // Scoreboard
        printScoreBoard(match);

        // Goal information (if any)
        if (match -> numScorers > 0)
        {
            fprintf(stdout, "\nGoal Scorers:\n");

            // Print each goal's information
            for (int i = 0; i < match -> numScorers; i++) {
                fprintf(stdout, 
                        "%d' - %s (%s)\n", 
                        match -> scoringMinutes[i],
                        match -> scorers[i] -> name,
                        // If scoring team was home then => print home team name : else print away team name
                        match -> scorerTeamIndices[i] == 0 ? match -> homeTeam -> name : match -> awayTeam -> name);
            }
        }

        // fprintf(stdout, "\nMatch Statistics:\n");
        // Add more statistics here in the future?
    }    
}

// Update the records/tables of the teams involved in the match
void updateTeamRecords(Match* match)
{
    // Null check match
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried to update team records with results from a match that doesn't exist.\n");
        return;
    }

    // Make sure the match has been played
    if (match -> isCompleted == false) {
        fprintf(stderr, "Error: Cannot update team records because the match (%s vs %s) has not been played yet.\n", 
                match->homeTeam->name, match->awayTeam->name);
        return;
    }

    // Get the winner of the match & update their record
    int matchWinner = getMatchWinner(match);
    if (matchWinner == HOME_TEAM) {
        updateRecord(match -> homeTeam, true, false, false);
        updateRecord(match -> awayTeam, false, true, false);
    } else if (matchWinner == AWAY_TEAM) {
        updateRecord(match -> homeTeam, false, true, false);
        updateRecord(match -> awayTeam, true, false, false);
    } else if (matchWinner == DRAW) {
        updateRecord(match -> homeTeam, false, false, true);
        updateRecord(match -> awayTeam, false, false, true);
    }
    
    // Update goals and points for home team
    updateGoals(match -> homeTeam, match -> homeScore, match -> awayScore);
    updatePoints(match -> homeTeam);

    // Update goals and points for away team
    updateGoals(match -> awayTeam, match -> awayScore, match -> homeScore);
    updatePoints(match -> awayTeam);
}

// Get match winner (0 => Home; 1 => Away; -1 => Draw)
int getMatchWinner(Match* match)
{
    // Null check match
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried to get the winner from a match that doesn't exist.\n");
        return -2;
    }

    // Make sure the match has been played
    if (match -> isCompleted == false) {
        fprintf(stderr, "Error: Cannot get winner because the match (%s vs %s) has not been played yet.\n", 
                match->homeTeam->name, match->awayTeam->name);
        return -2;
    }


    if (match -> homeScore > match -> awayScore) { return HOME_TEAM; }
    if (match -> homeScore < match -> awayScore) { return AWAY_TEAM; }
    return DRAW;
}



/* HELPER FUNCTION(S) */

// Pretty print out the scoreboard
void printScoreBoard(Match* match)
{
    // Null check match
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried to print the scoreboard for a match that doesn't exist.\n");
        return;
    }

    // Scoreboard
    fprintf(stdout, 
            "%s %d - %d %s\n", 
            match -> homeTeam -> name, match -> homeScore, 
            match -> awayScore, match -> awayTeam -> name);
}