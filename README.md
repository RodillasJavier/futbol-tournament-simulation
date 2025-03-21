# Futbol Tournament Simulation
 Project to simulate a futbol tournament from league phase to knockouts and the final.



## File Structure (containing all implemented files so far)
```
.
├── README.md
├── modules
│   ├── match_simulation.c
│   └── match_simulation.h
├── src
│   ├── match.c
│   ├── match.h
│   ├── player.c
│   ├── player.h
│   ├── team.c
│   └── team.h
└── tests
    ├── test_player.c
    └── test_team.c
```



## Class Breakdown

### Player class

* **Attributes**: 
    * number
    * position
        * goal keeper - **gk**
        * center back - **cb**
        * full backs - **rb, lb, rwb, lwb**
        * center midfield - **cm, cdm, cam**
        * winger - **rm, lm, w, lw**
        * forward - **st, cf**
    * name
    * rating
    * goals
    * assists
    * injuryStatus

* **Methods**: 
    * createPlayer()
    * destroyPlayer()
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
    * createTeam()
    * destroyTeam()
    * addPlayer()
    * removePlayerByNumber()
    * removePlayerByName()
    * getPlayerByNumber()
    * getTeamRating()
    * updateRecord()
    * calculateGoalDifferential()
    * printTeam()


### League class

* **Attributes**: teams, region, leagueTable
* **Methods**: 
    * setSchedule()
    * calculatePoints()
    * simulateMatchDay()
    * updateStandings()


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
    * createMatch()
    * destroyMatch()
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

**Player class**: `player.c` + `player.h`

We defined the test file `test_player.c` to test the different function in the **player** class. We run it in the terminal by navigating to the tests folder and running the command:

    gcc -Wall -Wextra -I../src -o test_player test_player.c ../src/player.c
    ./test_player

**Team class**: `team.c` + `team.h`

We defined the test file `test_team.c` to test the different function in the **team** class. We run it in the terminal by navigating to the tests folder and running the command:

    gcc -Wall -Wextra -I../src -o test_team test_team.c ../src/team.c ../src/player.c
    ./test_team