#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20  //The maximun acceptable number of request.
#define M 100 //The maximun duration of all requests is two weeks time: 2*5(per week)*9(9am-6pm)=90.


/**
	@cmp: compare two variable to prepare for qsort in greedy()
*/
int cmp ( const void *pa, const void *pb ) {
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





/**@param 
		reqNum: int[] reqNum[index]=the request number. 
		numOfReq: int, the number of request needed to be proceeded by FCFS. 
        st: int[]  st[reqNum]: the start time of the requests. 
        ed: int[]. ed[reqNum]: the end time of the requests. 
		End time is inclusive.
        fNum: int[][]:
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
    for (i=0;i<numOfReq;i++){
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

/**@param 
		reqNum: int[] reqNum[index]=the request number. 
		numOfReq: int, the number of request needed to be proceeded by FCFS. 
        st: int[]  st[reqNum]: the start time of the requests. 
        ed: int[]. ed[reqNum]: the end time of the requests. 
		End time is inclusive.
        fNum: int[][]:
         	fNum[reqNum][0]=The total number facilities in this request
         	fNum[reqNum][i]=facility Number.(i=1..3)
   @the function takes in array of requests and schedule it with greedy algorithm
   @return: returns a 2D array. the second dimension has 2 variables, first one is the request number
   second one 1:accepted, 0:rejected.
   The first dimension is just for indexing different requests
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


/** function input
@param: int reqno:to let the function know how many request is recieved now
		char input for storing the line user entered

variables: int firstword to save the place of first word
		   char *fac for saving the facility user entered
		   char *dat for the date 
		   char* time for the time
		   char *dur for the duration
		   char * caller for the caller
	@the function reads a line of input and detect which request has been entered
	@This function return an array for saving input from user ,if end program command is received, it will return
	an array size one and value is -1, and if a print booking request is received, an array of size 1 and value 0 is returned.
	place of array:0:requestNumber,1: starttime,2:endtime,3:total no. of device in this booking,4:dev1 number,5:dev2 number
	
		   
*/
int* input(int reqno, char input[], int stdat[])
{
char **split();
int datdif();
int i,n;/* to store the length of the first word(request)*/
const char space[2]=" ";
const char *a="a";
const char *p="p";
const char *e="e";
const char *b="b";
char **splited;
char *third;
char **dat, **ti, **du;
int date[3], tim, ddif,ofl=0, dur;
int *result=malloc(sizeof(int));
result[0]=-1;

splited = split(input," ");

switch (input[0])
{/* for the case add booking command is entered*/
    case 'a':
	    result[0]=reqno;
	    printf("a request of booking is recieved\n");
	    switch (splited[0][3]){
		    case 'M':
		    case 'm':
			    result[3]=1;
				switch(splited[1][6]){
					case 'A':
						result[4] = 1;
						break;
					case 'B':
						result[4] = 2;
						break;
				}
			    printf("the request is addMeeting\n");
    			break;
		    case 'p':
    		case 'P':
			    n=7;
			    result = realloc(result, 7 * sizeof(int));
				result[3]=3;
				switch(splited[1][6]){
					case 'A':
						result[4] = 1;
						break;
					case 'B':
						result[4] = 2;
						break;
				}
    			printf("the request is addPresentation\n");
    			break;
		    case 'C':
		    case 'c':
			    n=7;
			    result = realloc(result, 7 * sizeof(int));
				result[3]=3;
				switch(splited[1][6]){
					case 'A':
						result[4] = 1;
						break;
					case 'B':
						result[4] = 2;
						break;
				}
			    printf("the request is addConference\n");
    			break;
		    case 'D':
		    case 'd':
			    printf("the request is addDevice\n");
				result[3]=1;
    			break;
		    case 'B':
		    case 'b':
				ofl=1;
			    printf("the request is addBatch\n");
			    break;
		    }
		
	    break;
	
	case 'p':
		/*call output and calaulate*/
		printf("print out booking\n");
		result[0]=0;
		return result;
	case 'e':
		printf("end of program, thanks for using!");
		return result;
    default:
		/*user input error*/
		printf("unknown request");
	}
if(ofl == 0){
	/* find out the start time and end time and save them in array result index 1 and 2*/
	dat = split(splited[2],"-");
	for (i=0;i<3;i++)
	date[i] = atoi(dat[i]);
	ti = split(splited[3],":");
	tim = atoi(ti[0]) - 9;
	ddif = datdif(stdat, date);
	result[1] = (ddif*10) + tim;
	du = split(splited[4],".");
	dur = atoi(du[0]);
	result[2] = result[1] + dur -1;
	//
	


}

	return result;
}

/**@param:char str[]: the string to be splited.

	@the funtion take a string and return a 2 dimentional array which contains the words in the input string splited into the array.
	

*/
char **split(char str[],const char s[]){
	
char ** res  = NULL;
char *  p    = strtok (str, s);
int n_spaces = 0, i;


/* split string and append tokens to 'res' */

while (p) {
  res = realloc (res, sizeof (char*) * ++n_spaces);

  if (res == NULL)
    exit (-1); /* memory allocation failed */

  res[n_spaces-1] = p;

  p = strtok (NULL, s);
}

/* realloc one extra element for the last NULL */

res = realloc (res, sizeof (char*) * (n_spaces+1));
res[n_spaces] = 0;


return res;
}

/**@param: int stdat[]: starting date for the counting
           int dat[] : the end date of the counting
	@ this function calculate the days difference of from start date to the end day and return it
	
*/

int datdif(int stdat[], int dat[]){
	int diff;
	int day = dat[2],year = dat[0] ,month = dat[1];
	int stday = stdat[2],styear = stdat[0] ,stmonth = stdat[1];
	if (month > stmonth){
		if ((stmonth == 4) || (stmonth == 6) || (stmonth == 9) || (stmonth == 11)){
			day += 30;
		}
		else if (stmonth == 2){
			if (styear%4 == 0){
				if (styear%100!=0){
					if(styear%400 == 0){
						day+=29;
					}
					else{
						day+=28;
					}
				}
				else{
					day+=29;
				}
			}
			else{
				day+=28;
			}
		}
		else{
			day+=31;
		}
	}
	diff = day - stday;
	return diff;
}



int main(){
	// 1. input module - input lines in cmd and lines in .dat files
	int reqNum[N],st[N],ed[N],fNum[N][5];
	int i,reqno = 1,commandno;
	int stdat[3] , *request;           // stdat an array saving the starting time of the whole booking period(2 weeks)
	int* input();   	// initialize the function input and split
	char **split();    
	char all[N][50], **str;   //an array for saving all command for convenience for output
	while(request[0]>0){
		printf("please enter booking/request:\n");
		fgets(input,50,stdin);
		strcpy(all[commandno++],input);
		
		/* set the start date for the system */
		if (reqno == 1){       //if this is the first command, take this book's date as the start time of the whole booking period
			str = split(input," ");
			str = split(str[2],"-");
			for(i=0;i<3;i++)
			stdat[i] = atoi(str[i]);
		}
		/*   */
		
		
		request = input(input ,reqno++, stdat);

	}
	
	
	// 2. scheduling module - comment your updated interface if there is any modification

	///*for testing
	reqNum[0]=1;
	st[1]=1;
	ed[1]=5;
	fNum[1][0]=2;fNum[1][1]=1;fNum[1][2]=2;

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

	//*/
	
	// 3. output module - print out the latest schedule and write into .dat files
	// parent and child processes

	int **reqStatus=greedy(reqNum,11,st,ed,fNum);
	for (i=0;i<11;i++)
		printf("request #%d %s\n", reqStatus[i][0],reqStatus[i][1]==1?"accept":"reject");
	
	return 0;
}