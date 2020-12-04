/* stacktrace.c - stacktrace */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

static unsigned long	*esp;
static unsigned long	*ebp;
unsigned long ctr1000;
extern int syscall_flag;

#define STKDETAIL

/*------------------------------------------------------------------------
 * stacktrace - print a stack backtrace for a process
 *------------------------------------------------------------------------
 */
SYSCALL stacktrace(int pid)
{
	struct pentry	*proc = &proctab[pid];
	unsigned long	*sp, *fp;
	unsigned long syscall_start;

        if(syscall_flag){
            proctab[currpid].frequency[23]++;
            syscall_start = ctr1000;
        }

	if (pid != 0 && isbadpid(pid)) {
        	if (syscall_flag)
            		proctab[currpid].syscall_exec[23] = proctab[currpid].syscall_exec[23] + (ctr1000 - syscall_start);	
		return SYSERR;
	}
	if (pid == currpid) {
		asm("movl %esp,esp");
		asm("movl %ebp,ebp");
		sp = esp;
		fp = ebp;
	} else {
		sp = (unsigned long *)proc->pesp;
		fp = sp + 2; 		/* where ctxsw leaves it */
	}
	kprintf("sp %X fp %X proc->pbase %X\n", sp, fp, proc->pbase);
#ifdef STKDETAIL
	while (sp < (unsigned long *)proc->pbase) {
		for (; sp < fp; sp++)
			kprintf("DATA (%08X) %08X (%d)\n", sp, *sp, *sp);
		if (*sp == MAGIC)
			break;
		kprintf("\nFP   (%08X) %08X (%d)\n", sp, *sp, *sp);
		fp = (unsigned long *) *sp++;
		if (fp <= sp) {
			kprintf("bad stack, fp (%08X) <= sp (%08X)\n", fp, sp);
        		if (syscall_flag)
            			proctab[currpid].syscall_exec[23] = proctab[currpid].syscall_exec[23] + (ctr1000 - syscall_start);
			return SYSERR;
		}
		kprintf("RET  0x%X\n", *sp);
		sp++;
	}
	kprintf("MAGIC (should be %X): %X\n", MAGIC, *sp);
	if (sp != (unsigned long *)proc->pbase) {
		kprintf("unexpected short stack\n");
	        if (syscall_flag)
            		proctab[currpid].syscall_exec[23] = proctab[currpid].syscall_exec[23] + (ctr1000 - syscall_start);
		return SYSERR;
	}
#endif
        if (syscall_flag)
            proctab[currpid].syscall_exec[23] = proctab[currpid].syscall_exec[23] + (ctr1000 - syscall_start);
	return OK;
}
