#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20    // The maximun acceptable number of request.
#define M 90    // The maximun duration of all requests is two weeks time: 2*5(per week)*9(9am-6pm)=90.

/*
Use the following id numbers to represent different components:
1 -- room_A
2 -- room_B
3 -- webcam_720p
4 -- webcam_1080p
5 -- monitor_50
6 -- monitor_75
7 -- projector_fhd
8 -- projector_xga
9 -- screen_100
10 -- screen_150
11 -- tenant_A
12 -- tenant_B
13 -- tenant_C
*/


/**
 * @brief      compare two variables to prepare for qsort in greedy()
 *
 * @param[in]  pa    1st array for comparing
 * @param[in]  pb    2nd array for comparing
 *
 * @return     -1: better, 0: equal, 1: Worse
 */
int cmp(const void *pa, const void *pb){
    const int (*a)[3] = pa;
    const int (*b)[3] = pb;
    
    if ( (*a)[2] > (*b)[2] ) return -1;
    if ( (*a)[2] < (*b)[2] ) return +1;
    if ( (*a)[2] == (*b)[2] ){
        if ( (*a)[1] < (*b)[1] ) return -1;
        if ( (*a)[1] > (*b)[1] ) return +1;
        return 0;
    }
}

/**
 * @brief      the function takes in array of requests and schedule it with First Come First Serve algorithm.
 *
 * @param      reqNum    reqNum[index]=the request number
 * @param[in]  numOfReq  the number of request needed to be proceeded by FCFS.
 * @param      st        st[reqNum]: the start time of the requests.
 * @param      ed        ed[reqNum]: the end time of the requests.
 * 						 End time is inclusive.
 * @param      fNum      fNum[reqNum][0]=The total number facilities in this request
 *          			 fNum[reqNum][i]=facility Number.(i=1..3)
 *
 * @return     returns a 2D array. the second dimension has 2 variables:
 *      			1st one is the request number
 *     				2nd one 1:accepted, 0:rejected.
 */
