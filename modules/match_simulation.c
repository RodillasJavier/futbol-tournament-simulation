/**
 * @author  Javier A. Rodillas
 * @details Class implementation of the match simulation module. 
 * 
 * @cite    Implementation of randomInt from: 
 *          https://www.quora.com/How-do-I-get-a-random-number-from-1-to-100-in-the-C-language
 * @cite    Base goal scoring probability from:
 *          https://www.uefa.com/uefachampionsleague/statistics/
 */


/* INCLUDE STATEMENTS */

#include "match_simulation.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>



/* HELPER FUNCTIONS */

//  Initialize random seed if not yet done
static bool randomInitialized = false;
void initRandom()
{
    if (randomInitialized == false)
    {
        srand((unsigned int)time(NULL));
        randomInitialized = true;
    }
}

// Generate a random number (double) between 0 and 1
double randomProbability()
{
    // Ensure random seed is initialized
    initRandom();

    // Generate a number b/w (0 & RAND_MAX) and div by RAND_MAX ~> Number b/w (0 & 1)
    return (double)rand() / RAND_MAX;
}

// Generate a random number (int) in the range (min, max) inclusive
int randomInt(int min, int max)
{
    // Ensure random seed is initialized
    initRandom();

    // Generate a number between min and max (inclusive)
    return (rand() % (max - min + 1)) + min;
}



/* FUNCTIONS */

// Simulate a complete match between two teams
void simulateMatch(Match* match)
{
    // Null check match 
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried to simulate a match that doesn't exist.\n");
        return;
    }
    
    // Make sure that the match hasn't been simulated already
    if (match -> isCompleted)
    {
        fprintf(stderr, "Error: Tried to simulate a match that has already been played.\n");
        return;
    }

    // 1st half
    int stoppageTimeFirstHalf = randomInt(0, 10);
    simulateMatchMinutes(match, 0, 45 + stoppageTimeFirstHalf);

    // 2nd half
    int stoppageTimeSecondHalf = randomInt(0, 10);
    simulateMatchMinutes(match, 45, 90 + stoppageTimeSecondHalf);

    // Any injuries that may have occurred during the match
    simulateInjuries(match);

    // Update game status
    match -> isCompleted = true;

    // Update team records
    updateTeamRecords(match);
}

// Calculate the probability of two teams scoring based on ratings
double calculateScoringProbability(Team* team, Team* opponentTeam)
{
    // Null check both teams
    if (team == NULL)
    {
        fprintf(stderr, "Error: Tried calculating scoring probabilities with NULL team.\n");
        return 0.0;
    } else if (opponentTeam == NULL)
    {
        fprintf(stderr, "Error: Tried calculating scoring probabilities with NULL opposing team.\n");
        return 0.0;
    }

    // Set base scoring probability
    double scoringProbability = 0.0357;

    // Get team ratings
    double teamRating = calculateTeamRating(team);
    double opponentTeamRating = calculateTeamRating(opponentTeam);

    // Calculate ratio between team ratings
    double teamRatingRatio = teamRating / opponentTeamRating;

    // Weight our new scoring probability
    double weightedScoringProbability = scoringProbability * teamRatingRatio;

    return weightedScoringProbability;
}

// Determine which player on the team scored the goal
Player* determineScorer(Team* team);

// Determine which player assisted the goal (if applicable)
Player* determineAssist(Team* team, Player* scorer);

// Simulate any potential injuries that would occur during a match
void simulateInjuries(Match* match);

// Simulate the minutes of a match, from start -> end
void simulateMatchMinutes(Match* match, int startMinute, int endMinute);
