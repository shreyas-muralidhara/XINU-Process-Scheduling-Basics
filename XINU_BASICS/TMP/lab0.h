/* structure used for print system call summary*/

#ifndef _LAB0_H_
#define _LAB0_H_

long zfunction(long param);
void printsegaddress(void);
void printtos(void);
void printprocstks(int priority);
void printsyscallsummary(void);
void syscallsummary_start(void);
void syscallsummary_stop(void);

/*struct summarysyscall{
   int frequency[27];
   long syscall_exec[27];
};

extern struct summarysyscall procsyscall[];*/
  
#endif
