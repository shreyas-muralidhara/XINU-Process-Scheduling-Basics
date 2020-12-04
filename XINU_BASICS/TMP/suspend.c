/* suspend.c - suspend */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  suspend  --  suspend a process, placing it in hibernation
 *------------------------------------------------------------------------
 */
SYSCALL	suspend(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority returned		*/
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[24]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadpid(pid) || pid==NULLPROC ||
	 ((pptr= &proctab[pid])->pstate!=PRCURR && pptr->pstate!=PRREADY)) {
		restore(ps);
	        if (syscall_flag)
            		proctab[currpid].syscall_exec[24] = proctab[currpid].syscall_exec[24] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	if (pptr->pstate == PRREADY) {
		pptr->pstate = PRSUSP;
		dequeue(pid);
	}
	else {
		pptr->pstate = PRSUSP;
		resched();
	}
	prio = pptr->pprio;
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[24] = proctab[currpid].syscall_exec[24] + (ctr1000 - syscall_start);
	return(prio);
}
