/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
SYSCALL getprio(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;
	unsigned long syscall_start;

	if(syscall_flag){
            proctab[currpid].frequency[3]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
	        if (syscall_flag)
            		proctab[currpid].syscall_exec[3] = proctab[currpid].syscall_exec[3] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[3] = proctab[currpid].syscall_exec[3] + (ctr1000 - syscall_start);

	return(pptr->pprio);
}
