// test_yield.c
#include "types.h"
#include "user.h"

int main() {
  int pid = fork();
  if (pid < 0) {
    printf(1, "fork failed\n");
    exit();
  }

  if (pid == 0) 
  {
    while(1)
	{
		printf(1, "Child\n");
		yield();
	}
  } 
  else 
  {
    while(1)
	{
      	printf(1, "Parent\n");
      	yield();
    }
  }
}
