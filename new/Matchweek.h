//
// Created by pepperide on 17/04/23.
//

#ifndef CALENDARIO_FANTACALCIO_MATCHWEEK_H
#define CALENDARIO_FANTACALCIO_MATCHWEEK_H

#include "Match.h"
#define MAXMATCHWEEKS 90

typedef struct matchweek{
    Match *matches;
} Matchweek;

Matchweek* generatePossibleMatchweeks_wrapper(Match *matches, int n_matches_computed, int n_match);
int generatePossibleMatchweeks(int pos, Match *val, Match *sol, Matchweek *saved, int n, int k, int start, int count);

Match * pruneAvailableSet(Match* old, Match* current, int* new_size, int n, int current_size);
#endif //CALENDARIO_FANTACALCIO_MATCHWEEK_H
