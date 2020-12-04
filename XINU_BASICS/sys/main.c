/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

int prX;
void halt();

//static unsigned long *esp;
//static unsigned long *ebp;
/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
prch(c)
char c;
{
	int i;
	sleep(5);	
}
int main()
{

	kprintf("\n\nHello World, Xinu lives\n\n");
	kprintf("zfunction (0xaabbccdd) -  0x%lx \n",zfunction(0xaabbccdd));
	printsegaddress();
	
	/* Commenting the below lines of code used to test stack frame contents
	kprintf("\n\n****************Testing print to stack**************************\n");
	asm("movl %esp,esp");
        asm("movl %ebp,ebp");
        int cal  = 26472;
        int val  = 47529;
        int i;
	kprintf("cal -%d - %08x\n",cal, cal);
	printf("cal - %d\n",cal);
	kprintf("val -%d - %08x\n",val, val);
	printf("val - %d\n", val);
	// Testing to print the stack frame before print to stack function
	kprintf("\nPrinting the stack contents Right before PrinttoS\n\n");
	
	
 	kprintf("ebp\t[0x%08x]: 0x%08x\n",ebp, *(ebp));

	for(i=1; i<=10; i++){
	   kprintf("ebp - %d [0x%08x]: 0x%08x\n",i,(ebp+i),*(ebp+i));
	}
	kprintf("esp\t[0x%08x]: 0x%08x\n",esp, *(esp));*/
	printtos();
 	
	printprocstks(-1);
	
	syscallsummary_start();        
	resume(prX = create(prch,2000,20,"proc X",1,'A'));
	sleep(10);
	syscallsummary_stop();
	printsyscallsummary();	
	
	return 0;
}
