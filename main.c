//
// Created by pepperide on 17/04/23.
//

#include "Team.h"
#include "Match.h"
#include "Matchweek.h"
#include "Round.h"
#include <stdio.h>
#include <stdlib.h>

Team* readTeams_fromFile(char *filename, int n);
int main (int argc, char **argv){
    Team *teams;
    Match *matches;
    Matchweek *matchweeks;
    int n;
    int n_combination;

    if(argc!=3){
        fprintf(stderr,"Wrong number of parameters!\n");
        exit(1);
    }

    n = atoi(argv[1]);
    if(n>12){
        fprintf(stderr,"Participants must be between 4 and 8\n");
        exit(1);
    }

    teams = readTeams_fromFile(argv[2],n);
    matches = generatePossibleMatches_wrapper(teams,n);
    matchweeks = generatePossibleMatchweeks_wrapper(matches,n);
    generatePossibleRounds_wrapper(teams,matchweeks,n,MAX_ROUND8);

    n_combination = getComputedRounds(n);
    printf("\nTEAM NAME           \t\tFIRST PLACE\t\tSECOND PLACE\t\tTHIRD PLACE\n");
    for (int i = 0; i < n; i++){
        printf("%-20.20s\t\t%10.5f%%\t\t%11.5f%%\t\t%10.5f%%\n", teams[i].name, (teams[i].podium[0] / (float)n_combination) * 100, (teams[i].podium[1] / (float)n_combination) * 100, (teams[i].podium[2] / (float)n_combination) * 100);
    }
    return 0;
}

Team* readTeams_fromFile(char *filename, int n){
    Team *t;
    int size,match;
    FILE *fp;

    t=(Team*)malloc(n*sizeof(Team));
    for(int i=0;i<n;i++){
        initTeam(&t[i]);
    }

    fp=fopen(filename,"r");
    fscanf(fp,"%d",&match);
    size=0;
    while(fscanf(fp,"%s",t[size].name)!=EOF){
        for(int i=0;i<match;i++){
            fscanf(fp,"%f",&t[size].results[i]);
            t[size].sum += t[size].results[i];
        }
        t[size].id = size;
        t[size].played_matches = match;
        size++;
    }

    fclose(fp);
    return t;
}