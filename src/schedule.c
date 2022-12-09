#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/schedule.h"

#define N_MATCHES4TEAMS 6
#define N_MATCHES6TEAMS 15
#define N_MATCHES8TEAMS 28
#define N_MATCHES10TEAMS 45

#define N_MATCHWEEKS4TEAMS 3
#define N_MATCHWEEKS6TEAMS 15
#define N_MATCHWEEKS8TEAMS 105
#define N_MATCHWEEKS10TEAMS 945

#define N_ROUNDSS4TEAMS 6
#define N_ROUNDSS6TEAMS 360360
#define N_ROUNDSS8TEAMS 22760723700
#define N_ROUNDSS10TEAMS 945 // HUGE NUMBER

#define MAX_ROUNDS 500000 // 1mld max possible rounds to be calculated

Team *readFile(char *file, int *n_teams, int *n_matches_played)
{
    FILE *fp;
    Team *t;
    int count = 0;

    fp = fopen(file, "r");
    // Read the first line which contains "<n_teams> <n_played_matches>""
    fscanf(fp, "%d %d", n_teams, n_matches_played);

    t = (Team *)malloc(*n_teams * sizeof(Team));

    for (int i = 0; i < *n_teams; i++)
    {
        t[i].name = (char *)malloc(30 * sizeof(char));
        t[i].mw_points = (float *)malloc(*n_matches_played * sizeof(float));
        t[i].total_points = 0;
        t[i].podium[0] = 0;
        t[i].podium[1] = 0;
        t[i].podium[2] = 0;
    }

    // Read the following n_teams rows (One for each team)
    while (count < *n_teams)
    {
        fscanf(fp, "%s", t[count].name);
        for (int i = 0; i < *n_matches_played; i++)
        {
            fscanf(fp, "%f", &t[count].mw_points[i]);
        }
        count++;
    }

    fclose(fp);

    return t;
}

void generateSchedules(Team *teams, int n_teams, int max_calendars)
{
    Match *matches;
    Matchweek *matchweeks;
    Round *rounds;

    int n_possible_matches, n_matchweeks;
    long int n_rounds;

    switch (n_teams)
    {
    case 4:
        n_possible_matches = N_MATCHES4TEAMS;
        n_matchweeks = N_MATCHWEEKS4TEAMS;
        n_rounds = N_ROUNDSS4TEAMS;
        break;
    case 6:
        n_possible_matches = N_MATCHES6TEAMS;
        n_matchweeks = N_MATCHWEEKS6TEAMS;
        n_rounds = N_ROUNDSS6TEAMS;
        break;
    case 8:
        n_possible_matches = N_MATCHES8TEAMS;
        n_matchweeks = N_MATCHWEEKS8TEAMS;
        n_rounds = MAX_ROUNDS;
        break;
    case 10:
        n_possible_matches = N_MATCHES10TEAMS;
        n_matchweeks = N_MATCHWEEKS10TEAMS;
        n_rounds = MAX_ROUNDS;
        break;
    default:
        return;
    }

    matches = (Match *)malloc(n_possible_matches * sizeof(Match));
    matchweeks = (Matchweek *)malloc(n_matchweeks * sizeof(Matchweek));
    for (int i = 0; i < n_matchweeks; i++)
    {
        matchweeks[i].matches = (Match *)malloc(n_teams / 2 * sizeof(Match));
    }
    rounds = (Round *)malloc(n_rounds * sizeof(Round));

    compute_possible_matches_wrapper(teams, matches, n_teams);

    compute_possible_matchweeks_wrapper(matches, matchweeks, n_teams, n_possible_matches);

    compute_possible_rounds_wrapper(matchweeks, rounds, n_teams, n_teams / 2, n_matchweeks, max_calendars, teams);
    return;
}

void compute_possible_matches_wrapper(Team *val, Match *matches, int n_teams)
{
    Team *sol;
    int count;

    sol = (Team *)malloc(2 * sizeof(Team));

    count = compute_possible_matches(0, val, sol, matches, n_teams, 2, 0, 0);

    printf("Possible matches: %d\n", count);
    return;
}

