#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Team
{
    char *name;
} Team;

typedef struct
{
    Team home;
    Team away;
} Match;

typedef struct
{
    Match *matches;
} Matchweek;

typedef struct
{
    Matchweek matchweek[3];
} Round;

void generateSchedules(Team *teams, int n_teams);
int comb_sempl_match(int pos, Team *val, Team *sol, int n, int k, int start, int count, Match *saved);
int comb_sempl_week(int pos, Match *val, Match *sol, int n, int k, int start, int count, Matchweek *saved);
int perm_sempl_round(int pos, Matchweek *val, Matchweek *sol, int *mark, int n, int count);
void printMatch(Match *matches, int size);
void printMatchWeek(Matchweek *mw, int size, int n_matches);
int validate(Match *sol, int n_matches);

int main()
{
    Team teams[10];

    teams[0] = (Team){"A"};
    teams[1] = (Team){"B"};
    teams[2] = (Team){"C"};
    teams[3] = (Team){"D"};
    teams[4] = (Team){"E"};
    teams[5] = (Team){"F"};
    teams[6] = (Team){"G"};
    teams[7] = (Team){"H"};
    teams[8] = (Team){"I"};
    teams[9] = (Team){"J"};

    int n_teams = (sizeof(teams) / sizeof(Team));
    generateSchedules(teams, n_teams);

    return 0;
}

void generateSchedules(Team *teams, int n_teams)
{
    Team teams_sol[2];    // Internal variable used for computing team combinations
    Match *matchweek_sol; // Internal variable for computing match combination
    Matchweek *round_sol;
    int *mark;

    Match *possible_team_pairs;
    Matchweek *possible_matchweeks;
    Matchweek *matchweek_set;

    int count;
    int n_matchweeks;
    int found = 0;
    int size = 0;

    switch (n_teams)
    {
    case 4:
        n_matchweeks = 3;
        possible_team_pairs = malloc(6 * sizeof(Match));                // Mathematically computed
        possible_matchweeks = malloc(n_matchweeks * sizeof(Matchweek)); // Mathematically computed
        matchweek_set = malloc(n_matchweeks * sizeof(Matchweek));       // Mathematically computed
        round_sol = malloc(n_matchweeks * sizeof(Matchweek));
        mark = malloc(n_matchweeks * sizeof(int));
        for (int i = 0; i < 3; i++)
        {
            possible_matchweeks[i].matches = malloc(n_teams / 2 * sizeof(Match));
            matchweek_set[i].matches = malloc(n_teams / 2 * sizeof(Match));
            round_sol[i].matches = malloc(n_teams / 2 * sizeof(Match));
            mark[i] = 0;
        }
        break;
    case 6:
        n_matchweeks = 15;
        possible_team_pairs = malloc(15 * sizeof(Match));               // Mathematically computed
        possible_matchweeks = malloc(n_matchweeks * sizeof(Matchweek)); // Mathematically computed
        matchweek_set = malloc(n_matchweeks * sizeof(Matchweek));       // Mathematically computed
        round_sol = malloc(n_matchweeks * sizeof(Matchweek));
        mark = malloc(n_matchweeks * sizeof(int));
        for (int i = 0; i < n_matchweeks; i++)
        {
            possible_matchweeks[i].matches = malloc(n_teams / 2 * sizeof(Match));
            matchweek_set[i].matches = malloc(n_teams / 2 * sizeof(Match));
            round_sol[i].matches = malloc(n_teams / 2 * sizeof(Match));
            mark[i] = 0;
        }

        break;
    case 8:
        n_matchweeks = 105;
        possible_team_pairs = malloc(28 * sizeof(Match));               // Mathematically computed
        possible_matchweeks = malloc(n_matchweeks * sizeof(Matchweek)); // Mathematically computed
        matchweek_set = malloc(n_matchweeks * sizeof(Matchweek));       // Mathematically computed
        round_sol = malloc(n_matchweeks * sizeof(Matchweek));
        mark = malloc(n_matchweeks * sizeof(int));
        for (int i = 0; i < n_matchweeks; i++)
        {
            possible_matchweeks[i].matches = malloc(n_teams / 2 * sizeof(Match));
            matchweek_set[i].matches = malloc(n_teams / 2 * sizeof(Match));
            round_sol[i].matches = malloc(n_teams / 2 * sizeof(Match));
            mark[i] = 0;
        }
        break;
    case 10:
        n_matchweeks = 945;
        possible_team_pairs = malloc(45 * sizeof(Match));               // Mathematically computed
        possible_matchweeks = malloc(n_matchweeks * sizeof(Matchweek)); // Mathematically computed
        matchweek_set = malloc(n_matchweeks * sizeof(Matchweek));       // Mathematically computed
        round_sol = malloc(n_matchweeks * sizeof(Matchweek));
        mark = malloc(n_matchweeks * sizeof(int));
        for (int i = 0; i < n_matchweeks; i++)
        {
            possible_matchweeks[i].matches = malloc(n_teams / 2 * sizeof(Match));
            matchweek_set[i].matches = malloc(n_teams / 2 * sizeof(Match));
            round_sol[i].matches = malloc(n_teams / 2 * sizeof(Match));
            mark[i] = 0;
        }
        break;
    default:
        return;
    };
    matchweek_sol = malloc((n_teams / 2) * sizeof(Match));

    count = comb_sempl_match(0, teams, teams_sol, n_teams, 2, 0, 0, possible_team_pairs);
    // printf("Combinazioni: %d\n", count);
    //  printMatch(possible_team_pairs, count);

    count = comb_sempl_week(0, possible_team_pairs, matchweek_sol, count, n_teams / 2, 0, 0, possible_matchweeks);
    printf("Combinazioni: %d\n", count);
    // printMatchWeek(possible_matchweeks, count, n_teams / 2);

    count = 0;
    for (int l = 0; l < n_matchweeks; l++)
    {
        for (int i = 0; i < n_matchweeks; i++)
        {
            matchweek_set[i].matches = (void *)0;
            // free(matchweek_set[i].matches);
        }
        for (int i = 0; i < n_matchweeks; i++)
        {
            matchweek_set[i].matches = malloc(n_teams / 2 * sizeof(Match));
        }
        size = 0;
        found = 0;
        matchweek_set[size++].matches = possible_matchweeks[l].matches;

        for (int i = 0; i < n_matchweeks; i++)
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
                        if ((strcmp(possible_matchweeks[i].matches[h].home.name, matchweek_set[j].matches[k].home.name) == 0 &&
                             strcmp(possible_matchweeks[i].matches[h].away.name, matchweek_set[j].matches[k].away.name) == 0) ||
                            (strcmp(possible_matchweeks[i].matches[h].home.name, matchweek_set[j].matches[k].away.name) == 0 &&
                             strcmp(possible_matchweeks[i].matches[h].away.name, matchweek_set[j].matches[k].home.name) == 0))
                        {
                            // Matchweek already in the set
                            found = 1;
                        }
                    }
                }
            }
            if (!found)
            {
                matchweek_set[size++].matches = possible_matchweeks[i].matches;
            }
            found = 0;
        }
        count = perm_sempl_round(0, matchweek_set, round_sol, mark, n_teams - 1, count);
        // printMatchWeek(matchweek_set, size, n_teams / 2);
        // count = count + 1;
    }
    // printf("Combinazioni: %d\n", count);
    printf("Permutazioni: %d\n", count);
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

