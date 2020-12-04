/* recvtim.c - recvtim */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  recvtim  -  wait to receive a message or timeout and return result
 *------------------------------------------------------------------------
 */
SYSCALL	recvtim(int maxwait)
{
	STATWORD ps;    
	struct	pentry	*pptr;
	int	msg;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[8]++;
            syscall_start = ctr1000;
        }

	if (maxwait<0 || clkruns == 0){
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[8] = proctab[currpid].syscall_exec[8] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	disable(ps);
	pptr = &proctab[currpid];
	if ( !pptr->phasmsg ) {		/* if no message, wait		*/
	        insertd(currpid, clockq, maxwait*1000);
		slnempty = TRUE;
		sltop = (int *)&q[q[clockq].qnext].qkey;
	        pptr->pstate = PRTRECV;
		resched();
	}
	if ( pptr->phasmsg ) {
		msg = pptr->pmsg;	/* msg. arrived => retrieve it	*/
		pptr->phasmsg = FALSE;
	} else {			/* still no message => TIMEOUT	*/
		msg = TIMEOUT;
	}
	restore(ps);

        if (syscall_flag)
            proctab[currpid].syscall_exec[8] = proctab[currpid].syscall_exec[8] + (ctr1000 - syscall_start);
	return(msg);
}
