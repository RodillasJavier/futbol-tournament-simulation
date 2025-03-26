/**
 * @author Javier A. Rodillas
 * @details Implementation of the 'tournament' class
 * 
 * @cite:   Fisher-Yates shuffle algorithm:
 *          https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/
 */



/* INCLUDE STATEMENTS */

#include "tournament.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



/* HELPER FUNCTION PROTOTYPES */

bool advanceTeam(Tournament* tournament, int round, int matchIndex);
void destroyBracket(Tournament* tournament);



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
    destroyBracket(tournament);

    // Free the tournament struct
    free(tournament);
}

// Add a team to the tournament
bool addTeamToTournament(Tournament* tournament, Team* team)
{
    // Validate input
    if (tournament == NULL) {
        fprintf(stderr, "Error: Cannot add team to NULL tournament.\n");
        return false;
    } else if (team == NULL) {
        fprintf(stderr, "Error: Cannot add NULL team to tournament.\n");
        return false;
    }

    // Check if tournament is full
    if (tournament->numTeams >= MAX_TEAMS_IN_TOURNAMENT)
    {
        fprintf(stderr, "Error: Tournament is already at maximum capacity (%d teams).\n", 
                MAX_TEAMS_IN_TOURNAMENT);
        return false;
    }

    // Check if team already exists in the tournament
    for (int i = 0; i < tournament->numTeams; i++)
    {
        if (strcmp(tournament->teams[i]->name, team->name) == 0)
        {
            fprintf(stderr, "Error: Team '%s' is already in the tournament.\n", team->name);
            return false;
        }
    }

    // Add team to the tournament
    tournament->teams[tournament->numTeams] = team;
    tournament->numTeams++;

    return true;
}

// Remove a team from the tournament by name
bool removeTeamFromTournament(Tournament* tournament, const char* teamName)
{
    // Validate input
    if (tournament == NULL) {
        fprintf(stderr, "Error: Cannot remove team from NULL tournament.\n");
        return false;
    } else if (teamName == NULL) {
        fprintf(stderr, "Error: Cannot remove team with NULL name.\n");
        return false;
    }

    // Find the team
    int teamIndex = -1;
    for (int i = 0; i < tournament->numTeams; i++)
    {
        if (strcmp(tournament->teams[i]->name, teamName) == 0)
        {
            teamIndex = i;
            break;
        }
    }

    // If we couldn't find the team
    if (teamIndex == -1)
    {
        fprintf(stderr, "Error: Team '%s' not found in the tournament.\n", teamName);
        return false;
    }

    // Shift teams left to fill the gap
    for (int i = teamIndex; i < tournament->numTeams - 1; i++)
    {
        tournament->teams[i] = tournament->teams[i + 1];
    }

    // Decrement our counter of the number of teams left in the tournament
    tournament->numTeams--;

    return true;
}

