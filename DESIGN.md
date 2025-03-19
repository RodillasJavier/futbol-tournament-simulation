# Futbol Tournament Simulation | Design Spec

Design specifications for the implementation of our UCL-style futbol tournament.



## Class Breakdown

`Player`
    
* **Attributes**: number, position, name, rating, goals, assists, injuryStatus
* **Methods**: 
    * createPlayer()
    * destroyPlayer()
    * scoreGoal()
    * assist()
    * updateInjuryStatus()

`Team` 
    
* **Attributes**: players (Roster via Array), schedule, record (W, L, D), goalDifferential, points, city, coach, homeStadium
* **Methods**: 
    * playMatch()
    * updateRecord()
    * calculateGoalDifferential
    * calculatePoints()
    * updateRoster()
 
`League`

* **Attributes**: teams, region, leagueTable
* **Methods**: 
    * generateSchedule()
    * simulateMatchDay()
    * updateStandings()

`Match`

* **Attributes**: homeTeam, awayTeam, score, scorers, date
* **Methods**: 
    * simulateMatch()
    * recordResults()

`Tournament`

* **Attributes**: teams, bracket, rounds
* **Methods**: 
    * drawTeams()
    * simulateKnockoutStage()
    * simulateFinal()



## Module Breakdown

### Match Simulation Module: 

Logic for simulating match outcomes. Idea: Weighted probabilities based on player ratings.

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