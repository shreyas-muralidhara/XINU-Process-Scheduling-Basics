/* setnok.c - setnok */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  setnok  -  set next-of-kin (notified at death) for a given process
 *------------------------------------------------------------------------
 */
SYSCALL	setnok(int nok, int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[14]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[14] = proctab[currpid].syscall_exec[14] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	pptr = &proctab[pid];
	pptr->pnxtkin = nok;
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[14] = proctab[currpid].syscall_exec[14] + (ctr1000 - syscall_start);
	return(OK);
}
