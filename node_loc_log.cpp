#include <sys/types.h> 
#include <sys/stat.h>
#include "Node.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define END_TIME 864000.0 //10 days
#define NODES 50


int main(void){

    Node n;
    double sim_time;
    FILE *out;
    char fileloc[50];

    printf("Input a random seed \n");
    int seed;
    scanf("%d", &seed);
    printf("Seed = %d\n",seed);

    //default is 1?  
    srand( (unsigned)seed );

    for(int i=0; i<NODES; i++){

        //initialize the iteration
        sim_time = 0;
        n.Initialize(i);
        
        char dirName[20];

        sprintf(dirName,"move_trace");
        mkdir(dirName,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        sprintf(fileloc,"move_trace/Node%d.txt", i);
        n.PrintTrace(fileloc);

        while(sim_time<END_TIME){

            sim_time = n.ExecuteEvent(sim_time);
        }

        n.CloseTraceFile();
    }
}
