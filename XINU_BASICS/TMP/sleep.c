/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep(int n)
{
	STATWORD ps;    
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[18]++;
            syscall_start = ctr1000;
        }

	if (n<0 || clkruns==0) {
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[18] = proctab[currpid].syscall_exec[18] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	if (n == 0) {
	        disable(ps);
		resched();
		restore(ps);
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[18] = proctab[currpid].syscall_exec[18] + (ctr1000 - syscall_start);
		return(OK);
	}
	while (n >= 1000) {
		sleep10(10000);
		n -= 1000;
	}
	if (n > 0)
		sleep10(10*n);

        if (syscall_flag)
            proctab[currpid].syscall_exec[18] = proctab[currpid].syscall_exec[18] + (ctr1000 - syscall_start);
	return(OK);
}
