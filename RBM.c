#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20  //The maximun acceptable number of request.
#define M 100 //The maximun duration of all requests is two weeks time: 2*5(per week)*9(9am-6pm)=90.
/**@param 
		reqNum: int[] reqNum[index]=the request number. 
!!!!!!!!!!!!Warning: The reqNum array should be in order.
		numOfReq: int, the number of request needed to be proceeded by FCFS. 
        st: int[]  st[reqNum]: the start time of the requests. 
        ed: int[]. ed[reqNum]: the end time of the requests. 
!!!!!!!!!!!!Warning: End time is included.
        fNum: int[][] 
         	fNum[reqNum][0]=The total number facilities in this request
         	fNum[reqNum][i]=facility Number.(i=1..3)
   @the function takes in array of requests and schedule it with First Come First Serve algorithm
   @return: returns a 2D array. the second dimension has 2 variables, first one is the request number
   second one 1:accepted, 0:rejected.
   The first dimension is just for indexing different requests
*/

int** FCFS(int reqNum[], int numOfReq, int st[], int ed[],int fNum[][5]){
	int i,j,k;

	//define an array
    int **reqStatus = (int**)malloc(N*sizeof(int*));
    for(i=0;i<N;i++)
    	reqStatus[i] = (int*)malloc(2*sizeof(int));
    
    //initialization
    for (i=0;i<N;i++){
    	reqStatus[i][0]=reqNum[i];
    	reqStatus[i][1]=1;
    }
    int status[11][M];						//status[fNum][time] 1:occupied, 0:vacant.
    for (i=0;i<11;i++)
    	for (j=0;j<M;j++)
    		status[i][j]=0;

    //core    
    for (i=0;i<numOfReq;i++){
    	for (j=1;j<=fNum[reqNum[i]][0];j++){
    		for (k=st[reqNum[i]];k<=ed[reqNum[i]];k++){
    			if (status[  fNum[ reqNum[i] ][j]  ][k]==1)
    				reqStatus[i][1]=0;}
    		if (reqStatus[i][1]==0) break;
    	}

    	if (reqStatus[i][1]==1){			//The request is accepted
    		for (j=1;j<=fNum[reqNum[i]][0];j++){
	    		for (k=st[reqNum[i]];k<=ed[reqNum[i]];k++)
	    			status[  fNum[ reqNum[i] ][j]  ][k]=1;
	    	}
    	}
    	
    	/*
    	for (j=1;j<=fNum[reqNum[i]][0];j++){
    		printf("facility #%d\n", fNum[reqNum[i]][j]);
	    		for (k=st[reqNum[i]];k<=ed[reqNum[i]];k++)
	    			printf("%d ",status[  fNum[ reqNum[i] ][j]  ][k]);
	    	 printf("\n");
    	}
	    printf("\n");*/
    }
    return reqStatus;
}



int main(){
	int reqNum[N],st[N],ed[N],fNum[N][5];
	int i;
	///*for testing
	//#1
	reqNum[0]=1;
	st[1]=2;
	ed[1]=4;
	fNum[1][0]=2;fNum[1][1]=1;fNum[1][2]=2;

	//#2
	reqNum[1]=4;
	st[4]=5;
	ed[4]=5;
	fNum[4][0]=2;fNum[4][1]=1;fNum[4][2]=3;

	//#3
	reqNum[2]=5;
	st[5]=2;
	ed[5]=3;
	fNum[5][0]=2;fNum[5][1]=3;fNum[5][2]=4;

	//*/

	int **reqStatus=FCFS(reqNum,3,st,ed,fNum);

	for (i=0;i<3;i++)
		printf("request #%d %s\n", reqStatus[i][0],reqStatus[i][1]==1?"accept":"reject");
	
	return 0;
}