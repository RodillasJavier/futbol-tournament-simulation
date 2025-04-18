/**
 * @author Javier A. Rodillas
 * @details Implementation for the 'Team' class
 */


 
/* INCLUDE STATEMENTS */

#include "team.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* FUNCTIONS */

// Create a new team
Team* createTeam(const char* name, const char* city, const char* coach, const char* homeStadium)
{
    // Allocate memory for a Team
    Team* team = (Team*)malloc(sizeof(Team));

    // Null checking team
    if (team == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for a team.\n");
        return NULL;
    }

    // Initialize team attributes:

    // Name of the club
    strncpy(team -> name, name, sizeof(team -> name) - 1);
    team -> name[sizeof(team -> name) - 1] = '\0';  // Ensure null termination

    // City the club is based in
    strncpy(team -> city, city, sizeof(team -> city) - 1);
    team -> city[sizeof(team -> city) - 1] = '\0';  // Ensure null termination

    // Name of the head coach
    strncpy(team -> coach, coach, sizeof(team -> coach) - 1);
    team -> coach[sizeof(team -> coach) - 1] = '\0';    // Ensure null termination

    // Name of the home stadium
    strncpy(team -> homeStadium, homeStadium, sizeof(team -> homeStadium) - 1);
    team -> homeStadium[sizeof(team -> homeStadium) - 1] = '\0';    // Ensure null termination

    // Team Roster
    team -> maxPlayers = 25;    // Default max roster size of 25
    team -> numPlayers = 0;
    team -> players = (Player**)malloc(sizeof(Player*) * team -> maxPlayers);

    // Rest of the fields & their default values
    team -> rating = 0.0;
    team -> schedule = NULL;
    team -> scheduleLength = 0;

    team -> wins = 0;
    team -> losses = 0;
    team -> draws = 0;

    team -> goalsScored = 0;
    team -> goalsConceded = 0;
    team -> goalDifferential = 0;

    team -> points = 0;

    return team;
}

// Destroy & free all memory allocated for our team
void destroyTeam(Team* team)
{
    // Null checking team
    if (team != NULL)
    {
        // Free the roster if it exists
        if (team -> players != NULL)
        {
            // Free each individual player on the roster
            for (int i = 0; i < team -> numPlayers; i++)
            {
                destroyPlayer(team -> players[i]);
            }

            // Free the array containing the roster
            free(team -> players);
        }

        // Free the schedule if it exists
        if (team -> schedule != NULL)
        {
            free(team -> schedule);
        }
        
        // Free the team itself now
        free(team);
    }
    
}

// Add a player to a team
bool addPlayer(Team* team, Player* player)
{
    // Null check if the team exists
    if (team == NULL)
    {
        fprintf(stderr, "Error: Trying to add a player from a team that does not exist.\n");
        return false;
    }

    // Null check if the player exists
    if (player == NULL)
    {
        fprintf(stderr, "Error: Trying to add a player that does not exist.\n");
        return false;
    }

    // Check to see if the roster is already full or not
    if (team -> numPlayers >= team -> maxPlayers)
    {
        fprintf(stderr, "Error: %s's roster is already full! %d/%d players on the team.\n", 
                team -> name, team -> numPlayers, team -> maxPlayers);
        return false;
    }
    
    // Check to see if a player on the roster with that number already exists or not
    for (int i = 0; i < team -> numPlayers; i++)
    {
        if (team -> players[i] -> number == player -> number)
        {
            fprintf(stderr, "Error: %s's number is already taken by %s on %s!\n", 
                    player -> name, team -> players[i] -> name, team -> name);
            return false;
        }
    }

    // Add player to roster and increment roster size
    team -> players[team -> numPlayers] = player;
    team -> numPlayers++;

    // Calculate new team rating
    team -> rating = calculateTeamRating(team);
    
    return true;
}

// Remove a specific player from a team, searching for them by number
bool removePlayerByNumber(Team* team, int number)
{
    // Null check team
    if (team == NULL)
    {
        fprintf(stderr, "Error: Trying to add a player from a team that does not exist.\n");
        return false;
    }

    // Find player with given number
    int playerIndex = -1;
    for (int i = 0; i < team -> numPlayers; i++)
    {
        // If we find a player with the given number
        if (team -> players[i] -> number == number)
        {
            // Remember the index that we found him at
            playerIndex = i;
            break;
        }
    }
    
    // If there is no player with the number, give error
    if (playerIndex < 0)
    {
        fprintf(stderr, "Error: There was no player found with the number %d on %s.\n", 
                number, team -> name);
        return false;
    }
    
    // Free the OLD player with the number
    destroyPlayer(team -> players[playerIndex]);

    // Shift players in roster to fill the gap
    for (int i = playerIndex; i < team -> numPlayers - 1; i++)
    {
        team -> players[i] = team -> players[i + 1];
    }

    // Update the number of players on the team
    team -> numPlayers--;

    // Re-calculate the team rating
    team -> rating = calculateTeamRating(team);

    return true;
}

