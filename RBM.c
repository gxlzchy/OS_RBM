#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20	// The maximun acceptable number of request.
#define M 100	// The maximun duration of all requests is two weeks time: 2*5(per week)*9(9am-6pm)=90.
/*
Use the following id numbers to represent different components:
0 -- room_A
1 -- room_B
2 -- webcam_720p
3 -- webcam_1080p
4 -- monitor_50
5 -- monitor_75
6 -- projector_fhd
7 -- projector_xga
8 -- screen_100
9 -- screen_150
10 -- tenant_A
11 -- tenant_B
12 -- tenant_C
*/



/**
	@cmp: compare two variables to prepare for qsort in greedy()
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



/**@param 
		reqNum:		int[] reqNum[index]=the request number. 
		numOfReq:	int, the number of request needed to be proceeded by FCFS. 
        st:			int[]  st[reqNum]: the start time of the requests. 
        ed:			int[]. ed[reqNum]: the end time of the requests. 
		End time is inclusive.
        fNum:		int[][]:
         	fNum[reqNum][0]=The total number facilities in this request
         	fNum[reqNum][i]=facility Number.(i=1..3)
   @the function takes in array of requests and schedule it with First Come First Serve algorithm
   @return: returns a 2D array. the second dimension has 2 variables:
		1st one is the request number
		2nd one 1:accepted, 0:rejected.
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
@param: int reqno	- to let the function know how many request is received now
		char input	- for storing the line user entered
		int stdat	- 

variables: int firstword	- to save the place of first word
		   char *fac		- for saving the facility user entered
		   char *dat		- for the date 
		   char *time		- for the time
		   char *dur		- for the duration
		   char *caller		- for the caller
	@the function reads a line of input and detects which request has been entered
	@This function returns an array for saving input from user, if end program command is received, it will return
	an array size one and value is -1, and if a print booking request is received, an array of size 1 and value 0 is returned.
	place of array:
	0:requestNumber,
	1:starttime,
	2:endtime,
	3:total no. of device in this booking,
	4:dev1 number,
	5:dev2 number
<<<<<<< HEAD
	6:dev3 number (for request addpresent add conference)
	
		   
=======
>>>>>>> origin/master
*/
int* input(int reqno, char input[], int stdat[])
{
	char **split();
	int datdif();
	int i,n=5;/* i to store the length of the first word(request)*/
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
	const char ckdev[8][15]={ "webcam_720p","webcam_1080p","monitor_50","monitor_75","projector_fhd","projector_xga","screen_100","screen_150" };
	
	splited = split(input," ");  
	/*
	split the input by space, return as a 2D array, 
	0:request 
	1:room (-aaa) 
	2:date (YYYY-MM-DD)
	3:time (hh:mm)
	4:duration (n.n)
	5:caller (bbb)
	6:1st device (ccc)
	7:2nd device (ddd)
	*/
	
	switch (input[0])
	{	/*for the case add booking command is entered*/
		case 'a':	// "add<sth>"
			result[0]=reqno;
			printf("a request of booking is received\n");
			switch (splited[0][3])
			{
				case 'M':	// "addMeeting"
				case 'm':
					result = realloc(result, 5 * sizeof(int));
					result[3]=1;
					switch(splited[1][6])
					{
						case 'A':	// "room_A"
							result[4] = 1;
							break;
						case 'B':	// "room_B"
							result[4] = 2;
							break;
					}
					
					break;
					
<<<<<<< HEAD
		        }
				

    			break;
		    case 'D':
		    case 'd':
				result = realloc(result, 5 * sizeof(int));
				for(i=0;i<8;i++){
		            ret = strstr(splited[1],ckdev[i]);
		            if (ret){
		                result[4] = i+2;
		                break;
		            }
		        }
				result[3]=1;
    			break;
				
		    case 'B':
		    case 'b':
			/* for reading a file, it return an array combining all request.
			 result : 0: -2(indicator) 
			 1:no. of request read 
			 for any n elements after , store the n elements (append to the array requet by request)
			 */
				ofl=1;
				FILE *fp;
				char **flname;
				char *line = NULL;
				int placeA=2,sizeofarray;
				size_t len = 0;
				ssize_t read;
				result[0] = -2;
				int *add;
				result[1] = 0;
				flname = split(splited[1],"-");
				result = realloc(result,7*sizeof(int));
				fp = fopen(flname[0],"r");
				if (fp == NULL)
					exit(EXIT_FAILURE);
				while ((read = getline(&line, &len,fp)) != -1){
					if (result[1]>=1)
					result = realloc((sizeofarray+5)*sizeof(int));
					sizeofarray+=5;
					add = input(reqno,line,stdat);
					result[1]++;
					result[placeA++] = reqno++;
					result[placeA++] = add[1];
					result[placeA++] = add[2];
					result[placeA++] = add[3];
					result[placeA++] = add[4];
					if (add[3] == 3){
						result = realloc((sizeofarray + 2)*sizeof(int));
						sizeofarray+=2;
						result[placeA++] = add[5];
						result[placeA++] = add[6];
					}
				}
			    printf("the request is addBatch\n");
			    break;
		    }
=======
				case 'p':
				case 'P':	// "addPresentation"
				case 'c':
				case 'C':	// "addConference"
					n=7;
					result = realloc(result, 7 * sizeof(int));
					result[3]=3;
					switch(splited[1][6])
					{
						case 'A':
							result[4] = 1;
							break;
						case 'B':
							result[4] = 2;
							break;
					}
					for(i=0;i<8;i++){
						ret = strstr(splited[6],ckdev[i]);
						if (ret){
							result[5] = i+2;
							break;
						}
						
					}
					for(i=0;i<8;i++){
						ret = strstr(splited[7],ckdev[i]);
						if (ret){
							result[6] = i+2;
							break;
						}
						
					}
					
					break;
				case 'D':	// "addDevice"
				case 'd':
					result = realloc(result, 5 * sizeof(int));
					for(i=0;i<8;i++){
						ret = strstr(splited[1],ckdev[i]);
						if (ret){
							result[4] = i+2;
							break;
						}
					}
					result[3]=1;
					break;
					
				case 'B':	// "addBatch"
				case 'b':
					ofl=1;
					FILE *fp;
					char **flname;
					char *line = NULL;
					size_t len = 0;
					ssize_t read;
					
					flname = split(splited[1],"-");
					fp = fopen(flname[0],"r");
					if (fp == NULL)
						exit(EXIT_FAILURE);
					while ((read = getline(&line, &len,fp)) != -1){
						result = input(reqno,line,stdat);
					}
					printf("the request is addBatch\n");
					break;
			}
			
			break;
