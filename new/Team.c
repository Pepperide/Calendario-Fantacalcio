//
// Created by pepperide on 17/04/23.
//

#include "Team.h"
#include <stdlib.h>
#include <stdio.h>

void init(Team *t){
    t->name = (char *)malloc(20*sizeof(char));
}

void printTeam(Team t){
    printf("%s\n",t.name);
}