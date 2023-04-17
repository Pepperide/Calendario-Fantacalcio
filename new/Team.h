//
// Created by pepperide on 17/04/23.
//

#ifndef CALENDARIO_FANTACALCIO_TEAM_H
#define CALENDARIO_FANTACALCIO_TEAM_H

typedef struct team{
    int id;
    char *name;
} Team;

void init(Team *t);

void printTeam(Team t);

#endif //CALENDARIO_FANTACALCIO_TEAM_H
