#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/schedule.h"

int main()
{
    Team teams[6];

    teams[0] = (Team){"A"};
    teams[1] = (Team){"B"};
    teams[2] = (Team){"C"};
    teams[3] = (Team){"D"};
    teams[4] = (Team){"E"};
    teams[5] = (Team){"F"};
    // teams[6] = (Team){"G"};
    // teams[7] = (Team){"H"};
    // teams[8] = (Team){"I"};
    // teams[9] = (Team){"J"};

    int n_teams = (sizeof(teams) / sizeof(Team));
    generateSchedules(teams, n_teams);

    return 0;
}