#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

static unsigned long *esp;

/*-------------------------------------------------------------------------------------------------------
 * printprocstks - print stack details and process details for process with larger prioriy than parameter
 *-------------------------------------------------------------------------------------------------------
 */

void printprocstks(int priority)
{
    
    int i;

    kprintf("\n\nvoid printprocstks(int priority)\n\n");
    for(i=0; i<NPROC; i++)
    {
        // Check the priority and the state for process before printing process details
	if( proctab[i].pprio > priority && proctab[i].pstate != PRFREE)
	{
	     kprintf("Process [%s]\n",proctab[i].pname);
             kprintf("\tpid: %d\n",i);
	     kprintf("\tpriority: %d\n",proctab[i].pprio);
	     kprintf("\tbase: 0x%08x\n",proctab[i].pbase);
	     kprintf("\tlimit: 0x%08x\n",proctab[i].plimit);
	     kprintf("\tlen: %d\n",proctab[i].pstklen);
	     //kprintf("\tcurrent process: %d \tno. active process: %d\n", currpid,numproc);    
	     if( i!= currpid){
             	kprintf("\tpointer: 0x%08x\n",proctab[i].pesp);
	     }
	     
	}

	// pointer for current process should be stack pointer esp 
	if(proctab[i].pprio > priority && i == currpid)
	{
	     asm("movl %esp,esp");
	     kprintf("\tpointer: 0x%08x\n", esp);
	}
    }  
}
