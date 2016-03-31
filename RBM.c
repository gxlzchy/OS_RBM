#include <stdio.h>
#include <stdlib.h>

#define N 20  //The maximun acceptable number of request.

/*@param reqNum: int[] the request number of the requests
         st: int[] start time of the requests
         ed: int[] end time of the requests
         fNum: int[] the facility number of the requests
   the function takes in array of requests and schedule it with First Come First Serve algorithm
   return: returns a 2D array. the second dimension has 2 variables, first one is the request number
   second one indicates if it is accepted or rejected with a boolean(0 for false, 1 for true)
   The first dimension is just for indexing different requests
*/

int** FCFS(int reqNum[], int st[], int ed[],int fNum[]){
	int i;

	//define an array
    int **reqStatus = (int*)malloc(N*sizeof(int));
    for(i = 0; i < N; i ++){
    	reqStatus[i] = (int*)malloc(2*sizeof(int));
    }
    
    reqStatus[10][0]=3;
    return reqStatus;
}



int main(){
	int reqNum[N],st[N],ed[N],fNum[N];
	int **reqStatus=FCFS(reqNum,st,ed,fNum);
	printf("%d\n", reqStatus[10][1]);
	return 0;
}