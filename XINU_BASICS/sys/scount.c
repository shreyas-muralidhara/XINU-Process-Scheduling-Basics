/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>
#include <proc.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount(int sem)
{
extern	struct	sentry	semaph[];
unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[10]++;
            syscall_start = ctr1000;
        }

	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
	        if (syscall_flag)
        	    proctab[currpid].syscall_exec[10] = proctab[currpid].syscall_exec[10] + (ctr1000 - syscall_start);

		return(SYSERR);
	}
        if (syscall_flag)
            proctab[currpid].syscall_exec[10] = proctab[currpid].syscall_exec[10] + (ctr1000 - syscall_start);

	return(semaph[sem].semcnt);
}
