#ifndef SCHEDULE
#define SCHEDULE

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
int comb_sempl_matchweek(int pos, Match *val, Match *sol, int n, int k, int start, int count, Matchweek *saved);

void filterMatchweeks(Matchweek *mw, int n, int n_teams);
int perm_sempl_round(int pos, Matchweek *val, Matchweek *sol, int *mark, int n, int count);

int validate(Match *sol, int n_matches);

#endif