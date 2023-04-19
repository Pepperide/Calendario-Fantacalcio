//
// Created by pepperide on 17/04/23.
//

#include "Match.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Match* initMatchesByParticipants(int n){
    Match* matches;
    switch(n){
        case 4:
            matches = (Match*)malloc(MAX_MATCHES4*sizeof(Match));
            break;
        case 6:
            matches = (Match*)malloc(MAX_MATCHES6*sizeof(Match));
            break;
        case 8:
            matches = (Match*)malloc(MAX_MATCHES8*sizeof(Match));
            break;
        case 10:
            matches = (Match*)malloc(MAX_MATCHES10*sizeof(Match));
            break;
        case 12:
            matches = (Match*)malloc(MAX_MATCHES12*sizeof(Match));
            break;
        default:
            fprintf(stderr,"Participants must be between 4 and 12\n");
            exit(1);
    }
    return matches;
}

int getComputedMatches(int n){
    switch(n){
        case 4:
            return MAX_MATCHES4;
        case 6:
            return MAX_MATCHES6;
        case 8:
            return MAX_MATCHES8;
        case 10:
            return MAX_MATCHES10;
        case 12:
            return MAX_MATCHES12;
        default:
            fprintf(stderr,"Participants must be between 4 and 12\n");
            exit(1);
    }
}

Match* generatePossibleMatches_wrapper(Team* teams, int n){
    Match* matches;
    Team tmp[2];
    //int size = 15;
    int n_matches;

    //matches = (Match*)malloc(size*sizeof(Match));
    matches = initMatchesByParticipants(n);

    n_matches= generatePossibleMatches(0,teams,tmp,matches,n,2,0,0);

    printf("Generated %d matches\n",n_matches);
//    for(int i=0;i<n_matches;i++){
//        printMatch(matches[i]);
//    }

    return matches;
}
int generatePossibleMatches(int pos, Team *val, Team *sol, Match *matches, int n, int k, int start, int count){
    int i;
    if (pos >= k){
        matches[count].home = sol[0];
        matches[count].away = sol[1];
        return count + 1;
    }

    for (i = start; i < n; i++){
        sol[pos] = val[i];
        count = generatePossibleMatches(pos + 1, val, sol, matches, n, k, i + 1, count);
    }
    return count;
}

void printMatch(Match m){
    printf("%s - %s\n",m.home.name,m.away.name);
}

int containTwiceTeams(Match m1, Match m2){
    if( strcmp(m1.home.name,m2.home.name)==0 ||
        strcmp(m1.home.name,m2.away.name)==0 ||
        strcmp(m1.away.name,m2.home.name)==0 ||
        strcmp(m1.away.name,m2.away.name)==0){
        return 1;
    }
    return 0;
}