// Remove a specific player from a team, searching for them by name
bool removePlayerByName(Team* team, char* name)
{
    // Null check team
    if (team == NULL)
    {
        fprintf(stderr, "Error: Trying to add a player from a team that does not exist.\n");
        return false;
    }
    

    // Find player with given name
    int playerIndex = -1;
    for (int i = 0; i < team -> numPlayers; i++)
    {
        int isTheSame = -1;
        isTheSame = strcmp(team -> players[i] -> name, name);
        // If we find a player with the given name
        if (isTheSame == 0)
        {
            // Remember the index that we found him at
            playerIndex = i;
            break;
        }
    }
    
    // If there is no player with the given name, give error
    if (playerIndex < 0)
    {
        fprintf(stderr, "Error: There was no player found with the name %s on %s.\n", 
            name, team -> name);
        return false;
    }
    
    // Free the OLD player with the number
    destroyPlayer(team -> players[playerIndex]);

    // Shift players in roster to fill the gap
    for (int i = playerIndex; i < team -> numPlayers - 1; i++)
    {
        team -> players[i] = team -> players[i + 1];
    }
    
    // Update the number of players on the team
    team -> numPlayers--;

    // Re-calculate the team rating
    team -> rating = calculateTeamRating(team);

    return true;
}

// Get a player by their jersey number
Player* getPlayerByNumber(const Team* team, int number)
{
    // Null check team
    if (team == NULL)
    {
        fprintf(stderr, "Error: Trying to add a player from a team that doesn't exist.\n");
        return NULL;
    }

    // Find player w/given jersey number
    for (int i = 0; i < team -> numPlayers; i++)
    {
        // If we find a player with the given number
        if (team -> players[i] -> number == number)
        {
            return team -> players[i];
        }
    }

    // Player not found
    return NULL;
}

// Calculate the teams overall rating (based on team average)
float calculateTeamRating(Team* team)
{
    // Null check team
    if (team == NULL)
    {
        fprintf(stderr, "Error: Tried to calculate rating of a team that doesn't exist.\n");
        return 0.0;
    }

    // Check for if/when a team has 0 players (avoid division by 0)
    if (team -> numPlayers == 0)
    {
        return 0.0;
    }
    

    // Sum up total ratings
    int total_ratings = 0;
    for (int i = 0; i < team -> numPlayers; i++)
    {
        total_ratings += team -> players[i] -> rating;
    }

    // Divide by the total number of players
    return (float)total_ratings / team -> numPlayers;
}

// Update the record & points of a team
void updateRecord(Team* team, bool win, bool loss, bool draw)
{
    // Null check team
    if (team == NULL)
    {
        fprintf(stderr, "Error: Tried updating the record of a team that doesn't exist.\n");
        return;
    }
    
    // Verify that only 1 out of 3 of the booleans are true
    int boolean_count = 0;
    if (win == true) { boolean_count++; }
    if (loss == true) { boolean_count++; }
    if (draw == true) { boolean_count++; }
    // Either no result recorded or multiple results recorded for one game
    if (boolean_count > 1 || boolean_count == 0)
    {
        fprintf(stderr, "Error: Team either recorded MULTIPLE outcomes for a game (i.e. w + l, w + d, d + l), or NO outcome for a game.\n");
        return;
    }

    // Increment the count of whichever boolean is true
    if (win) { team -> wins++; }
    else if (loss) { team -> losses++; }
    else if (draw) { team -> draws++; }
}

// Update the goal differential, goals scored & goals conceded
void updateGoals(Team* team, int goalsScored, int goalsConceded)
{
    // Null check team
    if (team == NULL)
    {
        fprintf(stderr, "Error: Tried to calculate goal differential for a team that doesn't exist.\n");
        return;
    }

    // Make sure goalsScored & goalsConceded are both non-negative
    if (goalsScored < 0) {
        fprintf(stderr, "Error: Tried to calculate goal differential with NEGATIVE goals scored.\n");
        return;
    } else if (goalsConceded < 0) {
        fprintf(stderr, "Error: Tried to calculate goal differential with NEGATIVE goals conceded.\n");
        return;
    }

    // Update goals scored & goals conceded by the team
    team -> goalsScored += goalsScored;
    team -> goalsConceded += goalsConceded;

    // Calculate goal differential
    team -> goalDifferential = team -> goalsScored - team -> goalsConceded;
}

// Update the points of a team
void updatePoints(Team* team)
{
    // Null check team
    if (team == NULL)
    {
        fprintf(stderr, "Error: Tried updating the record of a team that doesn't exist.\n");
        return;
    }

    // 3 points for a win; 1 point for a draw; no points awarded for a loss
    team -> points = (team -> wins * 3) + (team -> draws);
}

// Print the information of a Team
void printTeam(const Team* team, bool showPlayers)
{
    // Null check team
    if (team == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for a player.\n");
        return;
    }
    
    // Name, city, coach & stadium
    fprintf(stdout, "===== %s (%s) =====\n", 
            team->name, team->city);
    fprintf(stdout, "Coach: %s\n", team->coach);
    fprintf(stdout, "Home Stadium: %s\n", team->homeStadium);

    // Rating, record & points
    fprintf(stdout, "Team Rating: %.2f\n", team->rating);
    fprintf(stdout, 
            "Record: %d-%d-%d (W-L-D)\n", 
            team->wins, team->losses, team->draws);
    fprintf(stdout, "Points: %d\n", team->points);
    
    // Goals
    fprintf(stdout, "Goals: %d scored, %d conceded (Differential: %d)\n", 
           team->goalsScored, team->goalsConceded, team->goalDifferential);
    
    // Roster
    fprintf(stdout, 
            "Players: %d/%d\n", 
            team -> numPlayers, team -> maxPlayers);
    
    // OPTIONAL: Print the roster
    if (showPlayers && team -> numPlayers > 0) {
        fprintf(stdout, "\nRoster:\n");
        for (int i = 0; i < team -> numPlayers; i++) {
            fprintf(stdout, "Player %d: ", i + 1);
            printPlayer(team -> players[i]);
        }
    }
    
    fprintf(stdout, "\n");
}