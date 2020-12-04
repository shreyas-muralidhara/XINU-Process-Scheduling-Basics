/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include <proc.h>

extern int getutim(unsigned long *);

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
    /* long	now; */
    unsigned long syscall_start;
    if(syscall_flag){
        proctab[currpid].frequency[4]++;
        syscall_start = ctr1000;
    }
	
	/* FIXME -- no getutim */
    if (syscall_flag)
        proctab[currpid].syscall_exec[4] = proctab[currpid].syscall_exec[4] + (ctr1000 - syscall_start);
   
     return OK;
}
