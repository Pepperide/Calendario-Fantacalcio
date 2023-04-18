//
// Created by pepperide on 17/04/23.
//

#include "Matchweek.h"
#include <stdio.h>
#include <stdlib.h>

Matchweek* initMatchweeksByParticipants(int n){
    Matchweek* m;
    switch(n){
        case 4:
            m = (Matchweek*)malloc(MAX_MATCHWEEKS4*sizeof(Matchweek));
            for(int i=0;i<MAX_MATCHWEEKS4;i++)
                m[i].matches=(Match*)malloc(MAX_MATCHES4*sizeof(Match));
            break;
        case 6:
            m = (Matchweek*)malloc(MAX_MATCHWEEKS6*sizeof(Matchweek));
            for(int i=0;i<MAX_MATCHWEEKS6;i++)
                m[i].matches=(Match*)malloc(MAX_MATCHES6*sizeof(Match));
            break;
        case 8:
            m = (Matchweek*)malloc(MAX_MATCHWEEKS8*sizeof(Matchweek));
            for(int i=0;i<MAX_MATCHWEEKS8;i++)
                m[i].matches=(Match*)malloc(MAX_MATCHES8*sizeof(Match));
            break;
        case 10:
            m = (Matchweek*)malloc(MAX_MATCHWEEKS10*sizeof(Matchweek));
            for(int i=0;i<MAX_MATCHWEEKS10;i++)
                m[i].matches=(Match*)malloc(MAX_MATCHES10*sizeof(Match));
            break;
        case 12:
            m = (Matchweek*)malloc(MAX_MATCHWEEKS12*sizeof(Matchweek));
            for(int i=0;i<MAX_MATCHWEEKS12;i++)
                m[i].matches=(Match*)malloc(MAX_MATCHES12*sizeof(Match));
            break;
        default:
            fprintf(stderr,"Participants must be between 4 and 12\n");
            exit(1);
    }
    return m;
}

Matchweek* generatePossibleMatchweeks_wrapper(Match *matches, int n_teams){
    Matchweek *matchweeks;
    Match *sol;
    int n_matches_computed = getComputedMatches(n_teams);
    int n_matchweeks;

    sol = (Match *)malloc(n_teams/2 * sizeof(Match));
    matchweeks= initMatchweeksByParticipants(n_teams);

    n_matchweeks = generatePossibleMatchweeks(0, matches, sol, matchweeks, n_matches_computed, n_teams/2, 0, 0);

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
        Match *new_matches = pruneAvailableSet(val, sol,&new_size, n, pos,i+1);
        count = generatePossibleMatchweeks(pos + 1, new_matches, sol, saved, new_size, k, 0, count);
    }
    return count;
}

Match* pruneAvailableSet(Match* old, Match* current, int* new_size, int n, int current_size,int start){
    Match *new;
    new = (Match*)malloc(n*sizeof(Match));

        for(int j=start;j<n;j++){
            if(!containTwiceTeams(old[j],current[current_size]))
                new[(*new_size)++] = old[j];
        }
    return new;
}