>>>>>>> origin/master
		
		case 'p':
			/*call output and calculate*/
			printf("print out booking\n");
			result[0]=0;
			return result;
		case 'e':
			return result;
		default:
			/*user input error*/
			printf("unknown request");
	}
	
	if(ofl == 0){
		/*find out the start time and end time and save them in array result index 1 and 2*/
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

	@the funtion takes a string and returns a 2D array which contains the words in the input string splited into the array.
	

*/
char **split(char str[],const char s[])
{
	
	char ** res  = NULL;
	char *  p    = strtok (str, s);
	int n_spaces = 0, i;
	
	/* split string and append tokens to 'res' */
	while (p)
	{
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



/**@param: int stdat[]	: starting date for the counting
           int dat[]	: ending date of the counting
   @ this function calculates the days difference of from start date to the end day and then returns it
	
*/
int datdif(int stdat[], int dat[])
{
	int diff;
	int day = dat[2],year = dat[0] ,month = dat[1];
	int stday = stdat[2],styear = stdat[0] ,stmonth = stdat[1];
	int *request;
	if (month > stmonth){  /*if the period is across two months*/
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
	if (year > styear){/* if the period across two year*/
		dat+=31;
		
	}
	
	
	diff = day - stday;
	return diff;
}



/* this function converts an id --> a component name
0 -- room_A
1 -- room_B
2 -- webcam_720p
3 -- webcam_1080p
4 -- monitor_50
5 -- monitor_75
6 -- projector_fhd
7 -- projector_xga
8 -- screen_100
9 -- screen_150
10 -- tenant_A
11 -- tenant_B
12 -- tenant_C
*/
char* id2component(int id)
{
	switch (id)
	{
		case 0	: return "room_A";
		case 1	: return "room_B";
		case 2	: return "webcam_720p";
		case 3	: return "webcam_1080p";
		case 4	: return "monitor_50";
		case 5	: return "monitor_75";
		case 6	: return "projector_fhd";
		case 7	: return "projector_xga";
		case 8	: return "screen_100";
		case 9	: return "screen_150";
		case 10	: return "tenant_A";
		case 11	: return "tenant_B";
		case 12	: return "tenant_C";
	}
}



/* main program */
int main(){
<<<<<<< HEAD
	// 1. input module - input lines in cmd and lines in .dat files
	int reqNum[N],st[N],ed[N],fNum[N][5];
	int i,reqno = 0,commandno;
	int stdat[3] , *request;           // stdat an array saving the starting time of the whole booking period(2 weeks)
	int* input();   	// initialize the function input and split
=======
	// ##################################################
	// ################## Input Module ##################
	// ##################################################
	// ***output module: for each request, i also need a variable named "type" (i.e. meeting/presentation/conference/device)
	int reqNum[N], st[N], ed[N], fNum[N][5];	// request id, starting time, ending time, number of facilities
	int i, reqno = 1, commandno;
	int stdat[3], *request;	// stdat an array saving the starting time of the whole booking period(2 weeks)
	int* input();			// initialize the function input and split
>>>>>>> origin/master
	char **split();    
	char all[N][80], **str;	//an array for saving all command for convenience for output
	printf("~~ WELCOME TO PolySME ~~\n");
	
	while(request[0]>0){
		printf("Please enter booking:\n");
		fgets(input,80,stdin);
		
		printf("-> [Pending]\n");
		
		/* set the start date for the system */
		if (reqno == 1){	//if this is the first command, take this book's date as the start time of the whole booking period
			str = split(input," ");
			str = split(str[2],"-");
			for(i=0;i<3;i++)
			stdat[i] = atoi(str[i]);
		}
		
		request = input(input ,reqno, stdat);
		if (request[0]>0){
			strcpy(all[commandno++],input);
			reqNum[reqno] = request[0];
			st[reqno] = request[1];
			ed[reqno] = request[2];
			fNum[reqno][0] = request[3];
			fNum[reqno][1] = request[4];
			if (result[3] == 3){
				fNum[reqno][2] = request[5];
				fNum[reqno][3] = request[6];
			}
			reqno++;
			commandno++;
		}
		/*else if(request[0] == 0){  for calculate schdule and print it out
			printf()
		}*/
		else if (request[0] == -1){
			printf("end of program, thanks for using!");
			break;
		}
		else if (request[0] == -2){ /* read input from a batch file */
			int numreq = request[1];
			int place = 2;
			strcpy(all[commandno++],input);
			for (i=0;i<numreq;i++){
				reqNum[reqno] = request[place++];
				st[reqno] = request[place++];
				ed[reqno] = request[place++];
				fNum[reqno][0] = request[place++];
				fNum[reqno][1] = request[place++];
				if (result[3] == 3){
					fNum[reqno][2] = request[place++];
					fNum[reqno][3] = request[place++];
				}
				reqno++;
				commandno++;
			}
			request[0] =0;
			
			
		}

	}
	
	// ###################################################
	// ################ Scheduling Module ################
	// ###################################################
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
	
	int **reqStatus=greedy(reqNum,11,st,ed,fNum);
	for (i=0;i<11;i++)
		printf("request #%d %s\n", reqStatus[i][0],reqStatus[i][1]==1?"accept":"reject");
	
	// ###################################################
	// ################## Output Module ##################
	// ###################################################
	
	char title1[] = "***Room Booking ¨C ACCEPTED***";
	char title2[] = "***Room Booking - REJECTED***";
	char title3[] = "Performance:";
	
	// output files - FCFS_Schd.dat/PRIO_Schd.dat/OPTI_Schd.dat
	char filename[15] = "fcfs";
	//char filename[15] = "prio";
	//char filename[15] = "opti";
	strcat(filename, "_Schd.dat");
	
	// open and overwrite the corresponding output file
	FILE *fptr;
	fptr = fopen(filename, "w");
	fprintf(fptr, "%s\n\n", title1);
	fclose(fptr);
	
	// create the two necessary pipes - p2c and c2p
	int p2c[2], c2p[2];
	if (pipe(p2c) < 0)
	{
		printf("Pipe creation error\n");
		exit(1);
	}
	if (pipe(c2p) < 0)
	{
		printf("Pipe creation error\n");
		exit(1);
	}
	
	// fork() 13 children - 10 facilities and 3 tenants
	int id = -1;
	for (i = 0; i < 13; i++)
		if ((childpid = fork()) <= 0)
		{
			id = i;
			break;
		}
	// fprintf(stderr, "This is process %ld with parent %ld\n", (long)getpid(), (long)getppid());
	// printf("childpid: %d\n", childpid);
	
	if (childpid == -1)		// creation error //
		printf("Child process creation error!\n");
	else if (childpid > 0)	// parent execution //
	{
		// close the excessive ends
		close(p2c[0]);
		close(c2p[1]);
		
		// distribute the related booking records to children
		char records_in_char[10];
		int records;
		// printf("Child: my id - %d size - %d\n", buffer, sizeof(buffer));
		sprintf(records_in_char, "%d", records);
		write(c2p[1], records_in_char, 10);
		
		// wait for child
		wait(NULL);
		
		// close all the ends
		close(p2c[1]);
		close(c2p[0]);
		exit(0);
	}
	else	// child execution //
	{
		// close the excessive ends
		close(p2c[1]);
		close(c2p[0]);
		
		// open and append to the corresponding output file
		fptr = fopen(filename, "a");
		fprintf(fptr, "%shas the following bookings:\n\n", id2component(id));
		fprintf(fptr, "Date         Start   End     Type          Requester  Device\n", NULL);
		fprintf(fptr, "===========================================================================\n", NULL);
		
		// receive and output the related booking records
		long records[N][10];
		int n;
		for (i = 0; i < N; i++)
		{
			while ((n = read(c2p[0], records[i], 10)) > 0)
			{
				records[i][n] = 0;
				fprintf(fptr, "%13s%8s%8s%14s%11s%s\n", records[i]);
			}
		}
		printf("\n");
		fclose(fptr);
		
		// close all the ends
		close(p2c[0]);
		close(c2p[1]);
		exit(0);
	}
	
	return 0;
}