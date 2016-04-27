#include <stdio.h>

#define SLOTS 10000
#define STEP 10
#define STEPM 0.1
#define ITERATION 1500

int main(void){

    long int interm[SLOTS]={0};
    long int meet[SLOTS]={0};
    char filename[30];
    FILE *in;
    FILE *out;
    bool first;
    double start, end, meeting_time, intermeeting_time;
    double lastend;

    for(int ite=0; ite<ITERATION; ite++){

        sprintf(filename,"meeting_epoch/trace%d.txt",ite+1);
        in = fopen(filename,"r");

        first = true;

        while(fscanf(in,"%lf %lf", &start, &end)!=EOF){

            meeting_time = end-start;
            //insert meeting time sample
            for(int i=0; i<SLOTS; i++){
                if((double)meeting_time> i*STEPM){
                    meet[i]++;
                }
            }

            if(!first){
                intermeeting_time = start - lastend;
                //insert inter meeting time sample
                for(int i=0; i<SLOTS; i++){
                    if(intermeeting_time > i*STEP+1){
                        interm[i]++;
                    }
                }
            }

            first = false;
            lastend = end;
       } 
 
       fclose(in);
    }

    out = fopen("meetingtimestat.txt","w");
    for(int i=0; i<SLOTS; i++){
        fprintf(out,"%lf %d\n", i*STEPM, meet[i]);
    }
    fflush(out);
    fclose(out);

    out = fopen("intermeetingtimestat.txt","w");
    for(int i=0; i<SLOTS; i++){
        fprintf(out,"%d %d\n", i*STEP+1, interm[i]);
    }
    fflush(out);
    fclose(out);

}
