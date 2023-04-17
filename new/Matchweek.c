//
// Created by pepperide on 17/04/23.
//

#include "Matchweek.h"
#include <stdio.h>
#include <stdlib.h>

Matchweek* generatePossibleMatchweeks_wrapper(Match *matches, int n_matches_computed, int n_match){
    Matchweek *matchweeks;
    Match *sol;
    int n_matchweeks;

    sol = (Match *)malloc(n_match * sizeof(Match));
    //To be changed
    matchweeks=(Matchweek*)malloc(MAXMATCHWEEKS*sizeof(Matchweek));
    for(int i=0;i<MAXMATCHWEEKS;i++)
        matchweeks[i].matches=(Match*)malloc(n_match*sizeof(Match));

    n_matchweeks = generatePossibleMatchweeks(0, matches, sol, matchweeks, n_matches_computed, n_match, 0, 0);

    printf("Generated %d matchweeks\n", n_matchweeks);
    return matchweeks;
}

int generatePossibleMatchweeks(int pos, Match *val, Match *sol, Matchweek *saved, int n, int k, int start, int count)
{
    int i,new_size=0;
    if (pos >= k){
        for (i = 0; i < k; i++){
            saved[count].matches[i] = sol[i];
        }
        return count + 1;
    }

    for (i = 0; i < n; i++){
        sol[pos].home = val[i].home;
        sol[pos].away = val[i].away;
        new_size=0;
        Match *new_matches = pruneAvailableSet(val, sol,&new_size, n, pos);
        count = generatePossibleMatchweeks(pos + 1, new_matches, sol, saved, new_size, k, 0, count);
    }
    return count;
}

Match* pruneAvailableSet(Match* old, Match* current, int* new_size, int n, int current_size){
    Match *new;
    new = (Match*)malloc(n*sizeof(Match));

        for(int j=0;j<n;j++){
            if(!containTwiceTeams(old[j],current[current_size]))
                new[(*new_size)++] = old[j];
        }
    return new;
}