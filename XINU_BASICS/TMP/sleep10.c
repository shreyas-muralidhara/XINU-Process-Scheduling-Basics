/* sleep10.c - sleep10 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * sleep10  --  delay the caller for a time specified in tenths of seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep10(int n)
{
	STATWORD ps;    
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[19]++;
            syscall_start = ctr1000;
        }

	if (n < 0  || clkruns==0) {
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[19] = proctab[currpid].syscall_exec[19] + (ctr1000 - syscall_start);
	        return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep10(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n*100);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);

        if (syscall_flag)
            proctab[currpid].syscall_exec[19] = proctab[currpid].syscall_exec[19] + (ctr1000 - syscall_start);
	return(OK);
}