int compute_possible_matches(int pos, Team *val, Team *sol, Match *saved, int n, int k, int start, int count)
{
    int i;

    if (pos >= k)
    {
        saved[count].home = sol[0];
        saved[count].away = sol[1];
        return count + 1;
    }

    for (i = start; i < n; i++)
    {
        sol[pos] = val[i];
        count = compute_possible_matches(pos + 1, val, sol, saved, n, k, i + 1, count);
    }
    return count;
}

void compute_possible_matchweeks_wrapper(Match *val, Matchweek *matchweeks, int n_teams, int n_possible_matches)
{
    Match *sol;
    int count;

    sol = (Match *)malloc(n_teams / 2 * sizeof(Match));

    count = compute_possible_matchweeks(0, val, sol, matchweeks, n_possible_matches, n_teams / 2, 0, 0);

    printf("Possible matchweeks: %d\n", count);
    // for (int i = 0; i < count; i++)
    // {
    //     printf("Matchweek %d\n", i + 1);
    //     for (int j = 0; j < n_teams / 2; j++)
    //     {
    //         printf("%s - %s\n", matchweeks[i].matches[j].home.name, matchweeks[i].matches[j].away.name);
    //     }
    //     printf("\n");
    // }
}

int compute_possible_matchweeks(int pos, Match *val, Match *sol, Matchweek *saved, int n, int k, int start, int count)
{
    int i;

    if (pos >= k)
    {
        if (!valid_matchweek(sol, k))
        {
            return count;
        }

        for (i = 0; i < k; i++)
        {
            saved[count].matches[i] = sol[i];
        }
        return count + 1;
    }
    for (i = start; i < n; i++)
    {
        sol[pos].home = val[i].home;
        sol[pos].away = val[i].away;
        count = compute_possible_matchweeks(pos + 1, val, sol, saved, n, k, i + 1, count);
    }
    return count;
}

int valid_matchweek(Match *sol, int n_matches)
{
    // Check if matches are valid for a matchweek
    for (int i = 0; i < n_matches; i++)
    {
        // For each match
        for (int j = 0; j < n_matches; j++)
        {
            if (i != j)
            {
                if (strcmp(sol[i].home.name, sol[j].home.name) == 0 ||
                    strcmp(sol[i].home.name, sol[j].away.name) == 0 ||
                    strcmp(sol[i].away.name, sol[j].home.name) == 0 ||
                    strcmp(sol[i].away.name, sol[j].away.name) == 0)
                {
                    // A team appears twice in a matchweek. Machweek not valid
                    return 0;
                }
            }
        }
    }
    return 1;
};

void compute_possible_rounds_wrapper(Matchweek *val, Round *rounds, int n_teams, int n_matches, int n_matchweeks, int max_calendars, Team *teams)
{
    Matchweek *sol;
    int count;

    sol = (Matchweek *)malloc((n_teams - 1) * sizeof(Matchweek));

    count = compute_possible_rounds(0, val, sol, rounds, n_matchweeks, n_teams - 1, 0, 0, n_matches, max_calendars, teams, n_teams);
    printf("Possible rounds: %d\n", count);
    printf("\n");
}

int compute_possible_rounds(int pos, Matchweek *val, Matchweek *sol, Round *saved, int n, int k, int start, int count, int n_matches, int max_calendars, Team *teams, int n_teams)
{
    int i;

    if (pos >= k)
    {
        if (count >= max_calendars || count >= MAX_ROUNDS)
        {
            return count;
        }
        if (!valid_round(saved, sol, n_matches, k, count))
        {
            return count;
        }

        count = permute_wrapper(saved, sol, k, count, max_calendars, teams, n_teams);
        printf("Possible rounds: %d\r", count);
        return count;
    }

    for (i = start; i < n && count < max_calendars; i++)
    {
        sol[pos] = val[i];
        count = compute_possible_rounds(pos + 1, val, sol, saved, n, k, i + 1, count, n_matches, max_calendars, teams, n_teams);
    }
    return count;
}

