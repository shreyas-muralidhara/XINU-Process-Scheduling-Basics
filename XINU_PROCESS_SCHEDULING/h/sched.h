/* schedule.h - set schedule, get schedule,  */
#ifndef _SCHED_H_
#define _SCHED_H_

#define EXPDISTSCHED 1
#define LINUXSCHED 2

void setschedclass (int sched_class);
int getschedclass(void);

extern int sched_type;

#endif
