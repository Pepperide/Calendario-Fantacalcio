#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_GIORNATE 18

typedef struct{
    int id;
    char nome[50];
    float punteggio[19];
    float sum;
}Squadra;

typedef struct{
    Squadra A;
    Squadra B;
}Partita;

typedef struct{
    Partita *p;
}Giornata;

void shiftLeft(Squadra *data, Squadra add,int len);
Squadra shiftRight(Squadra *data, Squadra add,int len);
int perm_sempl(int pos,Giornata *val,Giornata *sol,int *mark,int n,int n_squadre, int count);
int perm_sempl_squad(int pos,Squadra *val,Squadra *sol,int *mark,int n,int n_squadre, int count);
Squadra *leggiFile(int n);
void calcola_classifica(Giornata *sol,int n,int n_teams);
int goal(float punteggio);

int *n_wins;
int *n_silver;
int *n_bronze;
Squadra *teams;

int main(int argc,char** argv)
{
    char space[30]=" ";
    int numero_squadre=atoi(argv[1]);
    int giornate=numero_squadre-1;
    Giornata *g;
    Giornata *sol;
    Squadra *sol_squad;
    int *mark;
    int count=0;
    Squadra *casa;
    Squadra *trasferta;
    Squadra *teams_temp;

    g=malloc(giornate*sizeof(Giornata));
    sol=malloc(giornate*sizeof(Giornata));
    for(int i=0;i<giornate;i++){
        g[i].p=malloc((numero_squadre/2)*sizeof(Partita));
        sol[i].p=malloc((numero_squadre/2)*sizeof(Partita));
    }
    mark=calloc(giornate,sizeof(int));
    n_wins=calloc(numero_squadre,sizeof(int));
    n_silver=calloc(numero_squadre,sizeof(int));
    n_bronze=calloc(numero_squadre,sizeof(int));
    casa=malloc(numero_squadre*sizeof(Squadra));
    trasferta=malloc(numero_squadre*sizeof(Squadra));
    teams_temp=malloc(numero_squadre*sizeof(Squadra));
    sol_squad=malloc(numero_squadre*sizeof(Squadra));

    teams=leggiFile(atoi(argv[1]));
    for(int i=0;i<numero_squadre;i++)
        teams_temp[i]=teams[i];

    for(int k=0;k<numero_squadre/2;k++){
        for (int i = 0; i < numero_squadre /2; i++) {
            casa [i] = teams_temp[i];
            trasferta[i] = teams_temp[numero_squadre - 1 - i];
        }

        for (int i = 0; i < giornate; i++) {
            /* Stampa le partite di questa giornata */
            //printf("%d^ Giornata\n",i+1);

            /* Alterna le partite in casa e fuori */
            if ((i % 2) == 0) {
                for (int j = 0; j < numero_squadre /2 ; j++){
                    g[i].p[j].A=trasferta[j];
                    g[i].p[j].B=casa[j];
                    //printf("%d.  %d - %d\n", j+1, trasferta[j], casa[j]);
                }
            }
            else {
                for (int j = 0; j < numero_squadre /2 ; j++){
                    g[i].p[j].A=casa[j];
                    g[i].p[j].B=trasferta[j];
                    //printf("%d.  %d - %d\n", j+1, casa[j], trasferta[j]);
                }
            }

            // Ruota in gli elementi delle liste, tenendo fisso il primo elemento
            // Salva l'elemento fisso
            Squadra pivot = casa [0];

            /* Sposta in avanti gli elementi di "trasferta" inserendo
            all'inizio l'elemento casa[1] e salva l'elemento uscente in "riporto" */
            Squadra riporto = shiftRight(trasferta, casa [1],numero_squadre/2);

            /* Sposta a sinistra gli elementi di "casa" inserendo all'ultimo
            posto l'elemento "riporto" */
            shiftLeft(casa, riporto,numero_squadre/2);

            // Ripristina l'elemento fisso
            casa[0] = pivot ;
        }

        //Chiamo permutazioni semplici sulle giornate
        count+=perm_sempl(0,g,sol,mark,giornate,numero_squadre,0);
        //Scambio una partita in casa con una in trasferta
        shiftLeft(teams_temp,teams_temp[0],numero_squadre);
        //printf("\n");
    }
    printf("Calendari trovati = %d\n\n",count);
    printf("NOME SQUADRA\t\t\tPRIMO\t\tSECONDO\t\tTERZO\t\tVOLTE A PREMIO\n\n");
    for(int i=0;i<numero_squadre;i++){
        for(int j=0;j<30-strlen(teams[i].nome);j++)
            strcat(space," ");
        printf("%s%s %.2f%% (%d)\t%.2f%% (%d)\t%.2f%% (%d)\t%.2f%% (%d)\n\n",
               teams[i].nome,space,(float)n_wins[i]/count*100,n_wins[i],(float)n_silver[i]/count*100,n_silver[i],
               (float)n_bronze[i]/count*100,n_bronze[i],
               (float)(n_wins[i]+n_silver[i]+n_bronze[i])/count*100,(n_wins[i]+n_silver[i]+n_bronze[i]));
        strcpy(space,"");
    }
    return 0;
}

