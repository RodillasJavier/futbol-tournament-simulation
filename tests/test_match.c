/**
 * @author Javier A. Rodillas
 * @details Test file for the match.c class.
 */

 /* INCLUDE STATEMENTS */

#include "match.h"
#include "modules/match_simulation.h"
#include "team.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>



/* MAIN FUNCTION */
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
    printTeam(barcelona, false);
    printf("\n");
    printTeam(realMadrid, false);
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