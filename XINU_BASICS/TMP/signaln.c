/* signaln.c - signaln */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  signaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
SYSCALL signaln(int sem, int count)
{
	STATWORD ps;    
	struct	sentry	*sptr;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[17]++;
            syscall_start = ctr1000;
        }
	
	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		restore(ps);
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[17] = proctab[currpid].syscall_exec[17] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore(ps);

        if (syscall_flag)
            proctab[currpid].syscall_exec[17] = proctab[currpid].syscall_exec[17] + (ctr1000 - syscall_start);
	return(OK);
}
