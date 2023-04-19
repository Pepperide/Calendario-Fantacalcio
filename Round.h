//
// Created by pepperide on 19/04/23.
//

#ifndef NEW_ROUND_H
#define NEW_ROUND_H

#include "Matchweek.h"
#define MAX_ROUND4 6
#define MAX_ROUND6 720
#define MAX_ROUND8 31449600
#define MAX_ROUND10 31449600
#define MAX_ROUND12 31449600

typedef struct
{
    Matchweek *matchweeks;
} Round;

Round* initRoundsByParticipants(int n);
int getComputedRounds(int n);

void generatePossibleRounds_wrapper(Team *teams,Matchweek *matchweeks, int n_teams, int limit);
int generatePossibleRounds(int pos, Matchweek *val, Matchweek *sol, int n, int k, int start, int count, Team *teams, int n_teams, int limit);

Matchweek * pruneAvailableMatchweekSet(Matchweek* old, Matchweek* current, int* new_size, int n, int current_size, int start, int n_match);

int permuteMatchweeks_wrapper(Matchweek *val, int n_matchweeks, int count, Team *teams, int n_teams);
int permuteMatchweeks(int pos, Matchweek *val, Matchweek *sol, int *mark, int n, int count, Team *teams, int n_teams);

void compute(Matchweek *round, Team *teams, int n_matchweek, int n_teams, int n_matches);
int goal(float points);
#endif //NEW_ROUND_H
