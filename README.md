# Calendario-Fantacalcio

##Description
This program is able to generate most of the possible calendars by combining th possible matches of N teams and to compute some stats for each calendar computed.

In particular ```v1.0``` computes most of the possible calendar and for each of them compute the ranking and print on the stdout the percentage of each team to reach the podium
## How to use
- Go in the working directory
- Compile the program using ```gcc -Wall src/main.c src/schedule.c -o bin/schedule```
- To execute the program type ```./bin/schedule <input file>``` in the command line

## Input file structure
```<number of teams> <number of matchweeks played>
<name of team1> <score of matchweek1> <score of matchweek2> ... <score of matchweek N>
<name of team2> <score of matchweek1> <score of matchweek2> ... <score of matchweek N>
.
.
.
<name of teamN> <score of matchweek1> <score of matchweek2> ... <score of matchweek N>
```
