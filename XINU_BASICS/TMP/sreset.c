/* sreset.c - sreset */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  sreset  --  reset the count and queue of a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL sreset(int sem, int count)
{
	STATWORD ps;    
	struct	sentry	*sptr;
	int	pid;
	int	slist;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[22]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadsem(sem) || count<0 || semaph[sem].sstate==SFREE) {
		restore(ps);
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[22] = proctab[currpid].syscall_exec[22] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	slist = sptr->sqhead;
	while ((pid=getfirst(slist)) != EMPTY)
		ready(pid,RESCHNO);
	sptr->semcnt = count;
	resched();
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[22] = proctab[currpid].syscall_exec[22] + (ctr1000 - syscall_start);
	return(OK);
}
