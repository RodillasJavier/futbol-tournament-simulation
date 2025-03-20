#ifndef MATCH_H
#define MATCH_H

/**
 * @author Javier A. Rodillas
 * @details Header file for the Player class file
 */

#include "player.h"
#include "team.h"

typedef struct {
    // Teams
    Team* homeTeam;
    Team* awayTeam;

    // 
} Match;

#endif /* MATCH_H */