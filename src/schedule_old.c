#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/schedule.h"

void generateSchedules(Team *teams, int n_teams, int n_effective_matches)
{
    int count; // Useful for counting combinations in order to debug
    int n_matches;
    int n_matchweeks;

    Team sol1[2];            // Internal variable used for computing team combinations in comb_sempl_match().
    Match *possible_matches; // Possible matches computed by comb_sempl_match().

    Match *sol2; // Internal variable for computing match combination
    Matchweek *possible_matchweeks;

    // --- INITIALIZATION ---

    switch (n_teams)
    {
    case 4:
        n_matches = 6;
        n_matchweeks = 3;
        break;
    case 6:
        n_matches = 15;
        n_matchweeks = 15;
        break;
    case 8:
        n_matches = 28;
        n_matchweeks = 105;
        break;
    case 10:
        n_matches = 45;
        n_matchweeks = 945;
        break;
    default:
        return;
    };
    possible_matches = malloc(n_matches * sizeof(Match)); // Mathematically computed
    sol2 = malloc(n_matches * sizeof(Match));
    possible_matchweeks = malloc(n_matchweeks * sizeof(Matchweek)); // Mathematically computed
    for (int i = 0; i < n_matchweeks; i++)
    {
        possible_matchweeks[i].matches = malloc(n_teams / 2 * sizeof(Match));
    }

    count = comb_sempl_match(0, teams, sol1, n_teams, 2, 0, 0, possible_matches);
    printf("Possible matches: %d\n", count);

    count = comb_sempl_matchweek(0, possible_matches, sol2, n_matches, n_teams / 2, 0, 0, possible_matchweeks);
    printf("Possible matchweeks: %d\n", count);

    filterMatchweeks(possible_matchweeks, n_matchweeks, teams, n_teams, n_effective_matches);
}

int comb_sempl_match(int pos, Team *val, Team *sol, int n, int k, int start, int count, Match *saved)
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
        count = comb_sempl_match(pos + 1, val, sol, n, k, i + 1, count, saved);
    }
    return count;
}

int comb_sempl_matchweek(int pos, Match *val, Match *sol, int n, int k, int start, int count, Matchweek *saved)
{
    int i;

    if (pos >= k)
    {
        if (validate(sol, k))
        {
            return count;
        }

        for (i = 0; i < k; i++)
        {
            saved[count].matches[i].home = sol[i].home;
            saved[count].matches[i].away = sol[i].away;
        }
        return count + 1;
    }
    for (i = start; i < n; i++)
    {
        sol[pos].home = val[i].home;
        sol[pos].away = val[i].away;
        count = comb_sempl_matchweek(pos + 1, val, sol, n, k, i + 1, count, saved);
    }
    return count;
}

