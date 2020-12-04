#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <sched.h>
#include <math.h>
#include <stdio.h>

int sched_type;

/* This function changes the scheduler type to either EXPDISTSCHED or LINUXSCHED */
void setschedclass(int sched_class){
    sched_type = sched_class;
}


/* This function returns the scheduling class, i.e. either EXPDISTSCHED or LINUXSCHED */
int getschedclass(){
    return(sched_type);
}