// Seed/draw teams into the tournament bracket
bool drawTournament(Tournament* tournament)
{
    // NULL check tournament
    if (tournament == NULL)
    {
        fprintf(stderr, "Error: Cannot draw NULL tournament.\n");
        return false;
    }
    
    // Need at least 2 teams to create a tournament
    if (tournament->numTeams < 2)
    {
        fprintf(stderr, "Error: Need at least 2 teams to draw a tournament.\n");
        return false;
    }

    // Clean up existing bracket if there is one
    destroyBracket(tournament);

    /*  Calculate number of rounds needed...
        For a knockout tournament, we need log2(numTeams) rounds, rounded up */
    tournament->numRounds = (int)ceil(log2(tournament->numTeams));
    if (tournament->numRounds > MAX_ROUNDS) { tournament->numRounds = MAX_ROUNDS; }

    // Reset tournament state
    tournament->currentRound = 0;
    tournament->winner = NULL;
    tournament->isComplete = false;

    // Allocate memory for the bracket & NULL check
    tournament->bracket = (Match***)malloc(tournament->numRounds * sizeof(Match**));
    if (tournament->bracket == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for tournament bracket.\n");
        return false;
    }

    // Allocate memory for matchesPerRound and NULL check
    tournament->matchesPerRound = (int*)malloc(tournament->numRounds * sizeof(int));
    if (tournament->matchesPerRound == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for matchesPerRound.\n");
        free(tournament->bracket);
        tournament->bracket = NULL;
        return false;
    }

    // First round may have fewer matches due to byes
    tournament->matchesPerRound[0] = tournament->numTeams / 2;
    
    // Subsequent rounds have half as many matches as the previous round
    for (int i = 1; i < tournament->numRounds; i++)
    {
        tournament->matchesPerRound[i] = tournament->matchesPerRound[i - 1] / 2;
    }

    // Shuffle teams for random draw Fisher-Yates shuffle algorithm
    for (int i = tournament->numTeams - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Team* temp = tournament->teams[i];
        tournament->teams[i] = tournament->teams[j];
        tournament->teams[j] = temp;
    }

    // Allocate memory for the first round matches & NULL check
    tournament->bracket[0] = (Match**)malloc(tournament->matchesPerRound[0] * sizeof(Match*));
    if (tournament->bracket[0] == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for first round matches.\n");
        free(tournament->matchesPerRound);
        free(tournament->bracket);
        tournament->bracket = NULL;
        tournament->matchesPerRound = NULL;
        return false;
    }

    // Create matches for first round
    int teamIndex = 0;
    for (int i = 0; i < tournament->matchesPerRound[0]; i++)
    {
        char dateStr[20];
        sprintf(dateStr, "R1-M%d", i + 1);
        
        // Create match
        Match* match =  createMatch(tournament->teams[teamIndex], 
                                    tournament->teams[teamIndex + 1], 
                                    dateStr);
        if (match == NULL)
        {
            fprintf(stderr, "Error: Failed to create match for first round.\n");
            // Clean up
            for (int j = 0; j < i; j++)
            {
                destroyMatch(tournament->bracket[0][j]);
            }
            free(tournament->bracket[0]);
            free(tournament->matchesPerRound);
            free(tournament->bracket);
            tournament->bracket = NULL;
            tournament->matchesPerRound = NULL;
            return false;
        }
        
        tournament->bracket[0][i] = match;
        teamIndex += 2;
    }

    // Allocate memory for subsequent rounds & NULL check
    for (int round = 1; round < tournament->numRounds; round++)
    {
        tournament->bracket[round] = (Match**)malloc(tournament->matchesPerRound[round] * sizeof(Match*));
        if (tournament->bracket[round] == NULL)
        {
            fprintf(stderr, "Error: Failed to allocate memory for round %d matches.\n", round+1);
            // Look at each round in the tournament up to the current one
            for (int i = 0; i < round; i++)
            {
                // Free each match in the round
                for (int j = 0; j < tournament->matchesPerRound[i]; j++)
                {
                    destroyMatch(tournament->bracket[i][j]);
                }
                // Free the ith round of the bracket
                free(tournament->bracket[i]);
            }

            // Free the bracket and the matches per round array
            free(tournament->matchesPerRound);
            free(tournament->bracket);
            tournament->bracket = NULL;
            tournament->matchesPerRound = NULL;
            return false;
        }
        
        // Initialize pointers to NULL
        for (int i = 0; i < tournament->matchesPerRound[round]; i++)
        {
            tournament->bracket[round][i] = NULL;
        }
    }

    return true;
}

