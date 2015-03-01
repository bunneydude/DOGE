#include <stdio.h>
#include "test.h"
#include "task_list.h"

int main(int argc, char const *argv[])
{
	struct taskSlot a;
	a.counter = 1;
	printf("Hello wld! %d\n", print_task(&a) );
	return 0;
}
