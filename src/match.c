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
    // Null check match
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried to record a goal for a match that doesn't exist.\n");
        return;
    }    

    // Null check scorer
    if (scorer == NULL)
    {
        fprintf(stderr, "Error: Tried to record a goal by a player that doesn't exist.\n");
        return;
    }

    // Make sure the team index is either 0 (home) or 1 (away)
    if (teamIndex != 0 && teamIndex != 1)
    {
        fprintf(stderr, "Error: Invalid team index of %d, must be either 0 (home) or 1 (away).\n", 
                teamIndex);
        return;
    }
    
    // Verify 0 <= minute <= 90
    if (minute < 0 || minute > 90)
    {
        fprintf(stderr, "Error: Invalid scoring minute of %d, must be during the game (minute 0 - 90).\n", 
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
        printf("%s vs %s: Match not yet played\n", 
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
        printf("%s vs %s: Match not yet played\n", 
                match->homeTeam->name, match->awayTeam->name);
    }

    // The match HAS been played
    else 
    {
        // Header
        printf("Match Report: %s\n", match->date);

        // Scoreboard
        printScoreBoard(match);

        // Goal information (if any)
        if (match -> numScorers > 0)
        {
            printf("\nGoal Scorers:\n");

            // Print each goal's information
            for (int i = 0; i < match -> numScorers; i++) {
                printf("%d' - %s (%s)\n", 
                        match -> scoringMinutes[i],
                        match -> scorers[i] -> name,
                        // If scoring team was home then => print home team name : else print away team name
                        match -> scorerTeamIndices[i] == 0 ? match -> homeTeam -> name : match -> awayTeam -> name);
            }
        }

        // printf("\nMatch Statistics:\n");
        // Add more statistics here in the future?
    }    
}

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

    // Get the winner of the match
    int matchWinner = getMatchWinner(match);

    // Home team won
    if (matchWinner == 0)
    {
        updateRecord(match -> homeTeam, true, false, false);
        updateRecord(match -> awayTeam, false, true, false);
    }

    // Away team won
    else if (matchWinner == 1)
    {
        updateRecord(match -> homeTeam, false, true, false);
        updateRecord(match -> awayTeam, true, false, false);
    }

    // Draw
    else if (matchWinner == -1)
    {
        updateRecord(match -> homeTeam, false, false, true);
        updateRecord(match -> awayTeam, false, false, true);
    }
    
    updateGoals(match -> homeTeam, match -> homeScore, match -> awayScore);
    updateGoals(match -> awayTeam, match -> awayScore, match -> homeScore);

    updatePoints(match -> homeTeam);
    updatePoints(match -> awayTeam);
}

int getMatchWinner(Match* match);



/* HELPER FUNCTIONS */
void printScoreBoard(Match* match)
{
    // Null check match
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried to print the scoreboard for a match that doesn't exist.\n");
        return;
    }

    // Scoreboard
    printf("%s %d - %d %s\n", 
        match -> homeTeam -> name, match -> homeScore, 
        match -> awayScore, match -> awayTeam -> name);
}