// Simulate a specific round of the tournament
bool simulateTournamentRound(Tournament* tournament, int round)
{
    // Validate input
    if (tournament == NULL) {
        fprintf(stderr, "Error: Cannot simulate round for NULL tournament.\n");
        return false;
    } else if (round < 0 || round >= tournament->numRounds) {
        fprintf(stderr, "Error: Invalid round %d. Valid range is 0-%d.\n", 
                round, tournament->numRounds - 1);
        return false;
    } else if (round != tournament->currentRound) {
        fprintf(stderr, "Error: Can only simulate current round (%d).\n", tournament->currentRound);
        return false;
    }

    fprintf(stdout, "Simulating %s of %s...\n", getRoundName(round, tournament->numTeams), tournament->name);

    // Simulate all matches for the current round
    for (int i = 0; i < tournament->matchesPerRound[round]; i++)
    {
        Match* match = tournament->bracket[round][i];
        
        // Skip already played matches
        if (match->isCompleted)
        {
            fprintf(stdout, "Match between %s and %s was already played.\n", 
                    match->homeTeam->name, match->awayTeam->name);
            continue;
        }
        
        // Simulate the match
        simulateMatch(match);
        
        // Print the result
        fprintf(stdout, "  ");
        printMatchResult(match);
    }

    // If this is not the final round, create matches for the next round
    if (round < tournament->numRounds - 1)
    {
        // For each new match in the next round of the bracket
        for (int i = 0; i < tournament->matchesPerRound[round + 1]; i++)
        {
            // Get the winners of the two previous matches
            Team* team1 = NULL;
            Team* team2 = NULL;
            
            // First match index
            int match1Index = i * 2;
            if (match1Index < tournament->matchesPerRound[round])
            {
                // Get match 1
                Match* match1 = tournament->bracket[round][match1Index];

                // If the home team won
                if (getMatchWinner(match1) == 0){
                    team1 = match1->homeTeam;
                } 
                // Away team won
                else {
                    team1 = match1->awayTeam;
                }
            }
            
            // Second match index
            int match2Index = i * 2 + 1;
            if (match2Index < tournament->matchesPerRound[round])
            {
                // Get match 2
                Match* match2 = tournament->bracket[round][match2Index];

                // If the home team won
                if (getMatchWinner(match2) == 0) {
                    team2 = match2->homeTeam;
                } 
                // Away team won
                else {
                    team2 = match2->awayTeam;
                }
            }
            
            // Create the next round match if both teams are determined
            if (team1 != NULL && team2 != NULL)
            {
                // Store the match info/date
                char dateStr[20];
                sprintf(dateStr, "R%d-M%d", round+2, i+1);
                
                // Create match & NULL check
                Match* match = createMatch(team1, team2, dateStr);
                if (match == NULL)
                {
                    fprintf(stderr, "Error: Failed to create match for round %d.\n", round+2);
                    return false;
                }
                
                // Set the match in the next round of the bracket
                tournament->bracket[round+1][i] = match;
            }
        }
    } 
    
    // If this is the final rounf
    else 
    {
        // Determine the winner
        Match* finalMatch = tournament->bracket[round][0];

        // If the home team won the final
        if (getMatchWinner(finalMatch) == 0) {
            tournament->winner = finalMatch->homeTeam;
        } 
        // The away team won the final
        else {
            tournament->winner = finalMatch->awayTeam;
        }

        // Mark the tournament as officially completed
        tournament->isComplete = true;
        
        // Print results of the tournament
        fprintf(stdout, "\n%s tournament Complete! %s is the winner!\n", 
            tournament->name, tournament->winner->name);
    }

    // Advance to next round
    tournament->currentRound++;

    return true;
}

// Simulate the entire tournament from current round to final
void simulateEntireTournament(Tournament* tournament)
{
    // NULL check tournament
    if (tournament == NULL)
    {
        fprintf(stderr, "Error: Cannot simulate NULL tournament.\n");
        return;
    }

    fprintf(stdout, "Simulating entire %s tournament...\n", tournament->name);

    // Simulate each round until tournament is complete
    while (tournament->isComplete == false && 
            tournament->currentRound < tournament->numRounds)
    {
        simulateTournamentRound(tournament, tournament->currentRound);
        
        // Print results of the round
        fprintf(stdout, "\nResults of %s:\n", getRoundName(tournament->currentRound - 1, tournament->numTeams));
        printRoundResults(tournament, tournament->currentRound - 1);
        fprintf(stdout, "\n");
    }

    // Print final tournament bracket
    fprintf(stdout, "\nFinal Tournament Bracket:\n");
    printTournamentBracket(tournament);
}

// Get the winner of the tournament
Team* getTournamentWinner(const Tournament* tournament)
{
    // NULL check tournament
    if (tournament == NULL)
    {
        fprintf(stderr, "Error: Cannot get winner of NULL tournament.\n");
        return NULL;
    }

    // Make sure the tournament has been completed
    if (tournament->isComplete == false)
    {
        fprintf(stderr, "Error: Tournament is not yet complete.\n");
        return NULL;
    }

    return tournament->winner;
}

