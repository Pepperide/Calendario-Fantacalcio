# Calendario-Fantacalcio

## Description
This program is able to generate most of the possible calendars by combining the possible matches of N teams and to compute some stats for each calendar computed.

In particular ```v1.X``` computes most of the possible calendar and for each of them it computes the ranking and prints on the stdout the percentage of each team to reach the podium
## How to use
- Go in the working directory
- Compile the program using ```gcc -Wall main.c Team.c Match.c Matchweek.c Round.c -o bin/schedule```
- To execute the program type ```./bin/schedule <number of teams> <input filename>``` in the command line

## Input file structure
```
<number of matchweeks played>
<name of team1> <score of matchweek1> <score of matchweek2> ... <score of matchweek N>
<name of team2> <score of matchweek1> <score of matchweek2> ... <score of matchweek N>
.
.
.
<name of teamN> <score of matchweek1> <score of matchweek2> ... <score of matchweek N>
```
### Example
```
5
Team1 66 68 69 80 75.5
team2 65.5 78 73 74.5 85.5
Team3 66 65.5 64 62 81
```
