/**
 * @author Javier A. Rodillas
 * @details Test program for the 'League' class
 */

#include <stdio.h>
#include "player.h"
#include "team.h"
#include "match.h"
#include "league.h"
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
    printf("Creating a test league...\n");
    League* league = createLeague("Test League", "Test Region", 20);
    
    
    
    // Create and add teams to the league
    printf("Adding teams to the league...\n");
    addTeamToLeague(league, createTestTeam("Team A", "City A", "Coach A", "Stadium A"));
    addTeamToLeague(league, createTestTeam("Team B", "City B", "Coach B", "Stadium B"));
    addTeamToLeague(league, createTestTeam("Team C", "City C", "Coach C", "Stadium C"));
    addTeamToLeague(league, createTestTeam("Team D", "City D", "Coach D", "Stadium D"));
    
    // Print the teams in the league
    printf("\n");
    printLeagueTeams(league);
    printf("\n");
    
    // Generate a schedule
    printf("Generating league schedule...\n");

    if (generateSchedule(league) == true) {
        printf("Schedule generated successfully.\n\n");
        
        // Print the complete schedule
        printLeagueSchedule(league);
        printf("\n");
        
        // Print a specific matchday
        printMatchdaySchedule(league, 0);
        printf("\n");
        
        // Simulate one matchday
        printf("Simulating first matchday...\n");
        simulateMatchday(league);
        printf("\n");
        
        // Print the results
        printMatchdayResults(league, 0);
        printf("\n");
        
        // Print the league table
        printf("League table after first matchday:\n");
        printLeagueTable(league);
        printf("\n");
        
        // Simulate the rest of the season
        printf("Simulating the rest of the season...\n");
        while (league->currentMatchday < league->numMatchdays) {
            simulateMatchday(league);
        }
        
        // Print final league table
        printf("\nFinal league table:\n");
        printLeagueTable(league);
        printf("\n");
        
        // Get team positions
        for (int i = 0; i < league->numTeams; i++) {
            Team* team = league->teams[i];
            int position = getTeamPosition(league, team);
            printf("%s finished in position %d\n", team->name, position);
        }
    } else {
        printf("Failed to generate schedule.\n");
    }
    
    // Clean up
    destroyLeague(league);
    
    printf("\nLeague test completed successfully.\n");
    return 0;
}