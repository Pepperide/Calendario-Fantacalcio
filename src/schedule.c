#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/schedule.h"

void generateSchedules(Team *teams, int n_teams)
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

    filterMatchweeks(possible_matchweeks, n_matchweeks, n_teams);
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
        for (int j = i + 1; j < n_matches; j++)
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
    return 0;
};

void filterMatchweeks(Matchweek *mw, int n, int n_teams)
{
    Matchweek *matchweek_set; // Filtered matchweeks which are compatible to be in a round
    Matchweek *sol;
    int *mark;

    int size = 0;
    int found = 0;
    int count = 0;

    matchweek_set = malloc(n * sizeof(Matchweek));
    sol = malloc(n * sizeof(Matchweek));
    mark = malloc(n * sizeof(int));

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
        found = 0;
        matchweek_set[size++].matches = mw[l].matches;

        for (int i = 0; i < n; i++)
        {
            // Scan all the combinations
            for (int j = 0; j < size; j++)
            {
                // Scan the current set of combination
                for (int h = 0; h < n_teams / 2 && !found; h++)
                {
                    // Scan matches of a given matchweek
                    for (int k = 0; k < n_teams / 2 && !found; k++)
                    {
                        // Scan matches of a given matchweek inside the set
                        if ((strcmp(mw[i].matches[h].home.name, matchweek_set[j].matches[k].home.name) == 0 &&
                             strcmp(mw[i].matches[h].away.name, matchweek_set[j].matches[k].away.name) == 0) ||
                            (strcmp(mw[i].matches[h].home.name, matchweek_set[j].matches[k].away.name) == 0 &&
                             strcmp(mw[i].matches[h].away.name, matchweek_set[j].matches[k].home.name) == 0))
                        {
                            // Matchweek already in the set
                            found = 1;
                        }
                    }
                }
            }
            if (!found)
            {
                matchweek_set[size++].matches = mw[i].matches;
            }
            found = 0;
        }
        if (size >= n_teams - 1)
        {
            count = perm_sempl_round(0, matchweek_set, sol, mark, n_teams - 1, count);
        }
    }
    printf("Possible rounds: %d\n", count);
}

int perm_sempl_round(int pos, Matchweek *val, Matchweek *sol, int *mark, int n, int count)
{
    int i;
    if (pos >= n)
    {
        // TODO
        printf("%d\r", count + 1);
        return count + 1;
    }
    for (i = 0; i < n; i++)
    {
        if (mark[i] == 0)
        {
            mark[i] = 1;
            sol[pos] = val[i];
            count = perm_sempl_round(pos + 1, val, sol, mark, n, count);
            mark[i] = 0;
        }
    }
    return count;
}