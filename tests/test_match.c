/**
 * @author Javier A. Rodillas
 * @details Test file for the match.c class.
 */

#include "match.h"
#include "match_simulation.h"
#include "team.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

// int main() {
//     // Create teams
//     Team* homeTeam = createTeam("Barcelona F.C.", "Barcelona", "Hansi Flick", "Camp Nou");
//     Team* awayTeam = createTeam("Real Madrid C.F.", "Madrid", "Carlo Ancelotti", "Bernabeu");
    
//     // Create players
//     Player* player1 = createPlayer(9, "fwd", "Lamine Yamal", 81);
//     Player* player2 = createPlayer(10, "mid", "Jude Bellingham", 90);

//     // Add them to the roster
//     addPlayer(homeTeam, player1);
//     addPlayer(awayTeam, player2);

//     /* TESTING MATCH CREATION */

//     // Create a match
//     Match* match = createMatch(homeTeam, awayTeam, "2025-03-21");
//     if (match == NULL) {
//         fprintf(stdout, "Failed to create match.\n");
//         return 1;
//     }

//     // Print for match creation
//     fprintf(stdout, "\nMatch created: %s vs %s on %s\n", 
//             match -> homeTeam -> name, 
//             match -> awayTeam -> name, 
//             match -> date);
//     fprintf(stdout, "\n");

//     /* TESTING RECORD GOAL */

//     // Record goals
//     recordGoal(match, player1, HOME_TEAM, 15);
//     recordGoal(match, player2, AWAY_TEAM, 30);

//     /* TEST PRINT MATCH RESULT */

//     // Print match result before marking completion
//     printMatchResult(match);
//     fprintf(stdout, "\n");

//     // Mark match as completed
//     match -> isCompleted = true;

//     // Print match result after marking completion
//     printMatchResult(match);
//     fprintf(stdout, "\n");

//     /* TEST PRINT MATCH REPORT */

//     // Print match report
//     printMatchReport(match);
//     fprintf(stdout, "\n");

//     /* TEST UPDATE TEAM RECORDS */

//     // Update team records
//     updateTeamRecords(match);
//     fprintf(stdout, "\nUpdated team records.\n");
//     printTeam(homeTeam, true);
//     printTeam(awayTeam, true);

//     /* USING DESTROY MATCH */

//     // Destroy the match
//     destroyMatch(match);
//     fprintf(stdout, "\nMatch destroyed successfully.\n");

//     return 0;
// }

int main() {
    // Create two teams
    Team* barcelona = createTeam("Barcelona", "Barcelona", "Xavi", "Camp Nou");
    Team* realMadrid = createTeam("Real Madrid", "Madrid", "Ancelotti", "Santiago Bernabeu");
    
    // Add players to Barcelona
    addPlayer(barcelona, createPlayer(1, "gkp", "Marc-Andre ter Stegen", 88));
    addPlayer(barcelona, createPlayer(3, "def", "Gerard Pique", 85));
    addPlayer(barcelona, createPlayer(18, "def", "Jordi Alba", 86));
    addPlayer(barcelona, createPlayer(5, "mid", "Sergio Busquets", 87));
    addPlayer(barcelona, createPlayer(8, "mid", "Pedri", 85));
    addPlayer(barcelona, createPlayer(10, "fwd", "Lionel Messi", 94));
    addPlayer(barcelona, createPlayer(9, "fwd", "Memphis Depay", 84));
    
    // Add players to Real Madrid
    addPlayer(realMadrid, createPlayer(1, "gkp", "Thibaut Courtois", 89));
    addPlayer(realMadrid, createPlayer(4, "def", "Sergio Ramos", 88));
    addPlayer(realMadrid, createPlayer(12, "def", "Marcelo", 85));
    addPlayer(realMadrid, createPlayer(8, "mid", "Toni Kroos", 88));
    addPlayer(realMadrid, createPlayer(10, "mid", "Luka Modric", 87));
    addPlayer(realMadrid, createPlayer(9, "fwd", "Karim Benzema", 89));
    addPlayer(realMadrid, createPlayer(7, "fwd", "Eden Hazard", 85));
    
    // Print team information before the match
    printf("===== TEAMS BEFORE MATCH =====\n\n");
    printTeam(barcelona, true);
    printf("\n");
    printTeam(realMadrid, true);
    printf("\n");
    
    // Create a match
    Match* elClasico = createMatch(barcelona, realMadrid, "2023-11-01");
    
    // Simulate the match
    printf("===== SIMULATING EL CLASICO =====\n\n");
    simulateMatch(elClasico);
    
    // Print match result
    printf("===== MATCH RESULT =====\n\n");
    printMatchReport(elClasico);
    printf("\n");
    
    // Print team information after the match
    printf("===== TEAMS AFTER MATCH =====\n\n");
    printTeam(barcelona, true);
    printf("\n");
    printTeam(realMadrid, true);
    printf("\n");
    
    // Clean up
    destroyMatch(elClasico);
    destroyTeam(barcelona);
    destroyTeam(realMadrid);
    
    return 0;
}