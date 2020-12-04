/* screate.c - screate, newsem */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

LOCAL int newsem();
unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * screate  --  create and initialize a semaphore, returning its id
 *------------------------------------------------------------------------
 */
SYSCALL screate(int count)
{
	STATWORD ps;    
	int	sem;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[15]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if ( count<0 || (sem=newsem())==SYSERR ) {
		restore(ps);
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[15] = proctab[currpid].syscall_exec[15] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	semaph[sem].semcnt = count;
	/* sqhead and sqtail were initialized at system startup */
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[15] = proctab[currpid].syscall_exec[15] + (ctr1000 - syscall_start);
	return(sem);
}

/*------------------------------------------------------------------------
 * newsem  --  allocate an unused semaphore and return its index
 *------------------------------------------------------------------------
 */
LOCAL int newsem()
{
	int	sem;
	int	i;

	for (i=0 ; i<NSEM ; i++) {
		sem=nextsem--;
		if (nextsem < 0)
			nextsem = NSEM-1;
		if (semaph[sem].sstate==SFREE) {
			semaph[sem].sstate = SUSED;
			return(sem);
		}
	}
	return(SYSERR);
}
