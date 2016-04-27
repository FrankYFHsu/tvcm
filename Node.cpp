#include "Node.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define PAI 3.141592654
#define DELTATIME 0.000001

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

   return(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

Node::Node(){}
Node::~Node(){}

void Node::PrintTrace(char *filename){

   //turn on trace printing
   keeptrace = true;
   //open the file
   outfile = fopen(filename,"w");
}

void Node::CloseTraceFile(){

   fflush(outfile);
   fclose(outfile);
}

void Node::Initialize(int nodeid){

   nid = nodeid;

   //by default, no need to print the trace
   keeptrace = false;

   if(!USE_CONFIG_FILE){  ///config files not used, random community assignment
      if(TIERED_COMMUNITY){

         //create community
         for(int i=0; i<PERIOD; i++){
            center_x[i]= (double)rand()/RAND_MAX *XDIM;
            center_y[i]= (double)rand()/RAND_MAX *YDIM;
            center_x[i] = (int)center_x[i]/TIER_LENGTH/2*TIER_LENGTH*2 + TIER_LENGTH;
            center_y[i] = (int)center_y[i]/TIER_LENGTH/2*TIER_LENGTH*2 + TIER_LENGTH;

            for(int j=0; j<COMMTIER-1; j++){
               comm_x[i][j] = center_x[i] - (j*2+1)*TIER_LENGTH;
               comm_y[i][j] = center_y[i] - (j*2+1)*TIER_LENGTH;
               comm_xupper[i][j] = center_x[i] + (j*2+1)*TIER_LENGTH;
               comm_yupper[i][j] = center_y[i] + (j*2+1)*TIER_LENGTH;

               if(comm_x[i][j]<0)
                  comm_x[i][j]=0;
               if(comm_y[i][j]<0)
                  comm_y[i][j]=0;
               if(comm_xupper[i][j]>XDIM)
                  comm_xupper[i][j] = XDIM;
               if(comm_yupper[i][j]>YDIM)
                  comm_yupper[i][j] = YDIM;

            }

            //set the largest comm == sim area
            comm_x[i][COMMTIER-1]=0;
            comm_y[i][COMMTIER-1]=0;
            comm_xupper[i][COMMTIER-1]=XDIM;
            comm_yupper[i][COMMTIER-1]=YDIM;


         }

         /*for(int i=0; i<PERIOD; i++){
           printf("center_x %lf, center_y %lf\n",center_x[i],center_y[i]);
           }*/

         //setup periods
         time_period_counter=0;
         period = time_period_structure[0];
         next_period_start = time_period_dur[period];
         //printf("period %d, next_period_start %lf\n",period, next_period_start);

         //Initialize node state and location
         double random_number=(double) rand()/RAND_MAX;
         double level=0;
         for(int i=0; i<COMMTIER; i++){
            level+=state_prob[period][i];
            //printf("level %lf\n",level);
            //printf("rand %lf\n", random_number);
            if(random_number<level){
               state = i;
               break;
            }
         } 
         //printf("state %d\n", state);

         currentx = (double) rand()/RAND_MAX * (comm_xupper[period][state]-comm_x[period][state]) + comm_x[period][state];
         currenty = (double) rand()/RAND_MAX * (comm_yupper[period][state]-comm_y[period][state]) + comm_y[period][state];

         nextx=currentx;
         nexty=currenty;

         //setup the first event
         next_event_type = 1;
         next_event_time = 0;

      }else{ //if random community

         //create community
         for(int i=0; i<PERIOD; i++){
            for(int j=0; j<COMMTIER-1; j++){

               //NOTE: center has no meaning in this setting
               //It is just used to facilitate the calculation of community upper and lower boundary
               //Community side length = 2*TIER_LENGTH

               center_x[i]= (double)rand()/RAND_MAX *XDIM;
               center_y[i]= (double)rand()/RAND_MAX *YDIM;

               comm_x[i][j] = center_x[i] - TIER_LENGTH;
               comm_y[i][j] = center_y[i] - TIER_LENGTH;
               comm_xupper[i][j] = center_x[i] + TIER_LENGTH;
               comm_yupper[i][j] = center_y[i] + TIER_LENGTH;
               if(comm_x[i][j]<0)
                  comm_x[i][j]=0;
               if(comm_y[i][j]<0)
                  comm_y[i][j]=0;
               if(comm_xupper[i][j]>XDIM)
                  comm_xupper[i][j] = XDIM;
               if(comm_yupper[i][j]>YDIM)
                  comm_yupper[i][j] = YDIM;
            }    
            //set the largest comm == sim area
            comm_x[i][COMMTIER-1]=0;
            comm_y[i][COMMTIER-1]=0;
            comm_xupper[i][COMMTIER-1]=XDIM;
            comm_yupper[i][COMMTIER-1]=YDIM;
         }


         /*for(int i=0; i<PERIOD; i++){
           printf("center_x %lf, center_y %lf\n",center_x[i],center_y[i]);
           }*/

         //setup periods
         time_period_counter=0;
         period = time_period_structure[0];
         next_period_start = time_period_dur[period];
         //printf("period %d, next_period_start %lf\n",period, next_period_start);

         //Initialize node state and location
         double random_number=(double) rand()/RAND_MAX;
         double level=0;
         for(int i=0; i<COMMTIER; i++){
            level+=state_prob[period][i];
            //printf("level %lf\n",level);
            //printf("rand %lf\n", random_number);
            if(random_number<level){
               state = i;
               break;
            }
         }
         //printf("state %d\n", state);

         currentx = (double) rand()/RAND_MAX * (comm_xupper[period][state]-comm_x[period][state]) + comm_x[period][state];
         currenty = (double) rand()/RAND_MAX * (comm_yupper[period][state]-comm_y[period][state]) + comm_y[period][state];

         nextx=currentx;
         nexty=currenty;

         //setup the first event
         next_event_type = 1;
         next_event_time = 0;

      }
   }else{ //If read setting from file 

      FILE *parameters;
      char fileloc[40];
      sprintf(fileloc,"node_parameter_files/node%d", nodeid);
      parameters = fopen(fileloc,"r");

      for(int i=0; i<PERIOD; i++){
         for(int j=0; j<COMMTIER; j++){
            fscanf(parameters, "%lf", &state_prob[i][j]);
         }
      }

      fscanf(parameters, "%d", &number_of_item_in_structure);

      for(int i=0; i<number_of_item_in_structure; i++){
         fscanf(parameters, "%d", &time_period_structure[i]);
      }

      for(int i=0; i<PERIOD; i++){
         for(int j=0; j<COMMTIER; j++){
            fscanf(parameters, "%lf", &pause_max[i][j]);
         }
      }

      for(int i=0; i<PERIOD; i++){
         for(int j=0; j<COMMTIER; j++){
            fscanf(parameters, "%lf", &l_avg[i][j]);
         }
      }

      for(int i=0; i<PERIOD; i++){
         for(int j=0; j<COMMTIER; j++){
            fscanf(parameters, "%lf", &comm_x[i][j]);
            fscanf(parameters, "%lf", &comm_y[i][j]);
            fscanf(parameters, "%lf", &comm_xupper[i][j]);
            fscanf(parameters, "%lf", &comm_yupper[i][j]);
         }
      }

      for(int i=0; i<PERIOD; i++){
         for(int j=0; j<COMMTIER; j++){
            fscanf(parameters, "%lf", &vmin[i][j]);
         }
      }

      for(int i=0; i<PERIOD; i++){
         for(int j=0; j<COMMTIER; j++){
            fscanf(parameters, "%lf", &vmax[i][j]);
         }
      }

      for(int i=0; i<PERIOD; i++){
         for(int j=0; j<COMMTIER; j++){
            fscanf(parameters, "%lf", &vavg[i][j]);
         }
      }


      fclose(parameters);

      //setup periods
      time_period_counter=0;
      period = time_period_structure[0];
      next_period_start = time_period_dur[period];
      //printf("period %d, next_period_start %lf\n",period, next_period_start);

      //Initialize node state and location
      double random_number=(double) rand()/RAND_MAX;
      double level=0;
      for(int i=0; i<COMMTIER; i++){
         level+=state_prob[period][i];
         //printf("level %lf\n",level);
         //printf("rand %lf\n", random_number);
         if(random_number<level){
            state = i;
            break;
         }
      }
      //printf("state %d\n", state);

      currentx = (double) rand()/RAND_MAX * (comm_xupper[period][state]-comm_x[period][state]) + comm_x[period][state];
      currenty = (double) rand()/RAND_MAX * (comm_yupper[period][state]-comm_y[period][state]) + comm_y[period][state];

      nextx=currentx;
      nexty=currenty;

      //setup the first event
      next_event_type = 1;
      next_event_time = 0;

   }


   /*//varify the parameters are read properly
     printf("Read parameters from Node %d:\n", nodeid);
     printf("state_prob:\n");
     for(int i=0; i<PERIOD; i++){
     for(int j=0; j<COMMTIER; j++){
     printf("%d %d %lf\n", i, j, state_prob[i][j]);
     }
     }

     printf("number_of_item_in_structure: %d\n",number_of_item_in_structure);

     for(int i=0; i<number_of_item_in_structure; i++){
     printf("%d  ", time_period_structure[i]);
     }

     printf("\n pause_max \n");
     for(int i=0; i<PERIOD; i++){
     for(int j=0; j<COMMTIER; j++){
     printf("%d %d %lf\n", i, j, pause_max[i][j]);
     }
     }

     printf("\n l_avg \n");
     for(int i=0; i<PERIOD; i++){
     for(int j=0; j<COMMTIER; j++){
     printf("%d %d %lf\n", i, j, l_avg[i][j]);
     }
     }

     for(int i=0; i<PERIOD; i++){
     for(int j=0; j<COMMTIER; j++){
   fprintf(outfile,"# period %d, community %d:\n", i, j);
   fprintf(outfile,"# (%lf, %lf) - (%lf, %lf)\n", comm_x[i][j], comm_y[i][j], comm_xupper[i][j], comm_yupper[i][j]);
}
}*/

}

double Node::ExecuteEvent(double sim_time){


   if(next_event_type==1){

      //print the initial location if NS-trace format
      if(keeptrace){
         if(TRACE_FORMAT){
            if(sim_time == 0){
               for(int i=0; i<PERIOD; i++){
                  for(int j=0; j<COMMTIER; j++){
                     fprintf(outfile,"#period %d, community %d:\n", i, j);
                     fprintf(outfile,"# (%lf, %lf) - (%lf, %lf)\n", comm_x[i][j], comm_y[i][j], comm_xupper[i][j], comm_yupper[i][j]);
                  }
               }
               fprintf(outfile,"$node_%d set X_ %lf\n", nid, currentx);
               fprintf(outfile,"$node_%d set Y_ %lf\n", nid, currenty);
               fprintf(outfile,"$node_%d set Z_ 0.00\n", nid);
            }
         }
      }

      sim_time = next_event_time;

      //if warping happens, print NS-2 log
      if(currentx!=nextx){
         if(keeptrace){
            if(TRACE_FORMAT){
               fprintf(outfile, "$ns at %lf \"$node_%d setdest %lf %lf %lf\"\n", sim_time-DELTATIME, nid, nextx, nexty, distance(currentx, currenty, nextx, nexty)/DELTATIME);
               fprintf(outfile, "#warp at the beginning of a new time period\n");
            }
         }
      }


      //move the node AFTER the pause
      currentx = nextx;
      currenty = nexty;

      travel_dir = (double) rand()/RAND_MAX * 2 *PAI - PAI;
      travel_speed = (double) rand()/RAND_MAX * (vmax[period][state]-vmin[period][state]) + vmin[period][state];
      dur_mean = (double)l_avg[period][state]/(double)vavg[period][state];

      travel_duration = - dur_mean * log((double) rand()/RAND_MAX);
      travel_stop_time = sim_time + travel_duration;
      if(travel_stop_time > sim_time + TIME_STEP){
         next_event_type = 2;
         next_event_time = sim_time + TIME_STEP;
      }else{
         next_event_type = 2;
         next_event_time = travel_stop_time;
      }

      //print the NS-compatible trace, hop back to the next point
      if(keeptrace){
         if(TRACE_FORMAT){
            anchor_x = currentx;
            anchor_y = currenty;
            anchor_t = sim_time;
         }
      }

   }else{

      double time_diff = next_event_time - sim_time;
      sim_time = next_event_time;

      //printf("update node location with time diff %lf\n", time_diff);
      currentx = currentx + travel_speed*time_diff*cos(travel_dir);
      currenty = currenty + travel_speed*time_diff*sin(travel_dir);

      bool coordinate_change = false; 
      //adjust the x,y coordinate
      if(!TORUS_BOUNDARY){
         //reflective boundary 
         if(currentx>comm_xupper[period][state]){
            currentx = comm_xupper[period][state] - (currentx-comm_xupper[period][state]);
            travel_dir = reflectx(travel_dir);
            coordinate_change = true;
         }
         if(currentx<comm_x[period][state]){
            currentx = comm_x[period][state]+(comm_x[period][state]-currentx);
            travel_dir = reflectx(travel_dir);
            coordinate_change = true;
         }
         if(currenty>comm_yupper[period][state]){
            currenty = comm_yupper[period][state] - (currenty-comm_yupper[period][state]);
            travel_dir = reflecty(travel_dir);
            coordinate_change = true;
         }
         if(currenty<comm_y[period][state]){
            currenty = comm_y[period][state]+(comm_y[period][state]-currenty);
            travel_dir = reflecty(travel_dir);
            coordinate_change = true;
         }
      }else{
         //torus
         if(currentx>comm_xupper[period][state]){
            tempx = currentx;
            tempy = currenty;
            currentx = comm_x[period][state] + (currentx - comm_xupper[period][state]);
            coordinate_change = true;
         }
         if(currentx<comm_x[period][state]){
            tempx = currentx;
            tempy = currenty;
            currentx = comm_xupper[period][state] - (comm_x[period][state]-currentx);
            coordinate_change = true;
         }
         if(currenty>comm_yupper[period][state]){
            tempx = currentx;
            tempy = currenty;
            currenty = comm_y[period][state] + (currenty - comm_yupper[period][state]);
            coordinate_change = true;
         }
         if(currenty<comm_y[period][state]){
            tempx = currentx;
            tempy = currenty;
            currenty = comm_yupper[period][state] - (comm_y[period][state]-currenty);
            coordinate_change = true;
         }
      }

      //if direction has been changed, need to print out NS-2 compatible trace
      if(keeptrace){
         if(TRACE_FORMAT){
            if(coordinate_change){
               //for torus boundary, need to adjust the coordinate to avoid node moving out of community
               if(tempx>comm_xupper[period][state]){
                  tempx = comm_xupper[period][state]-0.000001;
               }
               if(tempx<comm_x[period][state]){
                  tempx = comm_x[period][state]+0.000001;
               }
               if(tempy>comm_yupper[period][state]){
                  tempy = comm_yupper[period][state]-0.000001;
               }
               if(tempy<comm_y[period][state]){
                  tempy = comm_y[period][state]+0.000001;
               }
               if(!TORUS_BOUNDARY){ //reflective boundary, use the current position as the latest anchor
                  fprintf(outfile, "$ns at %lf \"$node_%d setdest %lf %lf %lf\"\n", anchor_t, nid, currentx, currenty, distance(currentx, currenty, anchor_x, anchor_y)/(sim_time - anchor_t));
               }else{  //torus boundary, move towards the coordinate before the coordinate change, and then hop
                  fprintf(outfile, "$ns at %lf \"$node_%d setdest %lf %lf %lf\"\n", anchor_t, nid, tempx, tempy, distance(tempx, tempy, anchor_x, anchor_y)/(sim_time - anchor_t - DELTATIME));
                  fprintf(outfile, "$ns at %lf \"$node_%d setdest %lf %lf %lf\"\n",sim_time-DELTATIME, nid, currentx, currenty, distance(currentx, currenty, tempx, tempy)/DELTATIME);
               }
               fprintf(outfile, "#boundary reached\n");
               anchor_x = currentx;
               anchor_y = currenty;
               anchor_t = sim_time;
            }
         }
      }

      if(travel_stop_time > sim_time + TIME_STEP){
         next_event_type = 2;
         next_event_time = sim_time + TIME_STEP;
      }else if(travel_stop_time > sim_time){
         next_event_type = 2;
         next_event_time = travel_stop_time;
      }else{

         //print the  NS-2 compatible trace
         if(keeptrace){
            if(TRACE_FORMAT){
               //need to ensure sim_time!=anchor time
               if(sim_time!=anchor_t){
                  fprintf(outfile, "$ns at %lf \"$node_%d setdest %lf %lf %lf\"\n", anchor_t, nid, currentx, currenty, distance(currentx, currenty, anchor_x, anchor_y)/(sim_time - anchor_t));
               }
               fprintf(outfile, "#end of an epoch\n");
            }
         }

         next_event_type = 1;
         //pick pause time
         next_event_time = travel_stop_time + (double) rand()/RAND_MAX * pause_max[period][state];

         //decide the next state
         double random_number=(double) rand()/RAND_MAX;
         int next_state;
         double level=0;
         for(int i=0; i<COMMTIER; i++){
            level+=state_prob[period][i];
            if(random_number<level){
               next_state = i;
               break;
            }
         }

         //printf("next state %d, current state %d, comm (%lf %lf) - (%lf %lf)\n", next_state, state, comm_x[period][next_state], comm_y[period][next_state], comm_xupper[period][next_state], comm_yupper[period][next_state]);
         if(currentx<comm_x[period][next_state]||currentx>comm_xupper[period][next_state]||currenty<comm_y[period][next_state]||currenty>comm_yupper[period][next_state]){ //currently out of the next community, move to inner community, warp
            nextx = (double) rand()/RAND_MAX * (comm_xupper[period][next_state]-comm_x[period][next_state]) +comm_x[period][next_state];
            nexty = (double) rand()/RAND_MAX * (comm_yupper[period][next_state]-comm_y[period][next_state]) +comm_y[period][next_state]; 
         }else{
            nextx = currentx;
            nexty = currenty;
         }
         state=next_state; 
      }

      //if next event time larger than time period boundary, cancel the event and restart from a new movement
      if(next_event_time>next_period_start){
         next_event_time = next_period_start;
         double last_event_in_period = next_event_type;
         next_event_type = 1;
         time_period_counter++;
         if(time_period_counter==number_of_item_in_structure){
            time_period_counter=0;
         }
         //printf("counter %d\n",time_period_counter);
         period = time_period_structure[time_period_counter];

         //print the last NS-compatible trace for this epoch
         if(keeptrace){
            if(TRACE_FORMAT){
               if(last_event_in_period==2){  //if the last event is a moving event, finish the epoch
                  fprintf(outfile, "$ns at %lf \"$node_%d setdest %lf %lf %lf\"\n", anchor_t, nid, currentx, currenty, distance(currentx, currenty, anchor_x, anchor_y)/(sim_time - anchor_t));
                  fprintf(outfile, "#end of epoch in the current time period\n");
               }
            }
         }

         //Initialize node state and location
         double random_number=(double) rand()/RAND_MAX;
         double level=0;
         for(int i=0; i<COMMTIER; i++){
            level+=state_prob[period][i];
            if(random_number<level){
               state = i;
               break;
            }
         }

         //book keeping for NS-2 trace
         anchor_x = currentx;
         anchor_y = currenty;


         currentx = (double) rand()/RAND_MAX * (comm_xupper[period][state]-comm_x[period][state]) + comm_x[period][state];
         currenty = (double) rand()/RAND_MAX * (comm_yupper[period][state]-comm_y[period][state]) + comm_y[period][state];

         nextx=currentx;
         nexty=currenty;

         //print the warp
         if(keeptrace){
            if(TRACE_FORMAT){
               fprintf(outfile, "$ns at %lf \"$node_%d setdest %lf %lf %lf\"\n", next_event_time-DELTATIME, nid, nextx, nexty, distance(anchor_x, anchor_y, nextx, nexty)/DELTATIME);
               fprintf(outfile, "#warp at the beginning of a new time period\n");
            }
         }

         next_period_start += time_period_dur[period];
         //printf("next period start %lf, period %d\n", next_period_start,period);
      }
   }
   //end of execute event, print the trace if needed in (t x y) format
   if(keeptrace){
      if(!TRACE_FORMAT){
         fprintf(outfile,"%lf  %lf  %lf\n", sim_time, currentx, currenty);
      }
   }

   return(sim_time);
}
