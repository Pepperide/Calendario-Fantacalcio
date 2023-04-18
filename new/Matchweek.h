//
// Created by pepperide on 17/04/23.
//

#ifndef CALENDARIO_FANTACALCIO_MATCHWEEK_H
#define CALENDARIO_FANTACALCIO_MATCHWEEK_H

#include "Match.h"
#define MAX_MATCHWEEKS4 3
#define MAX_MATCHWEEKS6 15
#define MAX_MATCHWEEKS8 105
#define MAX_MATCHWEEKS10 945
#define MAX_MATCHWEEKS12 10395

typedef struct matchweek{
    Match *matches;
} Matchweek;

Matchweek* initMatchweeksByParticipants(int n);

Matchweek* generatePossibleMatchweeks_wrapper(Match *matches, int n_teams);
int generatePossibleMatchweeks(int pos, Match *val, Match *sol, Matchweek *saved, int n, int k, int start, int count);

Match * pruneAvailableSet(Match* old, Match* current, int* new_size, int n, int current_size, int start);
#endif //CALENDARIO_FANTACALCIO_MATCHWEEK_H
