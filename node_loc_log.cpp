#include "Node.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define END_TIME 20000.0
#define NODES 2


int main(void){

    Node n;
    double sim_time;
    FILE *out;
    char fileloc[50];

    //srand( (unsigned)time( NULL ) );

    for(int i=0; i<NODES; i++){

        //initialize the iteration
        sim_time = 0;
        n.Initialize(i);
        
        sprintf(fileloc,"move_trace/Node%d", i+1);
        n.PrintTrace(fileloc);

        while(sim_time<END_TIME){

            sim_time = n.ExecuteEvent(sim_time);
        }

        n.CloseTraceFile();
    }
}
