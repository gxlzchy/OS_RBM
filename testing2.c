#include <stdio.h>
int main(){
	char input[80],input1[80];
	fgets(input,80,stdin);
	printf("%s\n", input);
	fgets(input,80,stdin);
	printf("%s\n", input);
	return 0;
}