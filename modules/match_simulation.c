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



/* INCLUDE */

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

// Calculate the probability of two teams scoring based on ratings in the range (0.01, 0.1) inclusive
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
    double scoringProbability = 0.015;

    // Get team ratings
    double teamRating = calculateTeamRating(team);
    double opponentTeamRating = calculateTeamRating(opponentTeam);

    // Calculate ratio between team ratings
    double teamRatingRatio = teamRating / opponentTeamRating;

    // Weight our new scoring probability
    double weightedScoringProbability = scoringProbability * teamRatingRatio;

    // Upper and Lower bounding scoring probabilities
    if (weightedScoringProbability < 0.01)
    {
        return 0.01;
    } else if (weightedScoringProbability > 0.1)
    {
        return 0.1;
    }

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
        if (player == scorer)
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
void simulateInjuries(Match* match)
{
    // Verify input
    if (match == NULL || match -> homeTeam == NULL || match -> awayTeam == NULL)
    {
        fprintf(stderr, "Error: Match or one of it's participating teams were NULL.\n");
        return;
    }

    // Get home and away teams
    Team* homeTeam = match -> homeTeam;
    Team* awayTeam = match -> awayTeam;

    // Home team injuries
    for (int i = 0; i < homeTeam -> numPlayers; i++)
    {
        // Get player from the home team
        Player* player = homeTeam -> players[i];

        // 2.5% chance of injury per match
        if (randomProbability() < 0.025 && player -> injuryStatus == false)
        {
            updateInjuryStatus(player, true);
            fprintf(stdout, 
                    "INJURY: %s (%s) was injured during the match!\n", 
                    player -> name, homeTeam -> name);
        }
    }

    // Away team injuries
    for (int i = 0; i < awayTeam -> numPlayers; i++)
    {
        // Get player from the away team
        Player* player = awayTeam -> players[i];

        // 2.5% chance of injury per match
        if (randomProbability() < 0.025 && player -> injuryStatus == false)
        {
            updateInjuryStatus(player, true);
            fprintf(stdout, 
                    "INJURY: %s (%s) was injured during the match!\n", 
                    player -> name, awayTeam -> name);
        }
    }
}

// Simulate the minutes of a match, from start -> end
void simulateMatchMinutes(Match* match, int startMinute, int endMinute)
{
    // Validate input
    if (match == NULL)
    {
        fprintf(stderr, "Error: Tried simulating minutes for a NULL match.\n");
        return;
    }
    else if (match -> homeTeam == NULL || match -> awayTeam == NULL)
    {
        fprintf(stderr, "Error: Simulating minutes w/one or more NULL teams - %s or %s.\n", 
                match -> homeTeam -> name, match -> awayTeam -> name);
        return;
    }
    else if (startMinute < 0 || endMinute < startMinute)
    {
        fprintf(stderr, "Error: Invalid minutes, start: %d end: %d.\n", 
                startMinute, endMinute);
        return;
    }
    
    // Get Home and Away teams
    Team* homeTeam = match -> homeTeam;
    Team* awayTeam = match -> awayTeam;

    // Simulate each minute
    for (int minute = startMinute; minute <= endMinute; minute++)
    {
        // Check if home team scores
        double homeTeamScoreProbability = calculateScoringProbability(homeTeam, awayTeam);
        if (randomProbability() < homeTeamScoreProbability)
        {
            // Get the player who scored the goal
            Player* scorer = determineScorer(homeTeam);
            
            // Null check our scorer
            if (scorer == NULL)
            {
                fprintf(stderr, "Error: Couldn't find a scorer for %s at %d'.\n", 
                        homeTeam -> name, minute);
                continue;
            }

            // Recording goal for player & home team (0 => home)
            recordGoal(match, scorer, HOME_TEAM, minute);
            
            // Check for assist
            Player* assister = determineAssist(homeTeam, scorer);
            if (assister)
            {
                // Recording assist for player
                assist(assister);
            }
        }

        // Check if away team scores
        double awayTeamScoreProbability = calculateScoringProbability(awayTeam, homeTeam);
        if (randomProbability() < awayTeamScoreProbability)
        {
            // Get the player who scored the goal
            Player* scorer = determineScorer(awayTeam);
            
            // Null check our scorer
            if (scorer == NULL)
            {
                fprintf(stderr, "Error: Couldn't find a scorer for %s at %d'.\n", 
                        awayTeam -> name, minute);
                continue;
            }

            // Recording goal for player & away team (1 => away)
            recordGoal(match, scorer, AWAY_TEAM, minute);
            
            // Check for assist
            Player* assister = determineAssist(awayTeam, scorer);
            if (assister)
            {
                // Recording assist for player
                assist(assister);
            }
        }
    }
}