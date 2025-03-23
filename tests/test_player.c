/**
 * @author Javier A. Rodillas
 * @details Test file for the Player.c class.
 * 
 * @cite    Implementation of randomInt from: 
 *          https://www.quora.com/How-do-I-get-a-random-number-from-1-to-100-in-the-C-language
 */

#include "player.h"
#include <stdio.h>
#include <stdbool.h>

int main()
{
    // Create Messi
    Player* lionel_messi = createPlayer(10, "rw", "Messi", 93);

    // Print his information
    printf("Initial player information...\n");
    printPlayer(lionel_messi);
    printf("\n");

    // Record some stats
    printf("Recording 2 goals and an assist for the GOAT...\n");
    scoreGoal(lionel_messi);
    scoreGoal(lionel_messi);
    assist(lionel_messi);
    printPlayer(lionel_messi);
    printf("\n");

    // Injury + Recovery
    printf("Messi suffers an injury!\n");
    updateInjuryStatus(lionel_messi, true);
    printPlayer(lionel_messi);
    printf("\nRecovering from injury...\n");
    updateInjuryStatus(lionel_messi, false);
    printPlayer(lionel_messi);
    printf("\n");

    // Try making a copy of Messi
    printf("Cloning Messi...\n");
    Player* messi_clone = createPlayer(0, "", "", 1);
    copyPlayer(lionel_messi, messi_clone);
    printf("Done! Printing the clones information...\n");
    printPlayer(messi_clone);
    printf("\n");

    // Cleaning up
    destroyPlayer(messi_clone);
    destroyPlayer(lionel_messi);

    return 0;
}