int validate(Match *sol, int n_matches)
{
    for (int i = 0; i < n_matches; i++)
    {
        for (int j = 0; j < n_matches; j++)
        {
            if (i != j)
            {
                if (strcmp(sol[i].home.name, sol[j].home.name) == 0 ||
                    strcmp(sol[i].home.name, sol[j].away.name) == 0 ||
                    strcmp(sol[i].away.name, sol[j].home.name) == 0 ||
                    strcmp(sol[i].away.name, sol[j].away.name) == 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
};

void filterMatchweeks(Matchweek *mw, int n, Team *teams, int n_teams, int n_effective_matches)
{
    Matchweek *matchweek_set; // Filtered matchweeks which are compatible to be in a round
    Matchweek *sol;
    Round rounds[54]; // TO BE changed
    int *mark;

    int size = 0;
    int count = 0;

    matchweek_set = malloc(n * sizeof(Matchweek));
    sol = malloc(n * sizeof(Matchweek));
    mark = malloc(n * sizeof(int));

    for (int i = 0; i < 54; i++)
    {
        rounds[i].matchweeks = malloc((n_teams - 1) * sizeof(Matchweek));
    }

    for (int i = 0; i < n; i++)
    {
        matchweek_set[i].matches = malloc(n_teams / 2 * sizeof(Match));
        sol[i].matches = malloc(n_teams / 2 * sizeof(Match));
        mark[i] = 0;
    }

    for (int l = 0; l < n; l++)
    {
        for (int i = 0; i < n; i++)
        {
            matchweek_set[i].matches = (void *)0;
        }

        for (int i = 0; i < n; i++)
        {
            matchweek_set[i].matches = malloc(n_teams / 2 * sizeof(Match));
        }

        size = 0;
        matchweek_set[size++].matches = mw[l].matches;

        for (int i = 0; i < n; i++)
        {
            filter(0, mw, matchweek_set, n, 0, n_teams, 0);
            count = PermuteAndCompute(0, matchweek_set, sol, mark, n_teams - 1, count, teams, n_teams, n_effective_matches, rounds, 0);
        }
    }
    printf("Possible rounds: %d\n", count);
}

int PermuteAndCompute(int pos, Matchweek *val, Matchweek *sol, int *mark, int n, int count, Team *teams, int n_teams, int n_matches, Round *rounds, int rounds_size)
{
    int i;
    int valid = 1;
    if (pos >= n)
    {
        for (i = 0; i < n && valid; i++)
        {
            // For each matchweek of the sol

            for (int j = 0; j < count && valid; j++)
            {
                // For each round
                for (int k = 0; k < n && valid; k++)
                {
                    // For each matchweek of the round
                    if (compare_Matchweeks(sol[i], rounds[j].matchweeks[k], n_teams / 2) == 0)
                    {
                        valid = 0;
                    }
                }
            }
            if (valid)
            {
                for (i = 0; i < n; i++)
                {
                    for (int j = 0; j < n_teams / 2; j++)
                    {
                        rounds[count].matchweeks[i] = sol[i];
                        // rounds[count].matchweeks[i].matches[j].away.name = sol[i].matches[j].away.name;
                    }
                    count++;
                }

                compute(sol, teams, n, n_teams, n_matches);
                printf("%d\r", count);
            }
            valid = 1;
        }
        return count;
    }

    for (i = 0; i < n; i++)
    {
        if (mark[i] == 0)
        {
            mark[i] = 1;
            sol[pos] = val[i];
            count = PermuteAndCompute(pos + 1, val, sol, mark, n, count, teams, n_teams, n_matches, rounds, rounds_size);
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
                if (goal(round[i].matches[j].home.firstRound[k]) > goal(round[i].matches[j].away.firstRound[k]))
                {
                    for (int m = 0; m < n_teams; m++)
                    {
                        strcmp(round[i].matches[j].home.name, teams[m].name) == 0 ? teams[m].points += 3 : 1;
                    }
                }
                else if (goal(round[i].matches[j].home.firstRound[k]) < goal(round[i].matches[j].away.firstRound[k]))
                {
                    for (int m = 0; m < n_teams; m++)
                    {
                        strcmp(round[i].matches[j].away.name, teams[m].name) == 0 ? teams[m].points += 3 : 1;
                    }
                }
                else
                {
                    for (int m = 0; m < n_teams; m++)
                    {
                        strcmp(round[i].matches[j].home.name, teams[m].name) == 0 ? teams[m].points += 1 : 1;
                        strcmp(round[i].matches[j].away.name, teams[m].name) == 0 ? teams[m].points += 1 : 1;
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
            if (teams[j].points > teams[j + 1].points)
            {
                tmp = teams[j];
                teams[j] = teams[j + 1];
                teams[j + 1] = tmp;
            }
        }
    }

    teams[n_teams - 1].first++;
    teams[n_teams - 2].second++;
    teams[n_teams - 3].third++;

    for (int i = 0; i < n_teams; i++)
    {
        teams[i].points = 0;
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

int filter(int pos, Matchweek *val, Matchweek *sol, int n, int size, int n_teams, int count)
{
    int i;
    if (pos >= n)
    {
        // printf("%d\r", count);
        return count + 1;
    }

    for (i = pos; i < n; i++)
    {
        if (check_valid_rounds(val[i], sol, n_teams, size))
        {
            sol[size] = val[i];
            count = filter(pos + 1, val, sol, n, size + 1, n_teams, count);
            // printf("%d\r", count);
        }
    }

    return count;
}

int check_valid_rounds(Matchweek val, Matchweek *sol, int n_teams, int size)
{
    int count_invalid = 0;
    int valid = 1;
    for (int m = 0; m < size; m++)
    {
        for (int h = 0; h < n_teams / 2; h++)
        {
            // Scan matches of a given matchweek
            for (int k = 0; k < n_teams / 2 && valid; k++)
            {
                // Scan matches of a given matchweek inside the set
                if ((strcmp(val.matches[h].home.name, sol[m].matches[k].home.name) == 0 &&
                     strcmp(val.matches[h].away.name, sol[m].matches[k].away.name) == 0) ||
                    (strcmp(val.matches[h].home.name, sol[m].matches[k].away.name) == 0 &&
                     strcmp(val.matches[h].away.name, sol[m].matches[k].home.name) == 0))
                {
                    // Matchweek already in the set
                    count_invalid++;
                    valid = 0;
                }
            }
        }
    }
    if (count_invalid == n_teams / 2)
    {
        return 0;
    }
    return 1;
}

int compare_Matchweeks(Matchweek val1, Matchweek val2, int n_match)
{
    int count_equal = 0;
    for (int i = 0; i < n_match; i++)
    {
        // printf("%s - %s\n", val1.matches[i].home.name, val1.matches[i].away.name);
        // printf("%s - %s\n", val2.matches[i].home.name, val2.matches[i].away.name);
        // printf("\n\n");
        if ((strcmp(val1.matches[i].home.name, val2.matches[i].home.name) == 0 &&
             strcmp(val1.matches[i].away.name, val2.matches[i].away.name) == 0) ||
            (strcmp(val1.matches[i].home.name, val2.matches[i].away.name) == 0 &&
             strcmp(val1.matches[i].away.name, val2.matches[i].home.name) == 0))
        {
            count_equal++;
        }
    }

    if (count_equal == n_match)
    {
        return 0;
    }
    return 1;
}