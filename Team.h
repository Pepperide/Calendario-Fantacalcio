//
// Created by pepperide on 17/04/23.
//

#ifndef CALENDARIO_FANTACALCIO_TEAM_H
#define CALENDARIO_FANTACALCIO_TEAM_H

#define NAMELEN 50

typedef struct team{
    int id;
    char *name;
    float *results;
    int played_matches;
    int sum;
    int podium[3];
} Team;

void initTeam(Team *t);

void printTeam(Team t);

#endif //CALENDARIO_FANTACALCIO_TEAM_H