int valid_round(Round *rounds, Matchweek *sol, int n_matches, int n_matchweeks, int n_rounds)
{
    for (int i = 0; i < n_matchweeks; i++)
    {
        for (int j = i + 1; j < n_matchweeks; j++)
        {
            for (int m = 0; m < n_matches; m++)
            {
                for (int n = 0; n < n_matches; n++)
                {
                    if ((strcmp(sol[i].matches[m].home.name, sol[j].matches[n].home.name) == 0 &&
                         strcmp(sol[i].matches[m].away.name, sol[j].matches[n].away.name) == 0) ||
                        (strcmp(sol[i].matches[m].home.name, sol[j].matches[n].away.name) == 0 &&
                         strcmp(sol[i].matches[m].away.name, sol[j].matches[n].home.name) == 0))
                    {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
};

int permute_wrapper(Round *saved, Matchweek *val, int n_matchweeks, int count, int max_calendars, Team *teams, int n_teams)
{
    Matchweek *sol;
    int *mark;

    sol = (Matchweek *)malloc(n_matchweeks * sizeof(Matchweek));
    mark = (int *)calloc(n_matchweeks, sizeof(int));
    count = permute(0, val, sol, mark, n_matchweeks, count, saved, max_calendars, teams, n_teams);
    return count;
}

int permute(int pos, Matchweek *val, Matchweek *sol, int *mark, int n, int count, Round *saved, int max_calendars, Team *teams, int n_teams)
{
    int i;
    if (pos >= n)
    {
        if (count > max_calendars)
        {
            return count;
        }
        // saved[count].matchweeks = sol;
        compute(sol, teams, n, n_teams, n_teams / 2);
        // printf("%d\r", count);

        return count + 1;
    }

    for (i = 0; i < n && count < max_calendars; i++)
    {
        if (mark[i] == 0)
        {
            mark[i] = 1;
            sol[pos] = val[i];
            count = permute(pos + 1, val, sol, mark, n, count, saved, max_calendars, teams, n_teams);
            mark[i] = 0;
        }
    }
    return count;
}

void compute(Matchweek *round, Team *teams, int n_matchweek, int n_teams, int n_matches)
{
    for (int i = 0; i < n_matchweek; i++)
    {
        // For each matchweek
        for (int j = 0; j < n_teams / 2; j++)
        {
            // For each match of the matchweek
            for (int k = i; k < n_matches; k += (n_teams - 1))
            {
                if (goal(round[i].matches[j].home.mw_points[k]) > goal(round[i].matches[j].away.mw_points[k]))
                {
                    for (int m = 0; m < n_teams; m++)
                    {
                        strcmp(round[i].matches[j].home.name, teams[m].name) == 0 ? teams[m].total_points += 3 : 1;
                    }
                }
                else if (goal(round[i].matches[j].home.mw_points[k]) < goal(round[i].matches[j].away.mw_points[k]))
                {
                    for (int m = 0; m < n_teams; m++)
                    {
                        strcmp(round[i].matches[j].away.name, teams[m].name) == 0 ? teams[m].total_points += 3 : 1;
                    }
                }
                else
                {
                    for (int m = 0; m < n_teams; m++)
                    {
                        strcmp(round[i].matches[j].home.name, teams[m].name) == 0 ? teams[m].total_points += 1 : 1;
                        strcmp(round[i].matches[j].away.name, teams[m].name) == 0 ? teams[m].total_points += 1 : 1;
                    }
                }
            }
        }
    }
    Team tmp;
    for (int i = 0; i < n_teams - 1; i++)
    {
        for (int j = 0; j < n_teams - 1; j++)
        {
            if (teams[j].total_points > teams[j + 1].total_points)
            {
                tmp = teams[j];
                teams[j] = teams[j + 1];
                teams[j + 1] = tmp;
            }
        }
    }

    teams[n_teams - 1].podium[0]++;
    teams[n_teams - 2].podium[1]++;
    teams[n_teams - 3].podium[2]++;

    for (int i = 0; i < n_teams; i++)
    {
        teams[i].total_points = 0;
    }
}

int goal(float points)
{
    float base = 66.0;
    int goals = 1;

    if (points < base)
        return 0;

    while (base + 4.0 < points)
    {
        base += 4.0;
        goals++;
    }
    return goals;
}