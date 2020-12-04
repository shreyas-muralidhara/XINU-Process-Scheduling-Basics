#include <stdio.h>

extern int etext, edata, end;

/*------------------------------------------------------------------------------------------------------------
 *    printsegaddress  --  print the end of text data and segments of the current,preceding and After process
 *------------------------------------------------------------------------------------------------------------
 */

void printsegaddress()
{
  /* kprintf("sizeof(int):%d\n", sizeof(int)); - confirming the size of int is 4 on system that I am running`*/
  kprintf("\nvoid printsegaddress()\n\n");
  kprintf("Current: etext[0x%08x]=[0x%08x], edata[0x%08x]=0x%08x, ebss[0x%08x]=0x%08x\n", &etext, etext, &edata, edata, &end, end);
  kprintf("Preceding: etext[0x%08x]=[0x%08x], edata[0x%08x]=0x%08x, ebss[0x%08x]=0x%08x\n", &etext-1, *(&etext-1), &edata-1, *(&edata-1), &end-1, (&end-1));
  kprintf("After: etext[0x%08x]=[0x%08x], edata[0x%08x]=0x%08x, ebss[0x%08x]=0x%08x\n", &etext+1, *(&etext+1), &edata+1, *(&edata+1), &end+1, *(&end+1));

}