// Print the tournament bracket
void printTournamentBracket(const Tournament* tournament)
{
    // NULL check tournament
    if (tournament == NULL)
    {
        fprintf(stderr, "Error: Cannot print bracket for NULL tournament.\n");
        return;
    }

    // Print header for bracket
    fprintf(stdout, "%s Tournament Bracket:\n", tournament->name);
    fprintf(stdout, "===========================================================\n");

    // Print each round
    for (int round = 0; round < tournament->numRounds; round++)
    {
        // Print header for round
        fprintf(stdout, "%s:\n", getRoundName(round, tournament->numTeams));
        fprintf(stdout, "----------------------------------------------------------\n");

        // Print each match in the round
        for (int i = 0; i < tournament->matchesPerRound[round]; i++)
        {
            // Get the ith match in the round & NULL check
            Match* match = tournament->bracket[round][i];
            if (match == NULL)
            {
                fprintf(stdout, "Match not yet determined\n");
                continue;
            }

            // If the match hasn't been played yet
            if (match->isCompleted == false)
            {
                fprintf(stdout, "%s vs %s: Not played yet\n", 
                    match->homeTeam->name, match->awayTeam->name);
                continue;
            }

            // Print scoreboard
            fprintf(stdout, "%s %d - %d %s", 
                    match->homeTeam->name, match->homeScore, 
                    match->awayScore, match->awayTeam->name);
        
            // Indicate winner of the match
            if (match->homeScore > match->awayScore) {
                fprintf(stdout, " (Winner: %s)\n", match->homeTeam->name);
            } else if (match->homeScore < match->awayScore) {
                fprintf(stdout, " (Winner: %s)\n", match->awayTeam->name);
            } else {
                fprintf(stdout, " (Draw)\n");
            }
        }

        fprintf(stdout, "\n");
    }

    // Print winner if we know yet
    if (tournament->isComplete && tournament->winner != NULL)
    {
        fprintf(stdout, "Tournament Winner: %s\n", 
                tournament->winner->name);
    }
}

// Print the matches for a specific round
void printRoundMatches(const Tournament* tournament, int round)
{
    // Validate input
    if (tournament == NULL) {
        fprintf(stderr, "Error: Cannot print matches for NULL tournament.\n");
        return;
    } else if (round < 0 || round >= tournament->numRounds) {
        fprintf(stderr, "Error: Invalid round %d. Valid range is 0-%d.\n", 
                round, tournament->numRounds - 1);
        return;
    }

    // Print header
    fprintf(stdout, "%s Matches:\n", getRoundName(round, tournament->numTeams));
    fprintf(stdout, "----------------------------------------------------------\n");

    // Print each match in the current round
    for (int matchIndex = 0; matchIndex < tournament->matchesPerRound[round]; matchIndex++)
    {
        // Get match & NULL check
        Match* match = tournament->bracket[round][matchIndex];
        if (match != NULL) {
            fprintf(stdout, "%s vs %s\n", match->homeTeam->name, match->awayTeam->name);
        } else {
            fprintf(stdout, "Match not yet determined\n");
        }
    }
}

// Print the results for a completed round
void printRoundResults(const Tournament* tournament, int round)
{
    // Validate input
    if (tournament == NULL) {
        fprintf(stderr, "Error: Cannot print results for NULL tournament.\n");
        return;
    } else if (round < 0 || round >= tournament->numRounds) {
        fprintf(stderr, "Error: Invalid round %d. Valid range is 0-%d.\n", 
                round, tournament->numRounds - 1);
        return;
    }

    // Print header
    fprintf(stdout, "%s Results:\n", getRoundName(round, tournament->numTeams));
    fprintf(stdout, "----------------------------------------------------------\n");

    // Print each match result in the current round
    for (int matchIndex = 0; matchIndex < tournament->matchesPerRound[round]; matchIndex++)
    {
        // Get match & NULL check
        Match* match = tournament->bracket[round][matchIndex];
        if (match == NULL)
        {
            fprintf(stdout, "Match not yet determined\n");
            continue;
        }
        
        // Print information
        if (match->isCompleted) {
            // Print scoreboard
            fprintf(stdout, "%s %d - %d %s", 
                    match->homeTeam->name, match->homeScore, 
                    match->awayScore, match->awayTeam->name);
            
            // Indicate winner or draw
            if (match->homeScore > match->awayScore) {
                fprintf(stdout, " (Winner: %s)\n", match->homeTeam->name);
            } else if (match->homeScore < match->awayScore) {
                fprintf(stdout, " (Winner: %s)\n", match->awayTeam->name);
            } else {
                fprintf(stdout, " (Draw)\n");
            }
        }
        // match->isCompleted == false
        else {
            fprintf(stdout, "%s vs %s: Not played yet\n", 
                    match->homeTeam->name, match->awayTeam->name);
        }
    }
}

