/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <math.h>
#include <sched.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
int epoch_time;

/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:   Upon entry, currpid gives current process id.
 * 	Proctab[currpid].pstate gives correct NEXT state for
 *     	    current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	if( sched_type == EXPDISTSCHED ){		
                /* no switch needed if NULLPROC is running and queue is empty  */
	
		/*if( (optr= &proctab[currpid])->pstate == PRCURR && ( (q[rdyhead].qnext == NULLPROC) && (q[q[rdytail].qprev].qprev == rdyhead) ) )
			return (OK);*/
		
		/* force context switch */

                if ( (optr= &proctab[currpid])->pstate == PRCURR){
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);		
		}


		/* Unless the queue is empty perform scheduling using exponential distribution */		
		
		if ( q[rdytail].qprev != rdyhead){
			// Initialize the lambda value to 0.1 as specified
			int rand_prio = (int)expdev(0.1); 
			//kprintf(" the random value is %d\n",rand_prio);
		
			
			/* Reading the queue in reverse order, to schedule the process in round robin for processes with same riority */	
			int curr = q[rdytail].qprev;
	
			while((q[curr].qprev != rdyhead) && (rand_prio < q[q[curr].qprev].qkey))
				curr = q[curr].qprev;
		
			nptr = &proctab[(currpid = dequeue(curr))];
		} else{
			nptr = &proctab[NULLPROC];
		}		 

	        nptr->pstate = PRCURR;          /* mark it currently running    */
	#ifdef  RTCLOCK
        	preempt = QUANTUM;              /* reset preemption counter     */
	#endif

	        ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);

	        /* The OLD process returns here when resumed. */
	        return OK;

	}
	else if(sched_type == LINUXSCHED )
	{
		/* Block 1 - initialize the priority for the next epoch */
		int i;
		epoch_time = epoch_time + proctab[currpid].counter - preempt;	      
		
		if(epoch_time >= 0)
		{    
		    if(preempt <=0) 
		    {	// If the clock ticks of premption are complete then reset the counter and goodness for current process,
			// no change to the priority of process
			proctab[currpid].counter = 0;
			proctab[currpid].goodness = 0;
		    } 
		    else{ // If the prempted value has tick remaining then reassign the remaining ticks to the counter 
			  // and goodness = counter + priority 
			proctab[currpid].counter = preempt; 
			proctab[currpid].goodness = proctab[currpid].counter + proctab[currpid].priority;
		    }	
		}

		/* Block 2 - Insert the current process at the end of ready queue, based on the goodness value. 
                   force context switch */
                if ( (optr= &proctab[currpid])->pstate == PRCURR){
                    optr->pstate = PRREADY;
                    insert(currpid,rdyhead,proctab[currpid].goodness);
                }

		

		/* Block 3 - Check for time quantum of any runnable process in this epoch
 * 		   if the total clock ticks is zero anf remaining epoch time is also 0 	
    		   then no process available, we create a new epoch */
		int epoch_quantum = 0;		
		for(i = 0; i<NPROC; i++)
		    if( (proctab[i].pstate == PRREADY))
			epoch_quantum = epoch_quantum + proctab[i].goodness;

	
		/* when epoch is 0 or remaining time is 0 */
		if( (epoch_time <= 0)  || ( epoch_quantum <= 0) )
		{
		    epoch_time = 0;
		    for( i=0; i<NPROC; i++ )

			/* Initialize the epoch only for the runnable process */
                        if ( proctab[i].pstate != PRFREE )
			{

			    proctab[i].priority = proctab[i].pprio;	
			    proctab[i].goodness = proctab[i].counter + proctab[i].priority;
			    proctab[i].counter = (proctab[i].counter/2) + proctab[i].priority;

                            /* Maximum CPU time initialized for the epoch*/
			    epoch_time = epoch_time + proctab[i].priority;
			}
		}

		
		/* BLOCK 4 - Schedule the next runnable process with the highest goodness. 
  		   Use round Robin stratergy if the process is of same priority */
		int pid = NPROC-1,max_goodness = proctab[0].goodness;

		for(i=NPROC-1; i > 0; i--)
		{	
		    if( (proctab[i].goodness > max_goodness) && (proctab[i].pstate == PRREADY) && (proctab[i].counter > 0) ){
			max_goodness = proctab[i].goodness;
			pid = i;
		    }	 
		}

		nptr = &proctab[ (currpid = dequeue(pid)) ];

	        nptr->pstate = PRCURR;          /* mark it currently running    */
        #ifdef  RTCLOCK
                preempt = proctab[currpid].counter;          /* reset preemption counter     */
        #endif

        	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);

        	/* The OLD process returns here when resumed. */
        	return OK;

	}

	/* Default scheduler is initialized when sched type is other than 1 and 2 */
	else
	{
		/* no switch needed if current process priority higher than next*/
		if ( ( (optr= &proctab[currpid])->pstate == PRCURR ) && (lastkey(rdytail) < optr->pprio) )
			return(OK);
	
		/* force context switch */
		if (optr->pstate == PRCURR) 
		{
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}

		/* remove highest priority process at end of ready list */
		nptr = &proctab[ (currpid = getlast(rdytail)) ];
	

		nptr->pstate = PRCURR;		/* mark it currently running	*/
	#ifdef	RTCLOCK
		preempt = QUANTUM;		/* reset preemption counter	*/
	#endif
	
		ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
		/* The OLD process returns here when resumed. */
		return OK;
	}
}

