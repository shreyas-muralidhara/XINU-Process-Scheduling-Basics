/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	STATWORD ps;    
	struct	pentry	*pptr;
	unsigned long syscall_start;
	
        if(syscall_flag){
            proctab[currpid].frequency[1]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);

		if (syscall_flag)
            		proctab[currpid].syscall_exec[1] = proctab[currpid].syscall_exec[1] + (ctr1000 - syscall_start);		

		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[1] = proctab[currpid].syscall_exec[1] + (ctr1000 - syscall_start);

	return(newprio);
}
