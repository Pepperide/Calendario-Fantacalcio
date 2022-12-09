#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/schedule.h"

int main(int argc, char **argv)
{
    Team *teams;
    int n_teams, n_matches_played;
    int max_calendars;
    // int n_combinations;

    if (argc != 3)
    {
        printf("Too few arguments as parameter\n");
        return -1;
    }

    teams = readFile(argv[1], &n_teams, &n_matches_played);
    max_calendars = atoi(argv[2]);

    generateSchedules(teams, n_teams, max_calendars);

    // switch (n_teams)
    // {
    // case 4:
    //     n_combinations = 54;
    //     break;
    // case 6:
    //     n_combinations = 27000;
    //     break;
    // case 8:
    //     n_combinations = 483840;
    //     break;
    // case 10:
    //     n_combinations = 300101760;
    //     break;
    // default:
    //     return -1;
    // }
    // generateSchedules(teams, n_teams, n_matches_played);

    printf("\nTEAM NAME           \t\tFIRST PLACE\t\tSECOND PLACE\t\tTHIRD PLACE\n");
    for (int i = 0; i < n_teams; i++)
    {
        // printf("%-20.20s\t\t%10.5f%%\t\t%11.5f%%\t\t%10.5f%%\n", teams[i].name, (teams[i].podium[0] / (float)n_combinations) * 100, (teams[i].podium[1] / (float)n_combinations) * 100, (teams[i].podium[2] / (float)n_combinations) * 100);
        printf("%-20.20s\t\t%10d\t\t%11d\t\t%10d\n", teams[i].name, teams[i].podium[0], teams[i].podium[1], teams[i].podium[2]);
    }
    return 0;
}