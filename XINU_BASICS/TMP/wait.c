/* wait.c - wait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * wait  --  make current process wait on a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL	wait(int sem)
{
	STATWORD ps;    
	struct	sentry	*sptr;
	struct	pentry	*pptr;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[26]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[26] = proctab[currpid].syscall_exec[26] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	
	if (--(sptr->semcnt) < 0) {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		enqueue(currpid,sptr->sqtail);
		pptr->pwaitret = OK;
		resched();
		restore(ps);
	        if (syscall_flag)
            		proctab[currpid].syscall_exec[26] = proctab[currpid].syscall_exec[26] + (ctr1000 - syscall_start);
		return pptr->pwaitret;
	}
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[26] = proctab[currpid].syscall_exec[26] + (ctr1000 - syscall_start);
	return(OK);
}
