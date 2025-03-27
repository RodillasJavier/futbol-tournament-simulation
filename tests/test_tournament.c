/**
 * @author Javier A. Rodillas
 * @details Test program for the 'Tournament' class
 */

#include <stdio.h>
#include "player.h"
#include "team.h"
#include "match.h"
#include "tournament.h"
#include "modules/match_simulation.h"
#include "random_utils.h"
 
 // Function to create a team with some players
Team* createTestTeam(const char* name, const char* city, const char* coach, const char* stadium) {
    Team* team = createTeam(name, city, coach, stadium);
    
    // Add a goalkeeper
    addPlayer(team, createPlayer(1, "gkp", "GK", 80 + randomInt(0, 5)));
    
    // Add defenders
    addPlayer(team, createPlayer(2, "def", "DEF1", 75 + randomInt(0, 5)));
    addPlayer(team, createPlayer(3, "def", "DEF2", 75 + randomInt(0, 5)));
    addPlayer(team, createPlayer(4, "def", "DEF3", 75 + randomInt(0, 5)));
    addPlayer(team, createPlayer(5, "def", "DEF4", 75 + randomInt(0, 5)));
    
    // Add midfielders
    addPlayer(team, createPlayer(6, "mid", "MID1", 78 + randomInt(0, 5)));
    addPlayer(team, createPlayer(7, "mid", "MID2", 78 + randomInt(0, 5)));
    addPlayer(team, createPlayer(8, "mid", "MID3", 78 + randomInt(0, 5)));
    
    // Add forwards
    addPlayer(team, createPlayer(9, "fwd", "FWD1", 80 + randomInt(0, 5)));
    addPlayer(team, createPlayer(10, "fwd", "FWD2", 80 + randomInt(0, 5)));
    addPlayer(team, createPlayer(11, "fwd", "FWD3", 80 + randomInt(0, 5)));
    
    return team;
}
 
int main() {    
    printf("Creating a test tournament...\n");
    Tournament* tournament = createTournament("Champions League");
    
    // Create and add teams to the tournament
    printf("Adding teams to the tournament...\n");
    addTeamToTournament(tournament, createTestTeam("Liverpool", "Liverpool", "Arne Slot", "Anfield"));
    addTeamToTournament(tournament, createTestTeam("F.C. Barcelona", "Barcelona", "Hansi Flick", "Camp Nou"));
    addTeamToTournament(tournament, createTestTeam("Arsenal", "London", "Mikel Arteta", "Emirates Stadium"));
    addTeamToTournament(tournament, createTestTeam("Inter Milan", "Milan", "Simone Inzaghi", "San Siro"));
    addTeamToTournament(tournament, createTestTeam("Atleti", "Madrid", "Diego Simeone", "Wanda Metropolitano"));
    addTeamToTournament(tournament, createTestTeam("Leverkusen", "Leverkusen", "Xabi Alonso", "BayArena"));
    addTeamToTournament(tournament, createTestTeam("Lille", "Lille", "Paulo Fonseca", "Stade Pierre-Mauroy"));
    addTeamToTournament(tournament, createTestTeam("Aston Villa", "Birmingham", "Unai Emery", "Villa Park"));
    addTeamToTournament(tournament, createTestTeam("Club Brugge", "Bruges", "Ronny Deila", "Jan Breydel Stadium"));
    addTeamToTournament(tournament, createTestTeam("BVB", "Dortmund", "Edin Terzić", "Signal Iduna Park"));
    addTeamToTournament(tournament, createTestTeam("Real Madrid C.F.", "Madrid", "Carlo Ancelotti", "Santiago Bernabéu"));
    addTeamToTournament(tournament, createTestTeam("Bayern Munchen", "Munich", "Thomas Tuchel", "Allianz Arena"));
    addTeamToTournament(tournament, createTestTeam("Feyenoord", "Rotterdam", "Arne Slot", "De Kuip"));
    addTeamToTournament(tournament, createTestTeam("PSV", "Eindhoven", "Peter Bosz", "Philips Stadion"));
    addTeamToTournament(tournament, createTestTeam("PSG", "Paris", "Luis Enrique", "Parc des Princes"));
    addTeamToTournament(tournament, createTestTeam("Benfica", "Lisbon", "Roger Schmidt", "Estádio da Luz"));
    
    // Draw the tournament
    printf("\nDrawing tournament bracket...\n");
    if (drawTournament(tournament)) {
        printf("Tournament drawn successfully.\n\n");
        
        // Print the initial bracket
        printTournamentBracket(tournament);
        printf("\n");
        
        // Print the first round matches
        printRoundMatches(tournament, 0);
        printf("\n");
        
        // Simulate the first round
        printf("Simulating first round...\n");
        simulateTournamentRound(tournament, 0);
        printf("\n");
        
        // Print the results
        printRoundResults(tournament, 0);
        printf("\n");
        
        // Simulate the rest of the tournament
        printf("Simulating the rest of the tournament...\n");
        while (!tournament->isComplete) {
            simulateTournamentRound(tournament, tournament->currentRound);
            printf("\n");
        }
        
        // Print final tournament bracket
        printf("\nFinal tournament bracket:\n");
        printTournamentBracket(tournament);
        printf("\n");
        
        // Get tournament winner
        Team* winner = getTournamentWinner(tournament);
        if (winner != NULL) {
            printf("Tournament winner: %s\n", winner->name);
        }
    } else {
        printf("Failed to draw tournament.\n");
    }
    
    // Clean up
    destroyTournament(tournament);
    
    printf("\nTournament test completed successfully.\n");
    return 0;
}