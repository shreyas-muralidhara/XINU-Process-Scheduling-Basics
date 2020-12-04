/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
SYSCALL	setdev(int pid, int dev1, int dev2)
{
	short	*nxtdev;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[13]++;
            syscall_start = ctr1000;
        }

	if (isbadpid(pid)){
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[13] = proctab[currpid].syscall_exec[13] + (ctr1000 - syscall_start);
		return(SYSERR);
	}
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;
	
	if (syscall_flag)
            proctab[currpid].syscall_exec[13] = proctab[currpid].syscall_exec[13] + (ctr1000 - syscall_start);
	return(OK);
}
