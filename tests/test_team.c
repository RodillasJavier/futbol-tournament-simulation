/**
 * @author Javier A. Rodillas
 * @details Test file for the Player.c class.
 */

#include "team.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Create a team
    printf("Creating team...\n");
    Team* barca = createTeam("Barcelona FC", "Barcelona", "Hansi Flick", "Camp Nou");

    // Create some players
    printf("\nCreating some players...\n");
    Player* yamal = createPlayer(19, "rw", "Lamine Yamal", 81);
    Player* raphinha = createPlayer(11, "rw", "Raphael Dias Belloli", 84);
    Player* lewandowski = createPlayer(9, "st", "Robert Lewandowski", 88);
    Player* ter_stegen = createPlayer(1, "gk", "Marc-André ter Stegen", 89);

    // Add players to the team
    printf("\nAdding players to the team...\n");
    addPlayer(barca, yamal);
    addPlayer(barca, raphinha);
    addPlayer(barca, lewandowski);
    addPlayer(barca, ter_stegen);

    // Print team information
    printf("\nPrinting team information...\n");
    printTeam(barca, true);

    // Test get player by number
    printf("\nGetting player with number 10...\n");
    Player* foundPlayer1 = getPlayerByNumber(barca, 10);
    if (foundPlayer1 != NULL)
    {
        printPlayer(foundPlayer1);
    } else {
        printf("Could not find player with the number 10\n");
        destroyPlayer(foundPlayer1);
    }

    printf("\nGetting player with number 19...\n");
    Player* foundPlayer2 = getPlayerByNumber(barca, 19);
    if (foundPlayer2 != NULL)
    {
        printPlayer(foundPlayer2);
    } else {
        printf("Could not find player with the number 19\n");
        destroyPlayer(foundPlayer2);
    }

    // Test update record
    printf("\nUpdating team record (win)...\n");
    updateRecord(barca, true, false, false);
    updateGoals(barca, 3, 1);
    printf("Team record after win: %d-%d-%d (W-L-D), Points: %d\n", 
        barca->wins, barca->losses, barca->draws, barca->points);
    
    printf("\nUpdating team record (draw)...\n");
    updateRecord(barca, false, false, true);
    updateGoals(barca, 2, 2);
    printf("Team record after draw: %d-%d-%d (W-L-D), Points: %d\n", 
        barca->wins, barca->losses, barca->draws, barca->points);
    
    printf("\nUpdating team record (loss)...\n");
    updateRecord(barca, false, true, false);
    updateGoals(barca, 0, 1);
    printf("Team record after loss: %d-%d-%d (W-L-D), Points: %d\n", 
        barca->wins, barca->losses, barca->draws, barca->points);

    // Test removing a player by their number
    printf("\nRemoving a player with number 19 (seniority)...\n");
    removePlayerByNumber(barca, 19);
    printf("\nTeam after removal...\n");
    printTeam(barca, true);

    // Test removing a player by their number
    printf("\nRemoving a player with name Raphael Dias Belloli (seniority)...\n");
    removePlayerByName(barca, raphinha -> name);
    printf("\nTeam after removal...\n");
    printTeam(barca, true);

    // Clean up
    destroyTeam(barca);

    printf("\nTest completed successfully.\n");
    return 0;
}