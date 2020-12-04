/* resume.c - resume */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * resume  --  unsuspend a process, making it ready; return the priority
 *------------------------------------------------------------------------
 */
SYSCALL resume(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority to return		*/
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[9]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate!=PRSUSP) {
		restore(ps);
	        if (syscall_flag)
            		proctab[currpid].syscall_exec[9] = proctab[currpid].syscall_exec[9] + (ctr1000 - syscall_start);

		return(SYSERR);
	}
	prio = pptr->pprio;
	ready(pid, RESCHYES);
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[9] = proctab[currpid].syscall_exec[9] + (ctr1000 - syscall_start);

	return(prio);
}
