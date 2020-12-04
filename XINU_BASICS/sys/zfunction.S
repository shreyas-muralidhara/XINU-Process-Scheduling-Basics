	.text
.globl zfunction
	.type zfunction, @function
zfunction: 		       #Save ebp to stack andmove the value of esp to ebp
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx 	       # Storing the register values
	movl 8(%ebp), %eax
	movl $0xfffc01ff, %ebx # Load params from stack and set mask bits 9 - 17
	andl %ebx, %eax        # Maskbits for the provided input
	movl $0x000000ff, %ebx 
	shrd $8, %ebx, %eax    # Shifting right by 8 bits using extended shift instruction 	
	popl %ebx	       # restore the base register			
	movl %ebp, %esp
	popl %ebp              # Restore the frame pointer
	ret
	
