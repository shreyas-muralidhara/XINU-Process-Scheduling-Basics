/* send.c - send */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  send  --  send a message to another process
 *------------------------------------------------------------------------
 */
SYSCALL	send(int pid, WORD msg)
{
	STATWORD ps;    
	struct	pentry	*pptr;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[12]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadpid(pid) || ( (pptr= &proctab[pid])->pstate == PRFREE)
	   || pptr->phasmsg != 0) {
		restore(ps);
	        if (syscall_flag)
            		proctab[currpid].syscall_exec[12] = proctab[currpid].syscall_exec[12] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg = TRUE;
	if (pptr->pstate == PRRECV)	/* if receiver waits, start it	*/
		ready(pid, RESCHYES);
	else if (pptr->pstate == PRTRECV) {
		unsleep(pid);
		ready(pid, RESCHYES);
	}
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[12] = proctab[currpid].syscall_exec[12] + (ctr1000 - syscall_start);
	return(OK);
}
