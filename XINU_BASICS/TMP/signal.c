/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL signal(int sem)
{
	STATWORD ps;    
	register struct	sentry	*sptr;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[16]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
	        if (syscall_flag)
            		proctab[currpid].syscall_exec[16] = proctab[currpid].syscall_exec[16] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[16] = proctab[currpid].syscall_exec[16] + (ctr1000 - syscall_start);
	return(OK);
}
