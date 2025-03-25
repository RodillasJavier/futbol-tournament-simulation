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
│   ├── match.c
│   ├── match.h
│   ├── modules
│   │   ├── match_simulation.c
│   │   └── match_simulation.h
│   ├── player.c
│   ├── player.h
│   ├── team.c
│   ├── team.h
│   └── utils
│       ├── random_utils.c
│       └── random_utils.h
└── tests
    ├── test_league.c
    ├── test_match.c
    ├── test_player.c
    └── test_team.c
```



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

* **Attributes**: teams, bracket, rounds
* **Methods**: 
    * drawTeams()
    * simulateKnockoutStage()
    * simulateFinal()



## Module Breakdown

### Match Simulation Module: 

Logic for simulating match outcomes. *Idea*: Weighted probabilities based on player ratings.

* **Methods**: 
    * simulateMatch()
    * calculateScoringProbability()
    * determineScorer()
    * determineAssist()
    * simulateInjuries()
    * simulateMatchMinutes()

### Tournament Simulation Module: 

Logic for simulating tournament legs & final once the knockout stages begin. 

### Statistics Module: 

Tracks goals, assists, scores, etc. for matches as well as for the entire season. 

### Schedule Generator Module: 

Generates input for league play and knockout rounds based on drawing and league schedule structure: 

* Each team plays the other exactly once in the first half of the season.
* Each team plays each other in the exact same order in the second half of the season, but with home/away status reversed. 
* We'll generate the first half schedule at random for this project. 

### File I/O module: 

Takes in teams, rosters & schedules and uses them as input for the program. Also in charge of output in the terminal? Main method here?



## Testing
