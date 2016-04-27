#include "Node.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define COMM_RANGE 30
#define ITERATION 1500
#define ENDTIME 1000000.0

double distance(double x1, double y1, double x2, double y2){

    return(sqrt((pow((x1-x2),2)+pow((y1-y2),2))));

}


int main(void){

    srand( (unsigned)time( NULL ) );
    FILE *outfile;

    Node n1;
    Node n2;
    double timer1, timer2;
    char filename[30];
    double currenttime;
    //double xtarget, ytarget;

    bool in_touch;

    for(int ite=0; ite<ITERATION; ite++){

        //initialize the iteration
        //sim_time = 0;
        timer1=0;
        timer2=0;
        sprintf(filename,"meeting_epoch/trace%d.txt",ite+1);
        outfile = fopen(filename,"w");

        //play with the node community center here:
        /*n1.center_x[0] = 150.0;
        n1.center_y[0] = 250.0;
        n1.center_x[1] = 850.0;
        n1.center_y[1] = 450.0;
        n2.center_x[0] = 850.0;
        n2.center_y[0] = 250.0;
        n2.center_x[1] = 250.0;
        n2.center_y[1] = 850.0;*/

        n1.Initialize(1);
        n2.Initialize(2);

        if(distance(n1.currentx, n1.currenty, n2.currentx, n2.currenty)<COMM_RANGE){
            in_touch = true;
            fprintf(outfile, "0  ");
        }else{
            in_touch = false;
        }

        while(timer1<ENDTIME||timer2<ENDTIME){

            if(n1.next_event_time<n2.next_event_time){
                currenttime = timer1;
                timer1 = n1.ExecuteEvent(timer1);
                //printf("N1 , %lf\n", sim_time);
            }else{
                currenttime = timer2;
                timer2 = n2.ExecuteEvent(timer2);
                //printf("N2 , %lf\n", sim_time);
            }

            //fprintf(outfile,"%lf: (%lf, %lf)  (%lf, %lf)   %lf\n",currenttime, n1.currentx, n1.currenty, n2.currentx, n2.currenty,distance(n1.currentx, n1.currenty, n2.currentx, n2.currenty));

            if(in_touch){  //Currently in comm range
                if(distance(n1.currentx, n1.currenty, n2.currentx, n2.currenty)>COMM_RANGE){  //break of meeting
                    fprintf(outfile,"%lf\n", currenttime);
                    in_touch = false;
                }
            }else{  //Currently out of comm range
                if(distance(n1.currentx, n1.currenty, n2.currentx, n2.currenty)<COMM_RANGE){  //starting of meeting
                    fprintf(outfile,"%lf  ", currenttime);
                    in_touch = true;
                }
            }
        }
        
        if(in_touch){
            fprintf(outfile,"%lf\n",ENDTIME);
        }

        fflush(outfile);
        fclose(outfile);
    }
}
