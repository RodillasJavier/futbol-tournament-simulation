/**
 * @author  Javier A. Rodillas
 * @details Class implementation of the match simulation module. 
 * 
 * @cite    Implementation of randomInt from: 
 *          https://www.quora.com/How-do-I-get-a-random-number-from-1-to-100-in-the-C-language
 * @cite    Base goal scoring probability from:
 *          https://www.uefa.com/uefachampionsleague/statistics/
 * @cite    Weighted probability selection:
 *          https://www.reddit.com/r/C_Programming/comments/pljqdc/how_do_i_do_a_weighted_random_generator/
 */



/* INCLUDE STATEMENTS */

#include "match_simulation.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>



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
Player* determineScorer(Team* team)
{
    // Validate input
    if (team == NULL)
    {
        fprintf(stderr, "Error: Tried finding scorer for a NULL team.\n");
        return NULL;
    } 
    else if (team -> numPlayers <= 0)
    {
        fprintf(stderr, "Error: Tried finding scorer for a team with %d players.\n", 
        team -> numPlayers);
        return NULL;
    }

    // Allocate memory for weights
    double* weights = (double*)malloc(sizeof(double) * team -> numPlayers);
    double totalWeight = 0.0;

    // Null check weights
    if (weights == NULL)
    {
        fprintf(stderr, "Error: Failed allocating memory for weights array.\n");
        return NULL;
    }

    // Assign weights based on player attributes
    for (int i = 0; i < team -> numPlayers; i++)
    {
        // Pick a player off of the roster and assign their initial weight
        Player* player = team -> players[i];
        double weight = player -> rating;

        // Adjust weights based on their positions
        if (strcmp(player -> position, "fwd") == 0) { weight *= 2.0; }          // Forwards
        else if (strcmp(player -> position, "mid") == 0) { weight *= 1.25; }    // Midfielders
        else if (strcmp(player -> position, "def") == 0) { weight *= 0.75; }    // Defenders
        else if (strcmp(player -> position, "gkp") == 0) { weight *= 0.01; }    // Goal Keepers

        // Exclude injured players
        if (player -> injuryStatus == true) { weight = 0.0; }

        // Store weights and compute total
        weights[i] = weight;
        totalWeight += weight;
    }

    // If no eligible scorers (all injured)
    if (totalWeight <= 0.0)
    {
        free(weights);
        return NULL;
    }

    // Select player based on weighted probability
    double randomValue = randomProbability() * totalWeight;
    double cumulativeWeight = 0.0;

    for (int i = 0; i < team -> numPlayers; i++)
    {
        cumulativeWeight += weights[i];
        if (randomValue <= cumulativeWeight)
        {
            Player* scorer = team -> players[i];
            free(weights);
            return scorer;
        }   
    }

    // Fallback, if no player is selected just select whoever is not injured
    free(weights);

    for (int i = 0; i < team -> numPlayers; i++)
    {
        if (team -> players[i] -> injuryStatus == false)
        {
            return team -> players[i];
        }
    }

    fprintf(stderr, "Error: Could not find a player on %s who could've scored a goal.\n", 
            team -> name);
    return NULL;
}

// Determine which player assisted the goal (if applicable)
Player* determineAssist(Team* team, Player* scorer)
{
    // Validate input
    if (team == NULL)
    {
        fprintf(stderr, "Error: Tried finding assister for a NULL team.\n");
        return NULL;
    } 
    else if (team -> numPlayers <= 0)
    {
        fprintf(stderr, "Error: Tried finding assister for a team with %d players.\n", 
        team -> numPlayers);
        return NULL;
    }

    // 30% chance of no assist
    if (randomProbability() < 0.3)
    {
        return NULL;
    }

    // Allocate memory for weighted probabilities
    double* weights = (double*)malloc(sizeof(double) * team -> numPlayers);
    double totalWeight = 0.0;

    // Null check weights
    if (weights == NULL)
    {
        fprintf(stderr, "Error: Failed allocating memory for weights array.\n");
        return NULL;
    }

    // Assign assist weights
    for (int i = 0; i < team -> numPlayers; i++)
    {
        Player* player = team -> players[i];

        // Don't include the scorer of the goal
        if (team -> players[i] == scorer)
        {
            weights[i] = 0.0;
            continue;
        }
        
        double weight = player -> rating;

        // Adjust weights based on their positions
        if (strcmp(player -> position, "fwd") == 0) { weight *= 1.25; }          // Forwards
        else if (strcmp(player -> position, "mid") == 0) { weight *= 2.0; }    // Midfielders
        else if (strcmp(player -> position, "def") == 0) { weight *= 0.75; }    // Defenders
        else if (strcmp(player -> position, "gkp") == 0) { weight *= 0.01; }    // Goal Keepers

        // Exclude injured players
        if (player -> injuryStatus == true) { weight = 0.0; }

        // Store weights and compute total
        weights[i] = weight;
        totalWeight += weight;        
    }

    // Select player based on weighted probability
    if (totalWeight <= 0.0)
    {
        free(weights);
        return NULL;
    }
    
    double randomValue = randomProbability() * totalWeight;
    double cumulativeWeight = 0.0;
    for (int i = 0; i < team -> numPlayers; i++)
    {
        cumulativeWeight += weights[i];
        if (randomValue <= cumulativeWeight)
        {
            free(weights);
            return team -> players[i];
        }
    }
    
    // Fallback, if no player is selected just select whoever is not injured
    free(weights);

    for (int i = 0; i < team -> numPlayers; i++)
    {
        if (team -> players[i] -> injuryStatus == false)
        {
            return team -> players[i];
        }
    }

    fprintf(stderr, "Error: Could not find a player on %s who could've assisted a goal.\n", 
            team -> name);
    return NULL;
}

// Simulate any potential injuries that would occur during a match
void simulateInjuries(Match* match);

// Simulate the minutes of a match, from start -> end
void simulateMatchMinutes(Match* match, int startMinute, int endMinute);
