#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char* argv[])
{
	int res = getgpid();

	if (res == -1)
		exit();
	printf(1, "My student id is 2019060546\n");
	printf(1, "My pid is %d\n", getpid());
	printf(1, "My gpid is %d\n", getgpid());
	exit();
}
