/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL getpid()
{
	unsigned long syscall_start;
	
        if(syscall_flag){
            proctab[currpid].frequency[2]++;
            syscall_start = ctr1000;
        }
	
        if (syscall_flag)
            proctab[currpid].syscall_exec[2] = proctab[currpid].syscall_exec[2] + (ctr1000 - syscall_start);

	return(currpid);
}
