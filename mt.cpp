#include "Node.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define COMM_RANGE 60




double distance2(double x1, double y1, double x2, double y2){

    return(sqrt((pow((x1-x2),2)+pow((y1-y2),2))));

}


int main(void){

    srand( (unsigned)time( NULL ) );

    Node n1;
    Node n2;
    double timer1, timer2;
    //double xtarget, ytarget;

    bool reach;
    double sum_of_mt = 0;

    for(int i=0; i<25000; i++){

        //initialize the iteration
        //sim_time = 0;
        timer1=0;
        timer2=0;

        n1.Initialize(1);
        n2.Initialize(2);

        //play with the comm here
        for(int i=0; i<2; i++){
            for(int j=0;j<2; j++){
                n1.comm_x[i][j] = 120.0;
                n1.comm_y[i][j] = 120.0;
                n1.comm_xupper[i][j] = 280.0;
                n1.comm_yupper[i][j] = 280.0;
                n2.comm_x[i][j] = 720.0;
                n2.comm_y[i][j] = 720.0;
                n2.comm_xupper[i][j] = 880.0;
                n2.comm_yupper[i][j] = 880.0;

            }
        }

        //printf("comm node1 (%lf %lf)-(%lf %lf) node2 (%lf %lf) - (%lf %lf)\n", n1.comm_x[0][0], n1.comm_y[0][0], n1.comm_xupper[0][0], n1.comm_yupper[0][0], n2.comm_x[0][0], n2.comm_y[0][0], n2.comm_xupper[0][0], n2.comm_yupper[0][0]);
        
        reach = false;
        while(!reach){

            if(n1.next_event_time<n2.next_event_time){
                timer1 = n1.ExecuteEvent(timer1);
                //printf("N1 , %lf\n", sim_time);
            }else{
                timer2 = n2.ExecuteEvent(timer2);
                //printf("N2 , %lf\n", sim_time);
            }

            //printf("time:%lf %lf node1:(%lf %lf) node2:(%lf %lf)\n", timer1, timer2,n1.currentx, n1.currenty, n2.currentx, n2.currenty);
            if(distance2(n1.currentx, n1.currenty, n2.currentx, n2.currenty)<COMM_RANGE){
                reach=true;
                break;
            }
        }
        
        if(timer1<timer2){
            sum_of_mt+=timer1;
        }else{
            sum_of_mt+=timer2;
        }

        printf("%lf\n", sum_of_mt/(i+1));
    }
}