// Get the name of a tournament round
const char* getRoundName(int round, int totalTeams)
{
    int maxRounds = (int)ceil(log2(totalTeams)); // Total number of rounds
    int teamsInRound = 1 << (maxRounds - round); // Teams in the current round

    if (teamsInRound == 32)
    {
        return "Round of 32";
    }
    else if (teamsInRound == 16)
    {
        return "Round of 16";
    }
    else if (teamsInRound == 8)
    {
        return "Quarter Finals";
    }
    else if (teamsInRound == 4)
    {
        return "Semi Finals";
    }
    else if (teamsInRound == 2)
    {
        return "Final";
    }
    else    // round doesnt equal any of our definitions
    {
        return "Unknown Round";
    }
    
    
    
}



/* HELPER FUNCTIONS */

// Helper function to determine if a team advances to the next round
bool advanceTeam(Tournament* tournament, int round, int matchIndex)
{
    // Validate input
    if (tournament == NULL) {
        fprintf(stderr, "Error: Cannot advance team in NULL tournament.\n");
        return false;
    } if (round < 0 || round >= tournament->numRounds - 1) {
        fprintf(stderr, "Error: Invalid round %d. Valid range for advancement is 0-%d.\n", 
                round, tournament->numRounds - 2);
        return false;
    } if (matchIndex < 0 || matchIndex >= tournament->matchesPerRound[round]) {
        fprintf(stderr, "Error: Invalid match index %d for round %d.\n", 
                matchIndex, round);
        return false;
    }

    // Get match & NULL check
    Match* match = tournament->bracket[round][matchIndex];
    if (match == NULL)
    {
        fprintf(stderr, "Error: Match does not exist at round %d, index %d.\n", 
                round, matchIndex);
        return false;
    }

    // Make sure the match has been played
    if (match->isCompleted == false)
    {
        fprintf(stderr, "Error: Cannot advance team from a match that hasn't been played.\n");
        return false;
    }

    // Determine the winner
    Team* winner = NULL;
    if (match->homeScore > match->awayScore) {
        winner = match->homeTeam;
    } else if (match->homeScore < match->awayScore) {
        winner = match->awayTeam;
    } else {
        /* @note For simplicity, let's say the home team advances for now during a draw */
        winner = match->homeTeam;
    }

    // Calculate which match in the next round this team should go to
    int nextRoundMatchIndex = matchIndex / 2;

    // If the next round match doesn't exist yet, create it
    if (tournament->bracket[round + 1][nextRoundMatchIndex] == NULL)
    {
        // Get the round & match information
        char dateStr[20];
        sprintf(dateStr, "R%d-M%d", round+2, nextRoundMatchIndex+1);
        
        // Create match with winner as home team (second winner will be away team)
        Match* nextMatch = createMatch(winner, NULL, dateStr);
        if (nextMatch == NULL)
        {
            fprintf(stderr, "Error: Failed to create match for round %d.\n", round+2);
            return false;
        }
        
        tournament->bracket[round + 1][nextRoundMatchIndex] = nextMatch;
    } else {
        // If the match already exists, add this winner as the away team
        Match* nextMatch = tournament->bracket[round + 1][nextRoundMatchIndex];
        if (nextMatch->homeTeam == NULL) {
            nextMatch->homeTeam = winner;
        } else {
            nextMatch->awayTeam = winner;
        }
    }

    return true;
}

// Helper function to free the bracket from memory
void destroyBracket(Tournament* tournament)
{
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
}