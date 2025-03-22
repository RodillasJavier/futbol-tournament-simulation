#include "match.h"
#include "team.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Create teams
    Team* homeTeam = createTeam("Barcelona F.C.", "Barcelona", "Hansi Flick", "Camp Nou");
    Team* awayTeam = createTeam("Real Madrid C.F.", "Madrid", "Carlo Ancelotti", "Bernabeu");
    
    // Create players
    Player* player1 = createPlayer(9, "fwd", "Lamine Yamal", 81);
    Player* player2 = createPlayer(10, "mid", "Jude Bellingham", 90);

    /* TESTING MATCH CREATION */

    // Create a match
    Match* match = createMatch(homeTeam, awayTeam, "2025-03-21");
    if (match == NULL) {
        printf("Failed to create match.\n");
        return 1;
    }

    // Print for match creation
    printf("\nMatch created: %s vs %s on %s\n", 
            match -> homeTeam -> name, 
            match -> awayTeam -> name, 
            match -> date);
    printf("\n");

    /* TESTING RECORD GOAL */

    // Record goals
    recordGoal(match, player1, HOME_TEAM, 15);
    recordGoal(match, player2, AWAY_TEAM, 30);

    /* TEST PRINT MATCH RESULT */

    // Print match result before marking completion
    printMatchResult(match);
    printf("\n");

    // Mark match as completed
    match -> isCompleted = true;

    // Print match result after marking completion
    printMatchResult(match);
    printf("\n");

    /* TEST PRINT MATCH REPORT */

    // Print match report
    printMatchReport(match);
    printf("\n");

    /* TEST UPDATE TEAM RECORDS */

    // Update team records
    updateTeamRecords(match);
    printf("\nUpdated team records.\n");

    /* USING DESTROY MATCH */

    // Destroy the match
    destroyMatch(match);
    printf("\nMatch destroyed successfully.\n");

    return 0;
}
