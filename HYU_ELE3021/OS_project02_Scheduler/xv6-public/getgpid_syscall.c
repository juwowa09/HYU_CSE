#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
// getppid system call

int getgpid(void)
{
	return myproc()->parent->parent->pid;
}

int sys_getgpid(void)
{
	char *str;
	if (argstr(0, &str) < 0)
		return -1;
	return getgpid();
}
