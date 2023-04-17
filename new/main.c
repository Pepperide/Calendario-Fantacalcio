//
// Created by pepperide on 17/04/23.
//

#include "Team.h"
#include "Match.h"
#include "Matchweek.h"
#include <stdio.h>
#include <stdlib.h>

Team* readTeams_fromFile(char *filename, int n);
int main (int argc, char **argv){
    Team *teams;
    Match *matches;
    Matchweek *matchweeks;
    int n;

    if(argc!=3){
        fprintf(stderr,"Wrong number of parameters!\n");
        exit(1);
    }

    n = atoi(argv[1]);

    teams = readTeams_fromFile(argv[2],n);
    matches = generatePossibleMatches_wrapper(teams,n);
    matchweeks = generatePossibleMatchweeks_wrapper(matches,15,n/2);

    return 0;
}

Team* readTeams_fromFile(char *filename, int n){
    Team *t;
    int size;
    FILE *fp;

    t=(Team*)malloc(n*sizeof(Team));
    for(int i=0;i<n;i++){
        init(&t[i]);
    }

    fp=fopen(filename,"r");
    size=0;
    while(fscanf(fp,"%s",t[size].name)!=EOF){
        size++;
    }

    fclose(fp);
    return t;
}