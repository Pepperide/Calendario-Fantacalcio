#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/schedule.h"

int main(int argc, char **argv)
{
    Team *teams;
    int n_teams, n_matches_played;
    int n_combinations;

    if (argc != 2)
    {
        printf("Too few arguments as parameter");
        return -1;
    }

    teams = readFile(argv[1], &n_teams, &n_matches_played);

    switch (n_teams)
    {
    case 4:
        n_combinations = 18;
        break;
    case 6:
        n_combinations = 1560;
        break;
    case 8:
        n_combinations = 483840;
        break;
    case 10:
        n_combinations = 300101760;
        break;
    default:
        return -1;
    }
    generateSchedules(teams, n_teams, n_matches_played);

    printf("\nTEAM NAME           \t\tFIRST PLACE\t\tSECOND PLACE\t\tTHIRD PLACE\n");
    for (int i = 0; i < n_teams; i++)
    {
        printf("%-20.20s\t\t%10.5f%%\t\t%11.5f%%\t\t%10.5f%%\n", teams[i].name, (teams[i].first / (float)n_combinations) * 100, (teams[i].second / 483840.0) * 100, (teams[i].third / 483840.0) * 100);
    }
    return 0;
}

Team *readFile(char *file, int *n_teams, int *n_matches_played)
{
    FILE *fp;
    Team *t;
    int count = 0;

    fp = fopen(file, "r");
    fscanf(fp, "%d %d", n_teams, n_matches_played); // Number of lines to be read. Number of teams

    t = (Team *)malloc(*n_teams * sizeof(Team));
    for (int i = 0; i < *n_teams; i++)
    {
        t[i].name = (char *)malloc(30 * sizeof(char));
        t[i].firstRound = (float *)malloc(*n_matches_played * sizeof(float));
        t[i].secondRound = (float *)malloc(*n_matches_played * sizeof(float));
        t[i].n_matches_first = *n_matches_played;
        t[i].points = 0;
        t[i].first = 0;
        t[i].second = 0;
        t[i].third = 0;
    }

    while (count < *n_teams)
    {
        fscanf(fp, "%s", t[count].name);
        for (int i = 0; i < *n_matches_played; i++)
        {
            fscanf(fp, "%f", &t[count].firstRound[i]);
        }
        count++;
    }
    fclose(fp);

    return t;
}
