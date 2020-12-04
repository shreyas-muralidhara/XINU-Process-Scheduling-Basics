/* recvclr.c - recvclr */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  recvclr  --  clear messages, returning waiting message (if any)
 *------------------------------------------------------------------------
 */
SYSCALL	recvclr()
{
	STATWORD ps;    
	WORD	msg;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[7]++;
            syscall_start = ctr1000;
        }

	disable(ps);
	if (proctab[currpid].phasmsg) {
		proctab[currpid].phasmsg = 0;
		msg = proctab[currpid].pmsg;
	} else
		msg = OK;
	restore(ps);
        if (syscall_flag)
            proctab[currpid].syscall_exec[7] = proctab[currpid].syscall_exec[7] + (ctr1000 - syscall_start);
	
	return(msg);
}
