#ifndef NODE_H
#define NODE_H
#include <stdio.h>
#include <math.h>

#define USE_CONFIG_FILE 0 //if config files are used
#define TIERED_COMMUNITY 1 //if the communities are tiered or randomly assigned
#define TORUS_BOUNDARY 0 //if the community boundaries are torus or reflective
#define TRACE_FORMAT 0 //0: (t x y) format, 1: NS-2 format
#define PERIOD 3  //number of unique time periods
#define COMMTIER 6 //number of community tiers (or communities)
#define STRUCTURE 100 //longest structure you can have
#define XDIM 1500.0  //size of simulation area
#define YDIM 1500.0  //size of simulation area
#define TIER_LENGTH 25.0 // 1/2 of edge length of communities
#define TIME_STEP 1 //time interval to update node locations

    static const double state_prob[PERIOD][COMMTIER] = {{0.9,0.05,0.025,0.015,0.005,0.005},{0.6,0.2,0.10,0.08,0.015,0.005},{0.5,0.3,0.10,0.08,0.015,0.005}};
    static const double time_period_dur[PERIOD]={43200,28800,14400};
    static const int time_period_structure[STRUCTURE]={0,1,2};
    static const int number_of_item_in_structure = 3;
    static const double pause_max[PERIOD][COMMTIER]={{21600,2400,1200,900,600,60},{3600,2400,2400,2000,2000,1000},{3600,2400,1200,900,600,60}};
    static const double l_avg[PERIOD][COMMTIER] = {{300,500,500,800,1000,1000},{400,600,700,900,1000,1000},{300,500,500,800,1000,1000}};

    static double vmin[PERIOD][COMMTIER] = {{1,1,1,1,1,1},{1,1,1,1,1,1},{1,1,1,1,1,1}};
    static double vavg[PERIOD][COMMTIER] = {{1.25,1.25,1.25,1.25,1.25,1.25},{1.25,1.25,1.25,1.25,1.25,1.25},{1.25,1.25,1.25,1.25,1.25,1.25}};
    static double vmax[PERIOD][COMMTIER] = {{1.5,1.5,1.5,1.5,1.5,1.5},{1.5,1.5,1.5,1.5,1.5,1.5},{1.5,1.5,1.5,1.5,1.5,1.5}};


class Node{

public:

    Node();
    ~Node();
    void Initialize(int nodeid);
    double ExecuteEvent(double sim_time);
    void PrintTrace(char *filename);
    void CloseTraceFile();

    double currentx, currenty;
    double next_event_time;
    FILE *outfile;

    bool keeptrace;
    double nextx, nexty;  //location to move after the pause
    int next_event_type;
    int state;  //0,1,2,...,n-1 (n-1=roaming)
    int period;  //0,1,2,...,t-1
    double travel_speed;
    double travel_dir;
    double travel_duration;
    double travel_stop_time;
    double dur_mean;
    //double comm_x, comm_y, comm_xupper, comm_yupper;
    double next_period_start;
    double center_x[PERIOD], center_y[PERIOD];
    //variables used for NS-2 compatible trace generation
    double anchor_t, anchor_x, anchor_y;
    double tempx, tempy;
    int nid;

    double comm_x[PERIOD][COMMTIER];
    double comm_y[PERIOD][COMMTIER];
    double comm_xupper[PERIOD][COMMTIER];
    double comm_yupper[PERIOD][COMMTIER];


    int time_period_counter;

};
#endif
