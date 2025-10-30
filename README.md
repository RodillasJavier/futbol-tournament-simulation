# Football Tournament Simulation

A C-based football (soccer) simulation program that models league competitions and knockout tournaments. The simulation features realistic match mechanics with weighted probabilities based on player ratings, comprehensive team management, and tournament bracket systems.

## Overview

This project simulates football competitions at multiple levels:
- **League Phase**: Round-robin tournaments where each team plays every other team twice (home and away)
- **Knockout Tournament**: Single-elimination bracket-style competition
- **Match Simulation**: Detailed match-by-match simulation with goal scorers, assists, and minute-by-minute events

The main program simulates 5 major European leagues (La Liga, Premier League, Bundesliga, Serie A, and Ligue 1), selects top teams from each league, and creates a Champions League-style knockout tournament to determine the ultimate winner.



## File Structure (containing all implemented files so far)
```
.
├── Makefile
├── README.md
├── src
│   ├── league.c
│   ├── league.h
│   ├── main.c
│   ├── match.c
│   ├── match.h
│   ├── modules
│   │   ├── match_simulation.c
│   │   └── match_simulation.h
│   ├── player.c
│   ├── player.h
│   ├── team.c
│   ├── team.h
│   ├── tournament.c
│   ├── tournament.h
│   └── utils
│       ├── random_utils.c
│       └── random_utils.h
└── tests
    ├── test_league.c
    ├── test_match.c
    ├── test_player.c
    ├── test_team.c
    └── test_tournament.c
```



## Getting Started

### Prerequisites
- GCC compiler
- Make utility
- Standard C library

### Building the Project

The project uses a Makefile for easy compilation and testing:

```bash
# Build all targets (tests and main program)
make all

# Build and run individual tests
make test_player
make test_team
make test_match
make test_league
make test_tournament

# Build and run the main simulation
make main

# Clean build artifacts
make clean
```

### Running the Simulation

After building, execute the main program:

```bash
./bin/main
```

This will:
1. Create 5 leagues (La Liga, Premier League, Bundesliga, Serie A, Ligue 1) with 20 teams each
2. Generate random teams with players for each league
3. Simulate full league seasons (38 matchdays per league)
4. Select the top 3 teams from each league (15 teams total)
5. Add FC Barcelona as a special entrant (making 16 teams)
6. Run a single-elimination Champions League tournament
7. Display the tournament winner

## Testing

Comprehensive testing is implemented for each component of the system. The Makefile provides commands to run individual test suites:

- **make all**: Builds all executables (tests and main program)
- **make test_player**: Tests the Player class functionality
- **make test_team**: Tests the Team class functionality
- **make test_match**: Tests the Match class and match simulation logic
- **make test_league**: Tests the League class and scheduling
- **make test_tournament**: Tests the Tournament class and bracket generation
- **make main**: Builds and runs the main simulation program



## Class Breakdown

### Player Class

Represents an individual football player with attributes and statistics.

**Attributes:**
- `number` - Jersey number
- `position` - Player position (gk, def, mid, fwd)
- `name` - Player name (up to 50 characters)
- `rating` - Skill rating (1-100)
- `goals` - Goals scored
- `assists` - Assists provided
- `injuryStatus` - Boolean injury flag

**Methods:**
- `createPlayer()` / `destroyPlayer()` - Constructor and destructor
- `scoreGoal()` - Increment goal counter
- `assist()` - Increment assist counter
- `updateInjuryStatus()` - Update injury status
- `printPlayer()` - Display player information
- `copyPlayer()` - Create a deep copy of a player


### Team Class

Represents a football club with a roster of players and match statistics.

**Attributes:**
- `name` - Club name
- `city` - Home city
- `coach` - Head coach name
- `homeStadium` - Stadium name
- `players` - Array of player pointers
- `numPlayers` / `maxPlayers` - Roster size tracking
- `schedule` - Array of opponent indices (set by league)
- `rating` - Overall team rating (calculated from players)
- `wins` / `losses` / `draws` - Match record
- `goalsScored` / `goalsConceded` / `goalDifferential` - Goal statistics
- `points` - League points (3 for win, 1 for draw)

**Methods:**
- `createTeam()` / `destroyTeam()` - Constructor and destructor
- `addPlayer()` - Add a player to the roster
- `removePlayerByNumber()` / `removePlayerByName()` - Remove players from roster
- `getPlayerByNumber()` - Retrieve a specific player
- `getTeamRating()` - Calculate overall team rating
- `updateRecord()` - Update win/loss/draw record
- `calculateGoalDifferential()` - Update goal differential
- `printTeam()` - Display team information


### League Class

Manages a round-robin league competition where each team plays every other team twice.

**Attributes:**
- `name` - League name
- `region` - Country/region
- `teams` - Array of team pointers
- `numTeams` / `maxTeams` - Team count tracking
- `schedule` - 2D array of matches [matchday][match]
- `matchesPerMatchday` - Number of matches per matchday
- `numMatchdays` - Total matchdays in season
- `currentMatchday` - Current matchday tracker
- `scheduleGenerated` - Boolean flag for schedule status
- `leagueTable` - Sorted team rankings

