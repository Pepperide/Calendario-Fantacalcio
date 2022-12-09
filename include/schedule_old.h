#ifndef SCHEDULE
#define SCHEDULE

typedef struct Team
{
    char *name;
    float *firstRound;
    int n_matches_first;
    float *secondRound;
    int n_matches_second;
    int points;
    int first;
    int second;
    int third;
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
    Matchweek *matchweeks;
} Round;

Team *readFile(char *file, int *n_teams, int *n_matches_played);

void generateSchedules(Team *teams, int n_teams, int n_effective_matches);
int comb_sempl_match(int pos, Team *val, Team *sol, int n, int k, int start, int count, Match *saved);
int comb_sempl_matchweek(int pos, Match *val, Match *sol, int n, int k, int start, int count, Matchweek *saved);

void filterMatchweeks(Matchweek *mw, int n, Team *teams, int n_teams, int n_effective_matches);
int PermuteAndCompute(int pos, Matchweek *val, Matchweek *sol, int *mark, int n, int count, Team *teams, int n_teams, int n_matches, Round *rounds, int rounds_size);
void compute(Matchweek *round, Team *teams, int n_matchweek, int n_teams, int n_matches);
int goal(float points);

int validate(Match *sol, int n_matches);
int check_valid_rounds(Matchweek val, Matchweek *sol, int n_teams, int size);
int filter(int pos, Matchweek *val, Matchweek *sol, int n, int size, int n_teams, int count);

int compare_Matchweeks(Matchweek val1, Matchweek val2, int n_match);

#endif