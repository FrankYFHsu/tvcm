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

    Node n;
    double sim_time;
    double xtarget, ytarget;

    bool reach;
    double sum_of_ht = 0;
    double sum_of_ht_squared = 0;

    for(int i=0; i<25000; i++){

        //initialize the iteration
        sim_time = 0;
        n.Initialize(1);

        /*//play with the comm here,for testing
        double center_x[2];
        double center_y[2];
        for(int i=0; i<2; i++){
            for(int j=0;j<2; j++){

                center_x[i]= (double)rand()/RAND_MAX *XDIM;
                center_y[i]= (double)rand()/RAND_MAX *YDIM;

                n.comm_x[i][j] = center_x[i] - TIER_LENGTH;
                n.comm_y[i][j] = center_y[i] - TIER_LENGTH;
                n.comm_xupper[i][j] = center_x[i] + TIER_LENGTH;
                n.comm_yupper[i][j] = center_y[i] + TIER_LENGTH;
                if(n.comm_x[i][j]<0)
                    n.comm_x[i][j]=0;
                if(n.comm_y[i][j]<0)
                    n.comm_y[i][j]=0;
                if(n.comm_xupper[i][j]>XDIM)
                    n.comm_xupper[i][j] = XDIM;
                if(n.comm_yupper[i][j]>YDIM)
                    n.comm_yupper[i][j] = YDIM;

            }
        }*/

        xtarget = (double) rand()/RAND_MAX * XDIM;
        ytarget = (double) rand()/RAND_MAX * YDIM;


        reach = false;
        while(!reach){

            sim_time = n.ExecuteEvent(sim_time);
            //printf("state:%d loc: %lf %lf, time %lf\n", n.state, n.currentx, n.currenty, sim_time);

            if(distance2(n.currentx, n.currenty, xtarget, ytarget)<COMM_RANGE){
                reach=true;
                //printf("Hit at %lf, distance=%lf\n", sim_time, distance2(currentx, currenty, xtarget, ytarget));
                break;
            }
        }
        sum_of_ht+=sim_time;
        sum_of_ht_squared += sim_time*sim_time;
        printf("%lf  %lf\n", sum_of_ht/(i+1), sqrt(sum_of_ht_squared/(i+1) - sum_of_ht/(i+1)*sum_of_ht/(i+1)));
    }
}
