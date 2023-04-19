//
// Created by pepperide on 17/04/23.
//

#include "Team.h"
#include <stdlib.h>
#include <stdio.h>

void initTeam(Team *t){
    t->name = (char *)malloc(NAMELEN*sizeof(char));
    t->results=(float *)malloc(38*sizeof(float));
    t->sum = 0;
    t->podium[0] = 0;
    t->podium[1] = 0;
    t->podium[2] = 0;
}

void printTeam(Team t){
    printf("%s\n",t.name);
}