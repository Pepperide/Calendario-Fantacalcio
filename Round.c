//
// Created by pepperide on 19/04/23.
//

#include "Round.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getComputedRounds(int n){
    switch(n){
        case 4:
            return MAX_ROUND4;
        case 6:
            return MAX_ROUND6;
        case 8:
            return MAX_ROUND8;
        case 10:
            return MAX_ROUND10;
        case 12:
            return MAX_ROUND12;
        default:
            fprintf(stderr,"Participants must be between 4 and 12\n");
            exit(1);
    }
}

void generatePossibleRounds_wrapper(Team *teams, Matchweek *matchweeks, int n_teams, int limit){
    Matchweek *sol;
    int count;
    int n_matchweek_computed = getComputedMatchweeks(n_teams);

    sol = (Matchweek *)malloc((n_teams - 1) * sizeof(Matchweek)); // ?malloc also matches?

    count = generatePossibleRounds(0, matchweeks, sol, n_matchweek_computed, n_teams - 1, 0, 0,teams,n_teams, limit);
    printf("Generated %d rounds\n", count);
    printf("\n");
}

int generatePossibleRounds(int pos, Matchweek *val, Matchweek *sol, int n, int k, int start, int count, Team *teams, int n_teams, int limit){
    int i, new_size=0;
    if (pos >= k){
        if(count>=limit){
            return count;
        }
        // For each round set you have to permute
        count = permuteMatchweeks_wrapper(sol, k, count,teams,n_teams);
        return count;
    }
    for (i = start; i < n && count<limit; i++){
        sol[pos] = val[i];
        new_size=0;
        Matchweek *new_matchweeks = pruneAvailableMatchweekSet(val, sol,&new_size, n, pos,i+1, (k+1)/2);
        count = generatePossibleRounds(pos + 1, new_matchweeks, sol, new_size, k, 0, count, teams,n_teams, limit);
    }
    return count;
}

Matchweek * pruneAvailableMatchweekSet(Matchweek* old, Matchweek* current, int* new_size, int n, int current_size, int start, int n_match){
    Matchweek *new;
    new = (Matchweek*)malloc(n*sizeof(Matchweek));

    for(int j=start;j<n;j++){
        if(!containTwiceMatch(old[j],current[current_size],n_match))
            new[(*new_size)++] = old[j];
    }
    return new;
}

int permuteMatchweeks_wrapper(Matchweek *val, int n_matchweeks, int count, Team *teams, int n_teams){
    Matchweek *sol;
    int *mark;

    sol = (Matchweek *)malloc(n_matchweeks * sizeof(Matchweek));
    mark = (int *)calloc(n_matchweeks, sizeof(int));
    count = permuteMatchweeks(0, val, sol, mark, n_matchweeks, count, teams, n_teams);
    return count;
}

int permuteMatchweeks(int pos, Matchweek *val, Matchweek *sol, int *mark, int n, int count, Team *teams, int n_teams){
    int i;
    if (pos >= n){
        compute(sol,teams,n,n_teams,n_teams/2);
        return count + 1;
    }
    for (i = 0; i < n; i++){
        if (mark[i] == 0){
            mark[i] = 1;
            sol[pos] = val[i];
            count = permuteMatchweeks(pos + 1, val, sol, mark, n, count, teams, n_teams);
            mark[i] = 0;
        }
    }
    return count;
}

void compute(Matchweek *round, Team *teams, int n_matchweek, int n_teams, int n_matches){
    for (int i = 0; i < n_matchweek; i++){
        // For each matchweek
        for (int j = 0; j < n_teams / 2; j++){
            // For each match of the matchweek
            for (int k = i; k < n_matches; k += (n_teams - 1)){
                if (goal(round[i].matches[j].home.results[k]) > goal(round[i].matches[j].away.results[k])){
                    for (int m = 0; m < n_teams; m++){
                        strcmp(round[i].matches[j].home.name, teams[m].name) == 0 ? teams[m].sum += 3 : 1;
                    }
                }
                else if (goal(round[i].matches[j].home.results[k]) < goal(round[i].matches[j].away.results[k])){
                    for (int m = 0; m < n_teams; m++){
                        strcmp(round[i].matches[j].away.name, teams[m].name) == 0 ? teams[m].sum += 3 : 1;
                    }
                }
                else{
                    for (int m = 0; m < n_teams; m++){
                        strcmp(round[i].matches[j].home.name, teams[m].name) == 0 ? teams[m].sum += 1 : 1;
                        strcmp(round[i].matches[j].away.name, teams[m].name) == 0 ? teams[m].sum += 1 : 1;
                    }
                }
            }
        }
    }
    Team tmp;
    for (int i = 0; i < n_teams - 1; i++){
        for (int j = 0; j < n_teams - 1; j++){
            if (teams[j].sum > teams[j + 1].sum){
                tmp = teams[j];
                teams[j] = teams[j + 1];
                teams[j + 1] = tmp;
            }
        }
    }
    teams[n_teams - 1].podium[0]++;
    teams[n_teams - 2].podium[1]++;
    teams[n_teams - 3].podium[2]++;

    for (int i = 0; i < n_teams; i++){
        teams[i].sum = 0;
    }
}

int goal(float points){
    float base = 66.0;
    int goals = 1;
    if (points < base)
        return 0;

    while (base + 4.0 < points){
        base += 4.0;
        goals++;
    }
    return goals;
}