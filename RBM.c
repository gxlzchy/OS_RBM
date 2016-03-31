#include <stdio.h>
#include <stdlib.h>

#define N 20  //The maximun acceptable number of request.
#define M 100 //The maximun duration of all requests is two weeks time: 2*5(per week)*9(9am-6pm)=90.
/**@param reqNum: int[] the request number of the requests
         st: int[]  st[reqNum]: the start time of the requests. 
         ed: int[]. ed[reqNum]: the end time of the requests. 
         fNum: int[][] 
         	fNum[reqNum][0]=The total number facilities in this request
         	fNum[reqNum][i]=facility Number.(i=1..3)
   @the function takes in array of requests and schedule it with First Come First Serve algorithm
   @return: returns a 2D array. the second dimension has 2 variables, first one is the request number
   second one 1:accepted, 0:rejected.
   The first dimension is just for indexing different requests
*/

int** FCFS(int reqNum[], int st[], int ed[],int fNum[][]){
	int i,j,k;
	st[reqStatus[i][0]]

	//define an array
    int **reqStatus = (int*)malloc(N*sizeof(int));
    for(i=0;i<N;i++){
    	reqStatus[i] = (int*)malloc(2*sizeof(int));
    }

    //initialization
    for (i=0;i<N;i++){
    	reqStatus[i][0]=reqNum[i];
    	reqStatus[i][1]=1;
    }
    int status[10][M]						//status[fNum][time] 1:occupied, 0:vacant.
    for (i=0;i<10;i++)
    	for (j=0;j<M;j++)
    		status[i][j]=0;

    //core    
    for (i=0;i<strlen(reqNum);i++){

    	for (j=1;j<fNum[reqNum[i]][0];j++){
    		for (k=st[reqNum[i]];k<=ed[reqNum[i]];k++)
    			if (status[  fNum[ reqNum[i] ][j]  ][k]==1)
    				reqStatus[i][1]=0;
    		if (reqStatus[i][1]==0) break;
    	}

    	if (reqStatus[i][1]==1){			//The request is accepted
    		for (j=1;j<fNum[reqNum[i]][0];j++){
	    		for (k=st[reqNum[i]];k<=ed[reqNum[i]];k++)
	    			reqStatus[i][1]=1;
	    	}
    	}
    	
    }

    return reqStatus;
}



int main(){
	int reqNum[N],st[N],ed[N],fNum[N];
	int **reqStatus=FCFS(reqNum,st,ed,fNum);
	printf("%d\n", reqStatus[10][1]);
	return 0;
}