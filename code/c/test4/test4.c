#include <stdlib.h>
#include <stdio.h>

void test_fun_x()
{
	printf("test fun x\n");
}

int main()
{
	for(int i=0;i<10;i++)
	{
		printf("execute test_fun_x\n");
		test_fun_x();
	}
	
    return 0;
}
