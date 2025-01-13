#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

// simple system call
int
myfunction(char *str)
{
	cprintf("%s\n", str);
	return 0xABCDABCD;
}
//Wrapper for my_syscall
int
sys_myfunction(void)
{
	char *str;
	//Decode argument using argstr
	if(argstr(0, &str) < 0)
		return -1;
	return myfunction(str);
}

void test_c(void)
{
	cprintf("ticks = %d, pid = %d, name = %s\n", ticks, myproc()->pid, myproc()->name);
}

int sys_test_c(void)
{
	char* str;
	if (argstr(0, &str) < 0)
		return -1;
	test_c();
	return 1;
}
