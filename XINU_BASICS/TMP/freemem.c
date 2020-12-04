/* freemem.c - freemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <stdio.h>
#include <proc.h>

unsigned long ctr1000;
extern int syscall_flag;

/*------------------------------------------------------------------------
 *  freemem  --  free a memory block, returning it to memlist
 *------------------------------------------------------------------------
 */
SYSCALL	freemem(struct mblock *block, unsigned size)
{
	STATWORD ps;    
	struct	mblock	*p, *q;
	unsigned top;
	unsigned long syscall_start;

	if(syscall_flag){
	    proctab[currpid].frequency[0]++;
            syscall_start = ctr1000;
	} 

	if (size==0 || (unsigned)block>(unsigned)maxaddr
	    || ((unsigned)block)<((unsigned) &end)) {
	     if (syscall_flag)
		proctab[currpid].syscall_exec[0] = proctab[currpid].syscall_exec[0] + (ctr1000 - syscall_start) ;
	     return(SYSERR);
	}
	size = (unsigned)roundmb(size);
	disable(ps);
	for( p=memlist.mnext,q= &memlist;
	     p != (struct mblock *) NULL && p < block ;
	     q=p,p=p->mnext )
		;
	if (((top=q->mlen+(unsigned)q)>(unsigned)block && q!= &memlist) ||
	    (p!=NULL && (size+(unsigned)block) > (unsigned)p )) {
		restore(ps);

            	if (syscall_flag)
                   proctab[currpid].syscall_exec[0] = proctab[currpid].syscall_exec[0] + (ctr1000 - syscall_start) ;

		return(SYSERR);
	}
	if ( q!= &memlist && top == (unsigned)block )
			q->mlen += size;
	else {
		block->mlen = size;
		block->mnext = p;
		q->mnext = block;
		q = block;
	}
	if ( (unsigned)( q->mlen + (unsigned)q ) == (unsigned)p) {
		q->mlen += p->mlen;
		q->mnext = p->mnext;
	}
	restore(ps);

	if (syscall_flag)
                proctab[currpid].syscall_exec[0] = proctab[currpid].syscall_exec[0] + (ctr1000 - syscall_start) ;


	return(OK);
}
