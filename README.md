# Futbol Tournament Simulation
 Project to simulate a futbol tournament from league phase to knockouts and the final.



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



## Testing

We conducted testing through various test files made for each class. Testing was done as I proceeded with the project and reconducted whenever changes were made to classes already defined & implemented. The makefile we constructed takes commands:

0. **All**:
    * Runs all commands

1. **test_player**:
    * Runs the test file for the `player` class

2. **test_team**:
    * Runs the test file for the `team` class

3. **test_match**:
    * Runs the test file for the `match` & `match_simulation` class

4. **test_league**:
    * Runs the test file for the `league` class

5. **test_tournament**:
    * Runs the test file for the `tournament` class

6. **main**: 
    * Runs the main simulation program, where we simulate the league phase of each of the 5 big leagues, and select the top teams to compete in the UCL



## Class Breakdown

### Player class

* **Attributes**: 
    * number
    * position
        * goal keeper - **gkp**
        * defender - **def**
        * midfielder - **mid**
        * forward - **fwd**
    * name
    * rating
    * goals
    * assists
    * injuryStatus

* **Methods**: 
    * createPlayer() + destroyPlayer()
    * scoreGoal()
    * assist()
    * updateInjuryStatus()
    * printPlayer()
    * copyPlayer()


### Team class

* **Attributes**: 
    * players/team roster
    * schedule `Set by the league`
    * record
        * wins
        * losses
        * draws
    * goalDifferential
    * points
    * city
    * coach
    * homeStadium
    * name

* **Methods**: 
    * createTeam() + destroyTeam()
    * addPlayer() + removePlayerByNumber() + removePlayerByName()
    * getPlayerByNumber()
    * getTeamRating()
    * updateRecord()
    * calculateGoalDifferential()
    * printTeam()


### League class

* **Attributes**: 
    * name
    * region/country name
    * teams
        * numTeams
        * maxTeams
    * schedule (2d array)
        * matchesPerDay (array)
        * numMatchDays
        * currentMatchday
        * scheduleGenerated (boolean)
    * leagueTable

* **Methods**: 
    * createLeague() + destroyLeague()
    * addTeamToLeague() + removeTeamFromLeague()
    * generateSchedule()
    * simulateMatchday() + simulateSeason()
    * updateLeagueTable()
    * getTeamByName()
    * getTeamPosition() ~ in league standings
    * *Printing Information*:
        * printLeagueTable()
        * printLeagueTeams()
        * printMatchdaySchedule()
        * printLeagueSchedule()
        * printMatchdayResults()


### Match class

* **Attributes**: 
    * homeTeam
    * awayTeam
    * homeScore
    * awayScore
    * numScorers
    * scorers
    * scorerTeamIndices
    * scoringMinutes
    * date
    * isCompleted

* **Methods**: 
    * createMatch() + destroyMatch()
    * recordGoal()
    * printMatchResult()
    * printMatchReport()
    * updateTeamRecords()
    * getMatchWinner()


### Tournament class

* **Attributes**: 
    * name
    * teams
        * numTeams
    * bracket
        * numRounds
        * matchesPerRound
        * currentRound
    * winner
    * isComplete

* **Methods**: 
    * createTournament() + destroyTournament()
    * addTeamToTournament() + removeTeamFromTournament()
    * drawTournament()
    * simulateTournamentRound()
    * simulateEntireTournament()
    * getTournamentWinner()
    * printTournamentBracket()
    * printRoundMatches()
    * printRoundResults()
    * getRoundName()



## Module + Utils Breakdown

### Match Simulation: 

Logic for simulating match outcomes. *Idea*: Weighted probabilities based on player ratings.

* **Methods**: 
    * simulateMatch()
    * calculateScoringProbability()
    * determineScorer()
    * determineAssist()
    * simulateInjuries()
    * simulateMatchMinutes()

### Random Utils: 

Some utility functions for generating random numbers & probabilities that we needed in our simulations.

* **Methods**: 
    * initRandom()
    * randomProbability()
    * randomInt()