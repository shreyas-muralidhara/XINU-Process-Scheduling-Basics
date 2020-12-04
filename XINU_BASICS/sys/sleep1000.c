/* sleep1000.c - sleep1000 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * sleep1000 --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
SYSCALL sleep1000(int n)
{
	STATWORD ps;    
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[21]++;
            syscall_start = ctr1000;
        }

	if (n < 0  || clkruns==0){
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[21] = proctab[currpid].syscall_exec[21] + (ctr1000 - syscall_start);
	        return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep1000(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[21] = proctab[currpid].syscall_exec[21] + (ctr1000 - syscall_start);
	return(OK);
}
