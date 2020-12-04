/* sdelete.c - sdelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * sdelete  --  delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
SYSCALL sdelete(int sem)
{
	STATWORD ps;    
	int	pid;
	struct	sentry	*sptr;
	unsigned long syscall_start;

	if(syscall_flag){
            proctab[currpid].frequency[11]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		restore(ps);
	        if (syscall_flag)
        	    proctab[currpid].syscall_exec[11] = proctab[currpid].syscall_exec[11] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	sptr->sstate = SFREE;
	if (nonempty(sptr->sqhead)) {
		while( (pid=getfirst(sptr->sqhead)) != EMPTY)
		  {
		    proctab[pid].pwaitret = DELETED;
		    ready(pid,RESCHNO);
		  }
		resched();
	}
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[11] = proctab[currpid].syscall_exec[11] + (ctr1000 - syscall_start);
	return(OK);
}
