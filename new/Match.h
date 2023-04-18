//
// Created by pepperide on 17/04/23.
//

#ifndef CALENDARIO_FANTACALCIO_MATCH_H
#define CALENDARIO_FANTACALCIO_MATCH_H

#include "Team.h"
#define MAX_MATCHES4 6
#define MAX_MATCHES6 15
#define MAX_MATCHES8 28
#define MAX_MATCHES10 45
#define MAX_MATCHES12 66

typedef struct match{
    Team home;
    Team away;
} Match;

Match* initMatchesByParticipants(int n);
int getComputedMatches(int n);

Match* generatePossibleMatches_wrapper(Team* teams, int n);
int generatePossibleMatches(int pos, Team *val, Team *sol, Match *matches, int n, int k, int start, int count);

void printMatch(Match m);
int containTwiceTeams(Match m1, Match m2);

#endif //CALENDARIO_FANTACALCIO_MATCH_H