**Methods:**
- `createLeague()` / `destroyLeague()` - Constructor and destructor
- `addTeamToLeague()` / `removeTeamFromLeague()` - Team management
- `generateSchedule()` - Create round-robin match schedule
- `simulateMatchday()` - Simulate a single matchday
- `simulateSeason()` - Simulate entire league season
- `updateLeagueTable()` - Sort teams by points/goal differential
- `getTeamByName()` - Retrieve team by name
- `getTeamPosition()` - Get team's league standing
- **Display Methods:**
  - `printLeagueTable()` - Show current standings
  - `printLeagueTeams()` - List all teams
  - `printMatchdaySchedule()` - Show upcoming matches
  - `printLeagueSchedule()` - Show entire season schedule
  - `printMatchdayResults()` - Show matchday results


### Match Class

Represents a single match between two teams with detailed goal tracking.

**Attributes:**
- `homeTeam` / `awayTeam` - Team pointers
- `homeScore` / `awayScore` - Final scores
- `numScorers` - Number of goals scored
- `scorers` - Array of players who scored
- `scorerTeamIndices` - Which team scored each goal (0=home, 1=away)
- `scoringMinutes` - Minute each goal was scored
- `date` - Match date string
- `isCompleted` - Boolean completion flag

**Methods:**
- `createMatch()` / `destroyMatch()` - Constructor and destructor
- `recordGoal()` - Record a goal with scorer, team, and minute
- `printMatchResult()` - Display final score
- `printMatchReport()` - Show detailed match report with scorers
- `updateTeamRecords()` - Update both teams' win/loss/draw records
- `getMatchWinner()` - Return winning team (or -1 for draw)


### Tournament Class

Manages single-elimination knockout tournaments with bracket generation.

**Attributes:**
- `name` - Tournament name
- `teams` - Array of participating teams
- `numTeams` - Number of teams (must be power of 2)
- `bracket` - 3D array of matches [round][match]
- `numRounds` - Number of knockout rounds
- `matchesPerRound` - Matches in each round
- `currentRound` - Current round tracker
- `winner` - Tournament champion
- `isComplete` - Boolean completion flag

**Methods:**
- `createTournament()` / `destroyTournament()` - Constructor and destructor
- `addTeamToTournament()` / `removeTeamFromTournament()` - Team management
- `drawTournament()` - Seed teams into bracket (supports up to 32 teams)
- `simulateTournamentRound()` - Simulate a specific round
- `simulateEntireTournament()` - Simulate all rounds until completion
- `getTournamentWinner()` - Return tournament champion
- `printTournamentBracket()` - Display entire bracket structure
- `printRoundMatches()` - Show matches for a specific round
- `printRoundResults()` - Show results for a specific round
- `getRoundName()` - Get round name (e.g., "Round of 16", "Quarter-finals")



## Modules and Utilities

### Match Simulation Module (`src/modules/match_simulation.c`)

Core simulation engine that determines match outcomes using weighted probability calculations.

**Algorithm:**
- Calculates scoring probability for each team based on overall team rating
- Simulates 90 minutes of match time with probabilistic goal events
- Determines goal scorers weighted by individual player ratings
- Tracks assists and updates player statistics
- Handles injury simulations

**Methods:**
- `simulateMatch()` - Main simulation function
- `calculateScoringProbability()` - Compute probability based on team ratings
- `determineScorer()` - Select goal scorer weighted by player rating
- `determineAssist()` - Select assisting player (if applicable)
- `simulateInjuries()` - Apply injury mechanics
- `simulateMatchMinutes()` - Minute-by-minute simulation

### Random Utilities Module (`src/utils/random_utils.c`)

Provides random number generation utilities for simulation mechanics.

**Methods:**
- `initRandom()` - Initialize random seed (called once)
- `randomProbability()` - Generate random double [0.0, 1.0]
- `randomInt()` - Generate random integer in specified range

## Features

- **Realistic Match Simulation**: Goals are generated based on team and player ratings with weighted probabilities
- **Comprehensive Statistics**: Tracks goals, assists, wins, losses, draws, goal differential, and points
- **Flexible League System**: Supports any number of teams with automatic round-robin scheduling
- **Tournament Brackets**: Single-elimination tournaments with automatic bracket generation for power-of-2 team counts
- **Detailed Reporting**: Match reports with scorers, minutes, and final standings
- **Modular Architecture**: Separate modules for core classes, simulation logic, and utilities

## Project Structure

The codebase is organized into logical components:

- **`src/`** - Core class implementations (Player, Team, Match, League, Tournament)
- **`src/modules/`** - Simulation logic modules
- **`src/utils/`** - Utility functions
- **`tests/`** - Unit tests for each component
- **`build/`** - Compiled object files (created by Make)
- **`bin/`** - Executable binaries (created by Make)

## Future Enhancements

Potential improvements to the simulation:

- Home advantage mechanics
- More sophisticated injury systems
- Player fatigue and rotation
- Tactical formations and strategies
- Transfer system between teams
- Season-long player development
- More detailed match events (yellow/red cards, substitutions)
- GUI or web interface for visualization

## Author

Javier A. Rodillas

## License

This project is available for educational purposes.