int comb_sempl_week(int pos, Match *val, Match *sol, int n, int k, int start, int count, Matchweek *saved)
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
            printf("%s - %s", saved[count].matches[i].home.name, saved[count].matches[i].away.name);
            i != k - 1 ? printf(" --- ") : 0;
        }
        printf("\n");

        return count + 1;
    }
    for (i = start; i < n; i++)
    {
        sol[pos].home = val[i].home;
        sol[pos].away = val[i].away;
        count = comb_sempl_week(pos + 1, val, sol, n, k, i + 1, count, saved);
    }
    return count;
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

void printMatch(Match *matches, int size)
{
    // for (int i = 0; i < size; i++)
    // {
    //     printf("%s - %s\n", matches[i].home, matches[i].away);
    // }
    printf("Combinazioni: %d\n", size);
}

void printMatchWeek(Matchweek *mw, int size, int n_matches)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < n_matches; j++)
        {
            printf("%s - %s", mw[i].matches[j].home.name, mw[i].matches[j].away.name);
            j != n_matches - 1 ? printf(" --- ") : 0;
            // printf("%s - %s --- ", mw[i].matches[0].home, mw[i].matches[0].away);
            // printf("%s - %s\n", mw[i].matches[1].home, mw[i].matches[1].away);
        }
        printf("\n");
    }
    printf("Combinazioni: %d\n", size);
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