int** FCFS(int reqNum[], int numOfReq, int st[], int ed[],int fNum[][5]){
    int i,j,k;

    //define an array
    int **reqStatus = (int**)malloc(N*sizeof(int*));
    for(i=0;i<N;i++)
        reqStatus[i] = (int*)malloc(2*sizeof(int));
    
    //initialization
    for (i=0;i<numOfReq;i++){
        reqStatus[i][0]=reqNum[i];
        reqStatus[i][1]=1;
    }
    int status[11][M];                      //status[fNum][time] 1:occupied, 0:vacant.
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

        if (reqStatus[i][1]==1){            //The request is accepted
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


/**
 * @brief      the function takes in array of requests and schedule it with greedy algorithm
 *
 * @param      reqNum    reqNum[index]=the request number
 * @param[in]  numOfReq  the number of request needed to be proceeded by greedy
 * @param      st        st[reqNum]: the start time of the requests.
 * @param      ed        ed[reqNum]: the end time of the requests.
 * 						 End time is inclusive.
 * @param      fNum      fNum[reqNum][0]=The total number facilities in this request
 *          			 fNum[reqNum][i]=facility Number.(i=1..3)
 *
 * @return     @return     returns a 2D array. the second dimension has 2 variables:
 *      			1st one is the request number
 *     				2nd one 1:accepted, 0:rejected.
 */
int** greedy(int reqNum[], int numOfReq, int st[], int ed[],int fNum[][5]){
    int i,j,k;
    
    //combine reqNum, st, ed, into an array for qsort
    int requests[N][3];
    
    //initialization
    for (i=0;i<numOfReq;i++){
        requests[i][0]=reqNum[i];
        requests[i][1]=st[reqNum[i]];
        requests[i][2]=ed[reqNum[i]];
    }
    qsort(requests, numOfReq, sizeof(requests[0]), cmp);
    
    //for testing
    for (i=0;i<numOfReq;i++){
        printf("%d %d %d \n", requests[i][0],requests[i][1],requests[i][2]);
    }
    printf("\n");
    
    int *a = (int*)malloc(N*sizeof(int));;
    int *b = (int*)malloc(N*sizeof(int));;
    int *c = (int*)malloc(N*sizeof(int));;
    for(i = 0; i < numOfReq; i++){
        a[i] = requests[i][0];
        b[a[i]] = requests[i][1];
        c[a[i]] = requests[i][2];
    }
    return FCFS(a, numOfReq, b, c, fNum);
}

/**
 * @brief      reschedule the rejected requests
 *
 * @param      reqNum               reqNum[index]=the request number
 * @param[in]  numOfReq             the number of request needed to be proceeded
 *                                  by greedy
 * @param      st                   st[reqNum]: the start time of the requests.
 * @param      ed                   ed[reqNum]: the end time of the requests.
 *                                  End time is inclusive.
 * @param      fNum                 fNum[reqNum][0]=The total number facilities
 *                                  in this request fNum[reqNum][i]=facility
 *                                  Number.(i=1..3)
 * @param[in]  rescheduleStartTime  a time point from which on, the requests will assigned a new time after this time. 
 *
 * @return     { description_of_the_return_value }
 * @return     returns a 2D array. the second dimension has 2 variables: 1st one
 *             is the request number 2nd one start time of the request.
 * @warning    The start time of request may go beyond the convention: two weeks time. M may exceed the boundary.
 */
int** reschedule(int reqNum[], int numOfReq, int st[], int ed[],int fNum[][5], int rescheduleStartTime){
    int i,j,k;
    
    //define an array
    int **reqStatus = (int**)malloc(N*sizeof(int*));
    for(i=0;i<N;i++)
        reqStatus[i] = (int*)malloc(2*sizeof(int));
    
    //initialization
    for (i=0;i<numOfReq;i++){
        reqStatus[i][0]=reqNum[i];
        reqStatus[i][1]=ed[reqNum[i]]-st[reqNum[i]]+1;	//duration temp.
    }

    //core
    int x=rescheduleStartTime;
    for (i=0;i<numOfReq;i++){
    	int duration=reqStatus[i][1];
    	reqStatus[i][1]=x;
    	x+=duration;
    }

    return reqStatus;
}

/**
 * @brief      Main Program
 */
int main(){
    int reqNum[N], st[N], ed[N], fNum[N][5];    // request id, starting time, ending time, number of facilities
    int i, j, k;
    // ###################################################
    // ################ Scheduling Module ################
    // ###################################################
    // for testing
    reqNum[0]=1;
    st[1]=1;
    ed[1]=5;
    fNum[1][0]=2;fNum[1][1]=1;fNum[1][2]=3;

    reqNum[1]=2;
    st[2]=2;
    ed[2]=4;
    fNum[2][0]=1;fNum[2][1]=2;

    reqNum[2]=3;
    st[3]=3;
    ed[3]=3;
    fNum[3][0]=1;fNum[3][1]=2;

    reqNum[3]=4;
    st[4]=3;
    ed[4]=4;
    fNum[4][0]=1;fNum[4][1]=2;

    reqNum[4]=5;
    st[5]=5;
    ed[5]=5;
    fNum[5][0]=1;fNum[5][1]=2;

    reqNum[5]=6;
    st[6]=4;
    ed[6]=6;
    fNum[6][0]=1;fNum[6][1]=1;

    reqNum[6]=7;
    st[7]=1;
    ed[7]=4;
    fNum[7][0]=1;fNum[7][1]=1;

    reqNum[7]=8;
    st[8]=2;
    ed[8]=3;
    fNum[8][0]=1;fNum[8][1]=1;

    reqNum[8]=9;
    st[9]=3;
    ed[9]=5;
    fNum[9][0]=1;fNum[9][1]=3;

    reqNum[9]=10;
    st[10]=6;
    ed[10]=6;
    fNum[10][0]=1;fNum[10][1]=3;

    reqNum[10]=11;
    st[11]=1;
    ed[11]=1;
    fNum[11][0]=1;fNum[11][1]=2;
    
    int num_requests = 11;
    
    int **reqStatus=reschedule(reqNum,num_requests,st,ed,fNum,0);
    for (i=0;i<11;i++)
        printf("request #%d %d\n", reqStatus[i][0], reqStatus[i][1]);
    
    return 0;
}