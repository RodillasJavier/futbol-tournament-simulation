#ifndef MATCH_SIMULATION_H
#define MATCH_SIMULATION_H

/**
 * @author Javier A. Rodillas
 * @details Header file for the match simulation module. 
 */



/* INCLUDE STATEMENTS */

#include "match.h"



/* FUNCTION PROTOTYPES */

/**
 * Simulate a complete match between two teams
 * 
 * @param match that we want to simulate
 */
void simulateMatch(Match* match);

/**
 * Calculate the probability of two teams scoring based on ratings
 * 
 * @param team
 * @param opponentTeam
 * 
 * @return double representing the probability of a team scoring on the opponent team
 */
double calculateScoringProbability(Team* team, Team* opponentTeam);

/**
 * Determine which player on the team scored the goal
 * 
 * @param team that scored the goal
 * 
 * @return pointer to the player who scored the goal for the team
 */
Player* determineScorer(Team* team);

/**
 * Determine which player assisted the goal (if applicable)
 * 
 * @param team that scored the goal/scorer is on
 * @param scorer that made the goal
 * 
 * @return pointer to the player who assisted the goal for the scorer
 */
Player* determineAssist(Team* team, Player* scorer);

/**
 * Simulate any potential injuries that would occur during a match
 * 
 * @param match that we are simulating injuries in (if any)
 */
void simulateInjuries(Match* match);

/**
 * Simulate the minutes of a match, from start -> end
 * 
 * @param match that we are simulating the minutes of
 * @param startMinute that we are beginning the period of play at
 * @param endMinute that we are ending the period of play at
 */
void simulateMatchMinutes(Match* match, int startMinute, int endMinute);

#endif /* MATCH_SIMULATION_H */