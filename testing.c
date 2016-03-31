#include <stdio.h>
#include <stdlib.h>

int** function()
{
    int ** a = (int **)malloc(50*sizeof(int*));
    int i;
    for (i=0;i<40;i++)
    	a[i]=(int*)malloc(3*sizeof(int));
    
    a[1][1]=9;
    return a;
}

int main()
{
    int** a = function();
    printf("%d\n", a[1][1]);
    return 0;
}