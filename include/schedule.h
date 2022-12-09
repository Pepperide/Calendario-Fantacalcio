#ifndef SCHEDULECOMP
#define SCHEDULECOMP

typedef struct Team
{
    char *name;
    float *mw_points;
    int total_points;
    int podium[3];
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

// Read the file and populate Team structure
Team *readFile(char *file, int *n_teams, int *n_matches_played);

// Main function that handles schedule generation
void generateSchedules(Team *teams, int n_teams, int max_calendars);

// 2-combination algorithm. It computes the possible matches of the teams' set
void compute_possible_matches_wrapper(Team *val, Match *matches, int n_teams);
int compute_possible_matches(int pos, Team *val, Team *sol, Match *saved, int n, int k, int start, int count);

// k-combination algorithm where k is the (n_teams/2). It computes the possible matchweeks of the matches' set
void compute_possible_matchweeks_wrapper(Match *val, Matchweek *matchweeks, int n_teams, int n_matches);
int compute_possible_matchweeks(int pos, Match *val, Match *sol, Matchweek *saved, int n, int k, int start, int count);
int valid_matchweek(Match *sol, int n_matches);

// k-combination + permutation algorithms where k is the (n_teams-1). It computes the possible rounds
void compute_possible_rounds_wrapper(Matchweek *val, Round *rounds, int n_teams, int n_matches, int n_matchweeks, int max_calendars, Team *teams);
int compute_possible_rounds(int pos, Matchweek *val, Matchweek *sol, Round *saved, int n, int k, int start, int count, int n_matches, int max_calendars, Team *teams, int n_teams);
int valid_round(Round *rounds, Matchweek *sol, int n_matches, int n_matchweeks, int n_rounds);
int permute_wrapper(Round *saved, Matchweek *val, int n_matchweeks, int count, int max_calendars, Team *teams, int n_teams);
int permute(int pos, Matchweek *val, Matchweek *sol, int *mark, int n, int count, Round *saved, int max_calendars, Team *teams, int n_teams);

void compute(Matchweek *round, Team *teams, int n_matchweek, int n_teams, int n_matches);
int goal(float points);
#endif