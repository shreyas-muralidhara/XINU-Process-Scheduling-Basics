#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

int syscall_flag;
char syscall[27][10] = {"freemem","chprio","getpid","getprio","gettime","kill","receive","recvclr","recvtim","resume","scount","sdelete","send","setdev","setnok","screate","signal","signaln","sleep","sleep10","sleep100","sleep1000","sreset","stacktrace","suspend","unsleep","wait"};

/*-----------------------------------------------------------------------------------------------------------
 *  printsyscallsummary  --  function to print the frequency and run durationby system call for each process. 
 *-----------------------------------------------------------------------------------------------------------
 */

void syscallsummary_start()
{
     int i = 0,j = 0;
     syscall_flag = 1;
	
     for(i = 0; i < NPROC; i++){
	for(j=0; j < (sizeof(syscall)/sizeof(syscall[0])); j++){
	   proctab[i].frequency[j] = 0;
	   proctab[i].syscall_exec[j] = 0;
        }
     }
	    
}

 
void syscallsummary_stop(){
     syscall_flag = 0;
}


void printsyscallsummary()
{
     int i =0, j=0;
     int first_syscall = 0;
     kprintf("\n\nvoid printsyscallsummary()\n\n");
     
     for(i=0; i<NPROC; i++){
	for(j=0; j < (sizeof(syscall)/sizeof(syscall[0])); j++) {
          
            if( proctab[i].frequency[j] > 0) 
            {
		 if(first_syscall == 0)
                 {
		     kprintf("Process [pid:%d]\n", i);
                     first_syscall = 1;
                 }
                 long avg_exec =( proctab[i].syscall_exec[j] / proctab[i].frequency[j] ); 
		 kprintf("\tSyscall: sys_%s, count: %d, average execution time: %d (ms)\n", syscall[j], proctab[i].frequency[j], avg_exec);
            }
        }
        first_syscall = 0;
     }
}		    
