#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

static unsigned long *esp;
static unsigned long *ebp;

/*--------------------------------------------------------------------------------------------------
 *    printtos  --  print address and contents of run time stack,upto 4 loactions(can be 1,2,3 or 4) 
 *--------------------------------------------------------------------------------------------------
 */

void printtos()
{
   asm("movl %esp,esp");
   asm("movl %ebp,ebp");
   int i=1;  
   
   /* Commenting below lines of code to print the stack trace after the call
   int cal = 87465;
   int val = 28374;

   kprintf("\n\ncal -%d - %08x\n",cal, cal);
   printf("cal - %d\n",cal);
   kprintf("val -%d - %08x\n",val, val);
   printf("val - %d\n", val);
 
   kprintf("\nPrinting the stack right after entering the PrinttoS\n");

   for(i=1;i<=7;i++){
      kprintf("ebp + %d [0x%08x]: 0x%08x\n",i,(ebp-i),*(ebp-i));
   }
   kprintf("ebp\t[0x%08x]: 0x%08x\n",ebp, *(ebp));

   for(i=1; i<=10; i++){
       kprintf("ebp - %d [0x%08x]: 0x%08x\n",i,(ebp+i),*(ebp+i));
   }
   kprintf("esp\t[0x%08x]: 0x%08x\n",esp, *(esp));*/

   kprintf("\n\nvoid printtos()\n\n");
   kprintf("Before[0x%08x]: 0x%08x\n", ebp+2, *(ebp+2));
   kprintf("After[0x%08x]: 0x%08x\n", ebp, *(ebp));

   /* Printing 4 or fewer item pushed onto stack*/
   while(i <= 4 && ebp > (esp + i)){
      kprintf("\telement[0x%08x]: 0x%08x\n", esp+i,*(esp+i));
      i++;
   } 

 
}
