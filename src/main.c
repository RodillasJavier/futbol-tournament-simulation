/**
 * @author Javier A. Rodillas
 * @details Test program for the 'League' class
 */



/* INCLUDE STATEMENTS & DEFINITIONS */
#include "player.h"
#include "team.h"
#include "match.h"
#include "league.h"
#include "tournament.h"
#include "random_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_LEAGUES 5
#define TEAMS_PER_LEAGUE 20
#define NUM_TOURNAMENT_TEAMS 32



/* FUNCTIONS */

// Function to create a team with randomly generated players
Team* createRandomTeam(const char* name, const char* city)
{
    // Create team & NULL check
    Team* team = createTeam(name, city, "Coach", "Stadium");
    if (team == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for a new team.\n");
        return NULL;
    }
    
    // Add goalkeeper
    addPlayer(team, createPlayer(1, "gk", "GoalKeeper", 80 + randomInt(-10, 10)));

    // Add Defenders
    for (int i = 2; i <= 5; i++)
    {
        char playerName[20];
        sprintf(playerName, "Defender%d", i - 1);
        addPlayer(team, createPlayer(i, "def", playerName, 78 + randomInt(-10, 10)));
    }

    // Add Midfielders
    for (int i = 6; i <= 8; i++)
    {
        char playerName[20];
        sprintf(playerName, "Midfielder%d", i - 5);
        addPlayer(team, createPlayer(i, "mid", playerName, 79 + randomInt(-10, 10)));
    }

    // Add Forwards
    for (int i = 9; i <= 11; i++)
    {
        char playerName[20];
        sprintf(playerName, "Forward%d", i - 8);
        addPlayer(team, createPlayer(i, "fwd", playerName, 80 + randomInt(-10, 10)));
    }

    return team;
}

// Function to simulate a league season
void simulateLeagueSeason(League* league)
{
    // Validate input
    if (league == NULL)
    {
        fprintf(stderr, "Error: Tried simulating season of a NULL league.\n");
        return;
    }

    // Generate a schedule for the league
    if (generateSchedule(league))
    {
        // Simulate season
        simulateSeason(league);
        fprintf(stdout, "\n");
    }
    
}

// Function to simulate a tournament
void simulateTournament(Tournament* tournament)
{
    // Validate input
    if (tournament == NULL)
    {
        fprintf(stderr, "Error: tried simulating a NULL tournament.\n");
    }

    // Make drawings for the tournament
    if (drawTournament(tournament))
    {
        // Simulate tournament
        simulateEntireTournament(tournament);

        // Get results and print them
        Team* winner = getTournamentWinner(tournament);
        if (winner != NULL)
        {
            fprintf(stdout, "\n%s Winner: %s\n", tournament->name, winner->name);
        } else {
            fprintf(stderr, "\nError: Could not find a winner for the %s.\n", tournament->name);
        }
    }
}

/* MAIN FUNCTION */

int main() {
    // Create and simulate leagues
    League* leagues[NUM_LEAGUES];
    const char* leagueNames[] = {   "La Liga",  "Premier League",   "Bundesliga",   "Serie A",  "Ligue 1"};
    const char* leagueRegions[] = { "Spain",    "England",          "Germany",      "Italy",    "France"};

    // Create and simulate leagues
    fprintf(stdout, "Creating Leagues + Simulating their seasons...\n");
    for (int leagueIndex = 0; leagueIndex < NUM_LEAGUES; leagueIndex++)
    {
        // Create league
        leagues[leagueIndex] = createLeague(leagueNames[leagueIndex], leagueRegions[leagueIndex], TEAMS_PER_LEAGUE);

        // Add teams to league
        for (int teamIndex = 0; teamIndex < TEAMS_PER_LEAGUE; teamIndex++)
        {
            // Store name of team + city
            char teamName[50], cityName[50];
            sprintf(teamName, "%s Team %d", leagueNames[leagueIndex], teamIndex + 1);
            sprintf(cityName, "City %d", teamIndex + 1);

            // Create and add a new team to this league
            addTeamToLeague(leagues[leagueIndex], createRandomTeam(teamName, cityName));
        }
        simulateLeagueSeason(leagues[leagueIndex]);
    }
    
    // Create and fill UCL
    fprintf(stdout, "\nCreating UCL tournament...\n");
    Tournament* champions = createTournament("Champions League");

    // Take top teams from each league ~ 3 * 5 = 15
    for (int leagueIndex = 0; leagueIndex < NUM_LEAGUES; leagueIndex++)
    {
        fprintf(stdout, "\nSelcecting teams from %s\n", leagues[leagueIndex]->name);

        // Selecting top teams from each league
        for (int teamIndex = 0; teamIndex < 3; teamIndex++)
        {
            // Get teak ranking from 1st - 3rd
            int teamRank = leagues[leagueIndex]->leagueTable[teamIndex][0];
            // Get actual team with that ranking from the league table
            Team* qualifiedTeam = leagues[leagueIndex]->teams[teamRank];

            // Print qualifiers
            fprintf(stdout, "\n%s qualified\n", qualifiedTeam->name);
            addTeamToTournament(champions, qualifiedTeam);
        }
    }

    // add barça hehehe
    Team* Barca = createTeam("F.C. Barcelona", "Barcelona", "Hansi Flick", "Camp Nou");
    addPlayer(Barca, createPlayer(25, "gk", "Wojciech Szczęsny", 91));
    addPlayer(Barca, createPlayer(3, "def", "Alejandro Balde", 85));
    addPlayer(Barca, createPlayer(5, "def", "Iñigo Martinez", 88));
    addPlayer(Barca, createPlayer(2, "def", "Pau Cubarsí", 90));
    addPlayer(Barca, createPlayer(23, "def", "Jules Koundé", 89));
    addPlayer(Barca, createPlayer(8, "mid", "Pedri", 90));
    addPlayer(Barca, createPlayer(21, "mid", "Frenkie de Jong", 91));
    addPlayer(Barca, createPlayer(20, "mid", "Dani Olmo", 88));
    addPlayer(Barca, createPlayer(11, "fwd", "Raphinha", 90));
    addPlayer(Barca, createPlayer(9, "fwd", "Robert Lewandowski", 92));
    addPlayer(Barca, createPlayer(10, "fwd", "Lamine Yamal", 86));
    addTeamToTournament(champions, Barca);
    
    // Simulate UCL
    simulateTournament(champions);

    // Cleanup
    destroyTournament(champions);
    for (int leagueIndex = 0; leagueIndex < NUM_LEAGUES; leagueIndex++)
    {
        destroyLeague(leagues[leagueIndex]);
    }
    
    fprintf(stdout, "\nsimulation completed successfully\n");
    return 0;
}