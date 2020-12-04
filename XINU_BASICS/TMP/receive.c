/* receive.c - receive */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  receive  -  wait for a message and return it
 *------------------------------------------------------------------------
 */
SYSCALL	receive()
{
	STATWORD ps;    
	struct	pentry	*pptr;
	WORD	msg;
	unsigned long syscall_start;

	if(syscall_flag){
            proctab[currpid].frequency[6]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	pptr = &proctab[currpid];
	if ( !pptr->phasmsg ) {		/* if no message, wait for one	*/
		pptr->pstate = PRRECV;
		resched();
	}
	msg = pptr->pmsg;		/* retrieve message		*/
	pptr->phasmsg = FALSE;
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[6] = proctab[currpid].syscall_exec[6] + (ctr1000 - syscall_start);

	return(msg);
}