void shiftLeft(Squadra *data, Squadra add,int len){
    Squadra temp[len];
    for (int i = 0; i < len; i++) {
			temp[i] = data[i + 1];
    }
    temp[len-1] = add;
    for(int i=0;i<len;i++){
        data[i]=temp[i];
    }
    return;
}

Squadra shiftRight(Squadra *data, Squadra add,int len){
    Squadra temp[len];
    Squadra riporto = data[1];
    for (int i = 1; i < len; i++) {
        temp[i] = data[i - 1];
    }
    temp[0] = add;

    for(int i=0;i<len+1;i++)
        data[i]=temp[i];

    return riporto;
}

int perm_sempl(int pos,Giornata *val,Giornata *sol,int *mark,int n,int n_squadre, int count){
    int i;

    if(pos>=n){
        //Calcolare la classifica finale
        calcola_classifica(sol,n,n_squadre);
        return count+1;
    }

    for(i=0;i<n;i++){
        if(mark[i]==0){
            mark[i]=1;
            sol[pos]=val[i];
            count=perm_sempl(pos+1,val,sol,mark,n,n_squadre,count);
            mark[i]=0;
        }
    }
    return count;
}
Squadra *leggiFile(int n){
    Squadra *t;
    int count=0;
    FILE *fin;

    //Se il programma � lanciato da linea di comando
    //fin = fopen("../../Mollo_Cup.txt","r");

    //Se il programma � lanciato da IDE
    fin = fopen("Mollo_Cup_ritorno.txt","r");

    t=malloc(n*sizeof(Squadra));

    while(count<n){
        fscanf(fin,"%s",t[count].nome);
        t[count].id=count;
        t[count].sum=0;
        for(int i=0;i<NUM_GIORNATE;i++){
            fscanf(fin,"%f",&(t[count].punteggio[i]));
            t[count].sum+=t[count].punteggio[i];
        }
        count++;
    }
    fclose(fin);
    return t;
}

void calcola_classifica(Giornata *sol,int n,int n_teams){
    int *classifica;
    int first=-1,second=-1,third=-1,id1,id2,id3;

    classifica=calloc(n_teams,sizeof(int));
    for(int i=0;i<NUM_GIORNATE;i++){
        for(int j=0;j<n_teams/2;j++){
            if(goal(sol[i%(n_teams-1)].p[j].A.punteggio[i])>goal(sol[i%(n_teams-1)].p[j].B.punteggio[i]))
                classifica[sol[i%(n_teams-1)].p[j].A.id]+=3;
            else if(goal(sol[i%(n_teams-1)].p[j].A.punteggio[i])<goal(sol[i%(n_teams-1)].p[j].B.punteggio[i]))
                classifica[sol[i%(n_teams-1)].p[j].B.id]+=3;
            else{
                //Pareggio
                classifica[sol[i%(n_teams-1)].p[j].B.id]+=1;
                classifica[sol[i%(n_teams-1)].p[j].A.id]+=1;
            }
        }
    }

    for(int i=0;i<n_teams;i++){
        if((classifica[i]==first && teams[i].sum>teams[id1].sum)||(classifica[i]>first)){
            third=second;
            second=first;
            first=classifica[i];
            id3=id2;
            id2=id1;
            id1=i;
        }
        else if((classifica[i]==second && teams[i].sum>teams[id2].sum)||(classifica[i]>second)){
            third=second;
            second=classifica[i];
            id3=id2;
            id2=i;
        }
        else if((classifica[i]==third && teams[i].sum>teams[id3].sum)||(classifica[i]>third)){
            third=classifica[i];
            id3=i;
        }
    }
    n_wins[id1]++;
    n_silver[id2]++;
    n_bronze[id3]++;
    return;
}

int goal(float punteggio){
    float base=66.0;
    int gol=1;
    if(punteggio<base)
        return 0;

    while(base+4.0<punteggio){
        base+=4.0;
        gol++;
    }
    return gol;
}