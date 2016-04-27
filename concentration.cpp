#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define XDIM 1000
#define YDIM 1000
#define PAI 3.141592654
#define COMM_RANGE 5
#define TIME_STEP 0.1
#define VMIN 5
#define VMAX 35
#define VAVG 20
#define LAVG 200
#define LAVG_LOCAL 50
#define LAVG_LOCAL_CONC 100
#define PAUSEMAX 20
#define PAUSEMAX_LOCAL 20
#define PAUSEAVG 10
#define LOCAL_TO_ROAM 0.2
#define ROAM_TO_LOCAL 0.5
#define COMM_SIZE 200
#define COMM_SIZE_CONC 250
#define CONC_START 5000
#define CONC_END 8000
#define LOCAL_TO_ROAM2 0.05
#define ROAM_TO_LOCAL2 0.9

//The sin and cos functions are in radius for input

double reflectx(double angle){
    if(angle>=0){
        return(PAI-angle);
    }else{
        return(-PAI-angle);
    }
}

double reflecty(double angle){
    return(-angle);
}

double distance(double x1, double y1, double x2, double y2){

    return(sqrt((pow((x1-x2),2)+pow((y1-y2),2))));

}

int main(void){

    srand( (unsigned)time( NULL ) );

    double currentx, currenty;
    double xtarget, ytarget;
    double sim_time = 0;
    int next_event_type = 1; //1:set_new_trip 2:movement
    int state;  //0:local, 1:roaming
    int period;  //0:normal, 1:concentration
    double next_event_time;
    double sim_advance;
    double travel_speed;
    double travel_dir;
    double travel_duration;
    double dur_mean;
    bool reach;
    double travel_stop_time;
    double time_diff;
    double sum_of_ht=0;
    double stationary_local = ROAM_TO_LOCAL/(ROAM_TO_LOCAL+LOCAL_TO_ROAM);
    double stationary_local_conc = ROAM_TO_LOCAL2/(ROAM_TO_LOCAL2+LOCAL_TO_ROAM2);
    double comm_x, comm_y, comm_xupper, comm_yupper;
    double comm_x2, comm_y2, comm_xupper2, comm_yupper2;
    double next_conc_start, next_normal_start;

 for(int i=0; i<100000; i++){

    //create community
    comm_x = (double) rand()/RAND_MAX * (XDIM-COMM_SIZE);
    comm_y = (double) rand()/RAND_MAX * (YDIM-COMM_SIZE);
    comm_xupper = comm_x+COMM_SIZE;
    comm_yupper = comm_y+COMM_SIZE;
    comm_x2 = (double) rand()/RAND_MAX * (XDIM-COMM_SIZE_CONC);
    comm_y2 = (double) rand()/RAND_MAX * (YDIM-COMM_SIZE_CONC);
    comm_xupper2 = comm_x2+COMM_SIZE_CONC;
    comm_yupper2 = comm_y2+COMM_SIZE_CONC;
 
    //Initialize the target and moving node
    if((double) rand()/RAND_MAX<stationary_local){
        state = 0;
        currentx = (double) rand()/RAND_MAX * COMM_SIZE + comm_x;
        currenty = (double) rand()/RAND_MAX * COMM_SIZE + comm_y;
    }else{
        state = 1;
        currentx = (double) rand()/RAND_MAX * XDIM;
        currenty = (double) rand()/RAND_MAX * YDIM;
    }
    xtarget = (double) rand()/RAND_MAX * XDIM;
    ytarget = (double) rand()/RAND_MAX * YDIM;
    while((xtarget>comm_x&&xtarget<comm_xupper&&ytarget>comm_y&&ytarget<comm_yupper)||(xtarget>comm_x2&&xtarget<comm_xupper2&&ytarget>comm_y2&&ytarget<comm_yupper2)){ //out of comm for both
    //while(!((xtarget>comm_x&&xtarget<comm_xupper&&ytarget>comm_y&&ytarget<comm_yupper)&&(xtarget<comm_x2||xtarget>comm_xupper2||ytarget<comm_y2||ytarget>comm_yupper2))){ //in comm for normal period, out comm for conc period
    //while(!((xtarget<comm_x||xtarget>comm_xupper||ytarget<comm_y||ytarget>comm_yupper)&&(xtarget>comm_x2&&xtarget<comm_xupper2&&ytarget>comm_y2&&ytarget<comm_yupper2))){ //out of comm for normal period, in comm for conc period
        comm_x = (double) rand()/RAND_MAX * (XDIM-COMM_SIZE);
        comm_y = (double) rand()/RAND_MAX * (YDIM-COMM_SIZE);
        comm_xupper = comm_x+COMM_SIZE;
        comm_yupper = comm_y+COMM_SIZE;
        comm_x2 = (double) rand()/RAND_MAX * (XDIM-COMM_SIZE_CONC);
        comm_y2 = (double) rand()/RAND_MAX * (YDIM-COMM_SIZE_CONC);
        comm_xupper2 = comm_x2+COMM_SIZE_CONC;
        comm_yupper2 = comm_y2+COMM_SIZE_CONC;

        xtarget = (double) rand()/RAND_MAX * XDIM;
        ytarget = (double) rand()/RAND_MAX * YDIM;
    }
    //printf("a new trial starts!!\n");
    reach = false;
    sim_time=0;
    //printf("start from (%lf, %lf), target (%lf, %lf)\n", currentx, currenty, xtarget, ytarget);
    //printf("community setup: in normal period (%lf,%lf), in concentration period (%lf, %lf)-(%lf, %lf)\n", comm_x, comm_y, comm_x2, comm_y2, comm_xupper2, comm_yupper2);
    next_event_type = 1;
    next_event_time = 0;

    period = 0;
    next_conc_start = CONC_START;
    next_normal_start = CONC_END;

    while(!reach){
        if(next_event_type==1){
            sim_time = next_event_time;
            travel_dir = (double) rand()/RAND_MAX * 2 *PAI - PAI;
            travel_speed = (double) rand()/RAND_MAX * (VMAX-VMIN) + VMIN;
            if(period==0){  //normal period
                if(state==0){ //local_epoch
                    dur_mean = (double)LAVG_LOCAL/(double)VAVG;
                }else{ //roaming epoch
                    dur_mean = (double)LAVG/(double)VAVG;
                }
            }else{   //concentration period
                if(state==0){ //local_epoch
                    dur_mean = (double)LAVG_LOCAL_CONC/(double)VAVG;
                }else{ //roaming epoch
                    dur_mean = (double)LAVG/(double)VAVG;
                }
            }
            travel_duration = - dur_mean * log((double) rand()/RAND_MAX);
            travel_stop_time = sim_time + travel_duration;
            if(travel_stop_time > sim_time + TIME_STEP){
                next_event_type =2;
                next_event_time = sim_time + TIME_STEP;
            }else{
                next_event_type = 2;
                next_event_time = travel_stop_time;
            }
            //printf("time=%lf, period=%d, state=%d, set speed=%lf, angle=%lf, trave_time=%lf, travel ends at %lf\n", sim_time, period, state, travel_speed, travel_dir, travel_duration, travel_stop_time);
        }else{
            time_diff = next_event_time - sim_time;
            
            sim_time = next_event_time;
          
            currentx = currentx + travel_speed*time_diff*cos(travel_dir);
            currenty = currenty + travel_speed*time_diff*sin(travel_dir);

            //adjust the x,y coordinate
            if(state==1){
                //roaming state, reflect when boundary of simu area reached
                /*if(currentx>XDIM){
                    currentx = XDIM - (currentx-XDIM);
                    travel_dir = reflectx(travel_dir);
                }
                if(currentx<0){
                    currentx = -currentx;
                    travel_dir = reflectx(travel_dir);
                }
                if(currenty>YDIM){
                    currenty = YDIM - (currenty-YDIM);
                    travel_dir = reflecty(travel_dir);
                }
                if(currenty<0){
                    currenty = -currenty;
                    travel_dir = reflecty(travel_dir);
                }*/
                //torus
                if(currentx>XDIM){
                    currentx = currentx-XDIM;
                }
                if(currentx<0){
                    currentx = XDIM+currentx;
                }
                if(currenty>YDIM){
                    currenty = currenty-YDIM;
                }
                if(currenty<0){
                    currenty = YDIM+currenty;
                }

            }else if(state==0){  //local state, reflect when community boundary reached

                if(period==0){ //normal period
                   if(currentx>comm_xupper){
                       currentx = comm_xupper - (currentx-comm_xupper);
                       travel_dir = reflectx(travel_dir);
                   }
                   if(currentx<comm_x){
                       currentx = comm_x+(comm_x-currentx);
                       travel_dir = reflectx(travel_dir);
                   }
                   if(currenty>comm_yupper){
                       currenty = comm_yupper - (currenty-comm_yupper);
                       travel_dir = reflecty(travel_dir);
                   }
                   if(currenty<comm_y){
                       currenty = comm_y+(comm_y-currenty);
                       travel_dir = reflecty(travel_dir);
                   }
                }else{ //conc period
                   if(currentx>comm_xupper2){
                       currentx = comm_xupper2 - (currentx-comm_xupper2);
                       travel_dir = reflectx(travel_dir);
                   }
                   if(currentx<comm_x2){
                       currentx = comm_x2+(comm_x2-currentx);
                       travel_dir = reflectx(travel_dir);
                   }
                   if(currenty>comm_yupper2){
                       currenty = comm_yupper2 - (currenty-comm_yupper2);
                       travel_dir = reflecty(travel_dir);
                   }
                   if(currenty<comm_y2){
                       currenty = comm_y2+(comm_y2-currenty);
                       travel_dir = reflecty(travel_dir);
                   }
                }
            }

            if(distance(currentx, currenty, xtarget, ytarget)<COMM_RANGE){
                reach=true;
                //printf("Hit at %lf, distance=%lf\n", sim_time, distance(currentx, currenty, xtarget, ytarget));
                break;
            }
    
            if(travel_stop_time > sim_time + TIME_STEP){
                next_event_type = 2;
                next_event_time = sim_time + TIME_STEP;
            }else if(travel_stop_time > sim_time){
                next_event_type = 2;
                next_event_time = travel_stop_time;
            }else{
                next_event_type = 1;
                if(period==0){ //in normal period
                    if(state==1){ //currently in roaming state
                        next_event_time = travel_stop_time + (double) rand()/RAND_MAX *PAUSEMAX;
                        if((double) rand()/RAND_MAX < ROAM_TO_LOCAL){
                            state=0;
                            //Instantly move into the community
                            currentx = (double) rand()/RAND_MAX * COMM_SIZE + comm_x;
                            currenty = (double) rand()/RAND_MAX * COMM_SIZE + comm_y;
                        }else{
                            state=1;
                        }
                    }else{ //currently in local state
                        next_event_time = travel_stop_time + (double) rand()/RAND_MAX *PAUSEMAX_LOCAL;
                        if((double) rand()/RAND_MAX < LOCAL_TO_ROAM){
                            state=1;
                            //Start from a random location
                            currentx = (double) rand()/RAND_MAX * XDIM;
                            currenty = (double) rand()/RAND_MAX * YDIM;
                        }else{
                            state=0;
                        }
                    }
                }else{  //in concentration period
                    if(state==1){ //currently in roaming state
                        next_event_time = travel_stop_time + (double) rand()/RAND_MAX *PAUSEMAX;
                        if((double) rand()/RAND_MAX < ROAM_TO_LOCAL2){
                            state=0;
                            //Instantly move into the community
                            currentx = (double) rand()/RAND_MAX * COMM_SIZE_CONC + comm_x2;
                            currenty = (double) rand()/RAND_MAX * COMM_SIZE_CONC + comm_y2;
                        }else{
                            state=1;
                        }
                    }else{ //currently in local state
                        next_event_time = travel_stop_time + (double) rand()/RAND_MAX *PAUSEMAX_LOCAL;
                        if((double) rand()/RAND_MAX < LOCAL_TO_ROAM2){
                            state=1;
                            //Start from a random location
                            currentx = (double) rand()/RAND_MAX * XDIM;
                            currenty = (double) rand()/RAND_MAX * YDIM;
                        }else{
                            state=0;
                        }
                    }
                }
            }

            //if next event time larger than time period boundary, cancel the event and restart from a new movement
            if(next_event_time>next_conc_start){
                next_event_time = next_conc_start;
                next_event_type = 1;
                period = 1;
                if((double) rand()/RAND_MAX<stationary_local_conc){
                    state = 0;
                    currentx = (double) rand()/RAND_MAX * COMM_SIZE + comm_x2;
                    currenty = (double) rand()/RAND_MAX * COMM_SIZE + comm_y2;
                }else{
                    state = 1;
                }
                next_conc_start = next_conc_start + CONC_END;
            }

            if(next_event_time>next_normal_start){
                next_event_time = next_normal_start;
                next_event_type = 1;
                period = 0;
                if((double) rand()/RAND_MAX<stationary_local){
                    state = 0;
                    currentx = (double) rand()/RAND_MAX * COMM_SIZE + comm_x;
                    currenty = (double) rand()/RAND_MAX * COMM_SIZE + comm_y;
                }else{
                    state = 1;
                }
                next_normal_start = next_normal_start + CONC_END;
            }


                
            //printf("time=%lf, at (%lf, %lf), distance=%lf\n", sim_time, currentx, currenty, distance(currentx, currenty, xtarget, ytarget));
        } 
    }
    //printf("sin 1=%lf, cos 1=%lf\n",sin(1), cos(1));
    //printf("%lf\n", sim_time);
    sum_of_ht+=sim_time;
    printf("%lf\n", sum_of_ht/(i+1));
 }
 //printf("%lf\n", sum_of_ht/100000);

}
