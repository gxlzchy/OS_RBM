#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 70    // The maximun acceptable number of request.
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
 * @brief      the function reads a line of input and detects which request has been entered
 *
 * @param[in]  reqno  to let the function know how many request is received now
 * @param      input  for storing the line user entered
 * @param      stdat  { parameter_description }
 *
 * @return     an array for saving input from user, if end program command is received, it will return
 *				an array size one and value is -1, and if a print booking request is received, an array of size 1 and value 0 is returned.
 *				place of array:
 *				0:requestNumber,
 *				1:starttime,
 *				2:endtime,
 *				3:total no. of device in this booking,
 *				4:dev1 number,
 *				5:dev2 number
 *				6:dev3 number (for request addpresent add conference)
 */
int* inp(int reqno, char input[], int *stdat){
	/**
	 * param[int] firstword to save the place of first word
	 * param[char*] fac for saving the facility user entered
	 * param[char*] dat for the date
	 * param[char*] time for the time
	 * param[char*] dur for the duration
	 * param[char*] caller for the caller
	 */
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
	char *ret;
	char stry[80];
	strcpy(stry,input);

	splited = split(stry," "); 
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
printf("the start date passedinto this inp fuction is: %d %d %d \n",stdat[0],stdat[1],stdat[2]);
	
	switch (stry[0]){	/*for the case add booking command is entered*/
		case 'a':	// "add<sth>"
			result[0]=reqno;
printf("second switch");
			switch (splited[0][3]){
				case 'M':	// "addMeeting"
				case 'm':
					result = realloc(result, 5 * sizeof(int));
					result[3]=1;
					switch(splited[1][6]){
						case 'A':	// "room_A"
							result[4] = 1;
							printf("%s\n","abc");
							break;
						case 'B':	// "room_B"
							result[4] = 2;
							break;
					}
					break;
				case 'P':	// "addPresentation"
				case 'p':
				case 'C':	// "addConference"
				case 'c':
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
						if (ret!=NULL){
							result[5] = i+3;
							break;
						}
						
					}
					for(i=0;i<8;i++){
						ret = strstr(splited[7],ckdev[i]);
						if (ret!=NULL){
							result[6] = i+3;
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
							result[4] = i+3;
							break;
						}
					}
					result[3]=1;
					break;
					
				
				case 'b':
				case 'B':	// "addBatch"
				/* for reading a file, it return an array combining all request.
				result : 0: -2(indicator) 
				1:no. of request read 
				for any n elements after , store the n elements (append to the array requet by request)
				*/
					ofl=1;
					FILE *fp;
					char **flname;
					char *line = NULL,*stry;
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
						result = realloc(result,(sizeofarray+5)*sizeof(int));
						sizeofarray+=5;
printf("first hi");
						add = inp(reqno,line,stdat);
printf("hi");
						result[1]++;
						result[placeA++] = reqno++;
						result[placeA++] = add[1];
						result[placeA++] = add[2];
						result[placeA++] = add[3];
						result[placeA++] = add[4];
						if (add[3] == 3){
							result = realloc(result,(sizeofarray + 2)*sizeof(int));
							sizeofarray+=2;
							result[placeA++] = add[5];
							result[placeA++] = add[6];
						}
					}
					printf("the request is addBatch\n");
					break;
			}
			
			break;
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
printf("%d %d %d \n",date[0],date[1],date[2]);
		ti = split(splited[3],":");
		tim = atoi(ti[0]) - 9;
		ddif = datdif(stdat, date);
		result[1] = (ddif*10) + tim;
		du = split(splited[4],".");
		dur = atoi(du[0]);
		result[2] = result[1] + dur -1;
printf("%d %d %d\n",ddif, tim, dur);
		//
	
	}
	printf("%d %d %d %d %d\n",result[0],result[1],result[2],result[3],result[4]);
	return result;
}

/** 
@brief        the function get the date of the first command in a batch file
@param        fil    the file name which is going to be read

@return       am int array contain 3 integer which is the start date
*/
int *getstdat(char fil[]){
					FILE *fp;
					char *line = NULL,**stry,str[80],*strin,**finame,*filen,**a;
					int i;
					char **split();
					size_t len = 0;
					ssize_t read;
					int *stda=malloc(3*sizeof(int));
strcpy(str,fil);
printf("copied\n");
finame = split(str," ");


finame = split(finame[1],"-");
printf("ok!%s\n",finame[0]);
					fp = fopen(finame[0],"r");
printf("opended file\n");
					if (fp == NULL)
						exit(EXIT_FAILURE);
read = getline(&line, &len,fp);
printf("readed a line\n");
printf("%s\n",line);
printf("copied!!!!!!!!!\n");
stry = split(line," ");
			        a = split(stry[2],"-");
			        for(i=0;i<3;i++)
						stda[i] = atoi(a[i]);
free(line);
fclose(fp);
				return stda;
					
}

/**
 * @brief      the funtion takes a string and returns a 2D array which contains the words in the input string splited into the array.
 *
 * @param      str   the string to be splited.
 * @param[in]  s     the split character
 *
 * @return     { description_of_the_return_value }
 */
char **split(char str[],const char s[])
{
	
	char ** res  = NULL;
	char *  p    = strtok (str, s);
	int n_spaces = 0, i;
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


/**
 * @brief      this function calculates the days difference of from start date to the end day and then returns it
 *
 * @param      stdat  starting date for the counting
 * @param      dat    ending date of the counting
 *
 * @return     The difference from start date to the end day
 */
int datdif(int stdat[], int dat[])
{
	int diff;
	int day = dat[2],year = dat[0] ,month = dat[1];
	int stday = stdat[2],styear = stdat[0] ,stmonth = stdat[1];
printf("day = %d\n",day);
printf("stday = %d\n",stday);

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



/**
 * @brief      This function converts an id to a component's name
 *
 * @param[in]  id   1 -- room_A
 *					2 -- room_B
 *					3 -- webcam_720p
 *					4 -- webcam_1080p
 *					5 -- monitor_50
 *					6 -- monitor_75
 *					7 -- projector_fhd
 *					8 -- projector_xga
 *					9 -- screen_100
 *					10 -- screen_150
 *					11 -- tenant_A
 *					12 -- tenant_B
 *					13 -- tenant_C
 *
 * @return     { description_of_the_return_value }
 */
char* id2component(int id)
{
	switch (id)
	{
		case 1	: return "room_A";
		case 2	: return "room_B";
		case 3	: return "webcam_720p";
		case 4	: return "webcam_1080p";
		case 5	: return "monitor_50";
		case 6	: return "monitor_75";
		case 7	: return "projector_fhd";
		case 8	: return "projector_xga";
		case 9	: return "screen_100";
		case 10	: return "screen_150";
		case 11	: return "tenant_A";
		case 12	: return "tenant_B";
		case 13	: return "tenant_C";
	}
}




/**
 * @brief      this function forks child processes to output the schedules to _Schd.dat and Summary.dat
 *
 * @param      reqNum           	reqNum[index]=the request number
 * @param[in]  numOfReq             the number of request needed to be proceeded.
 * @param      st                   st[reqNum]: the start time of the requests.
 * @param      ed                   ed[reqNum]: the end time of the requests.
 *                                  End time is inclusive.
 * @param      fNum                 fNum[reqNum][0]=The total number facilities
 *                                  in this request fNum[reqNum][i]=facility
 *                                  Number.(i=1..3)
 * @param      reqStatus       		{ parameter_description }
 * @param[in]  total_accepted  		{ parameter_description }
 * @param[in]  total_rejected  		{ parameter_description }
 */
void output2dat(int reqNum[], int num_requests, int st[], int ed[],int fNum[][5], int **reqStatus, int total_accepted, int total_rejected)
{
	// ###################################################
	// ################## Output Module ##################
	// ###################################################
	//@booking info: int reqNum[N], st[N], ed[N], fNum[N][5]
	//@return: int **reqStatus
	//	reqStatus[i][0]: reqNum
	//	reqStatus[i][1]: 1 - accepted, 0 - rejected
	
	int i, j, k;
	
	char title1[] = "***Room Booking - ACCEPTED***";
	char title2[] = "***Room Booking - REJECTED***";
	char title3[] = "Performance:";
	
	// output files - FCFS_Schd.dat/PRIO_Schd.dat/OPTI_Schd.dat
	char filename[15] = "fcfs";
	//char filename[15] = "prio";
	//char filename[15] = "opti";
	strcat(filename, "_Schd.dat");
	
	// ###Room Booking - ACCEPTED###
	// open and overwrite the corresponding output file
	FILE *fptr;
	fptr = fopen(filename, "w");
	fprintf(fptr, "%s\n\n", title1);
	fclose(fptr);
	
	// create the pipe
	int fd[10][2];
	for (i = 0; i < 10; i++)
		if (pipe(fd[i]) < 0)
		{
			printf("Pipe creation error\n");
			exit(1);
		}
	
	int fid, childpid, num_bookings;	// facility number
	int utilization[10];
	for (i = 0; i < 10; i++)
		utilization[i] = 0;
	
	for (fid = 1; fid <= 10; fid++)	// for each facility
	{
		num_bookings = 0;
		childpid = fork();
		if (childpid == -1)		// creation error //
			printf("Child process creation error!\n");
		else if (childpid == 0)	// child execution //
		{
			// close the excessive end
			close(fd[fid-1][1]);
			
			// open and append to the corresponding output file
			fptr = fopen(filename, "a");
			fprintf(fptr, "Child %d: [%s] has the following bookings:\n\n", getpid(), id2component(fid));
			fprintf(fptr, "ID   Date         Start   End     Type          Requester  Facility\n");
			fprintf(fptr, "================================================================================\n");
			
			// receive and output the related booking records
			
			int n, rid;	// record id
			char buffer[10];
			read(fd[fid-1][0], buffer, 10);
			n = atoi(buffer);
			int records[n];
			for (i = 0; i < n; i++)
			{
				read(fd[fid-1][0], buffer, 10);
				records[i] = atoi(buffer);
				//date = split(input, " ")[2]
				//requester = split(input, " ")[5]
				char period[2][6];
				sprintf(period[0], "%d:00", st[records[i]] + 9);
				sprintf(period[1], "%d:00", ed[records[i]] + 9);
				fprintf(fptr, "%-5d%-13s%-8s%-8s%-14s%-11s%s\n", records[i], "YYYY-MM-DD", period[0], period[1], "undefined", "undefined", id2component(fNum[records[i]][1]));
				for (k = 2; k <= fNum[records[i]][0]; k++)
					fprintf(fptr, "                                                           %s\n", id2component(fNum[records[i]][k]));
				num_bookings++;
			}
			
			fprintf(fptr, "################################################################################\n");
			if (num_bookings == 0)
				fprintf(fptr, "#                  There is no any booking for this facility.                  #\n");
			else
				fprintf(fptr, "#                There is/are [%2d] booking(s) for this facility.               #\n", num_bookings);
			fprintf(fptr, "################################################################################\n");
			fprintf(fptr, "\n\n\n");
			fclose(fptr);
			
			// close all the ends
			close(fd[fid-1][0]);
			exit(0);
		}
		else	// parent execution //
		{
			// close the excessive end
			close(fd[fid-1][0]);
			
			// distribute the related booking records to children
			int records[N], n;
			for (i = 0; i < num_requests; i++)	// for each scheduled booking
			{
				if (reqStatus[i][1] == 1)	// if the booking is accepted
				{
					// if the accepted booking involves the facility
					for (j = 0; j < fNum[reqStatus[i][0]][0]; j++)
					{
						if (fNum[reqStatus[i][0]][j+1] == fid)
						{
							utilization[fid - 1] += ed[reqStatus[i][0]] - st[reqStatus[i][0]];
							records[num_bookings++] = reqStatus[i][0];
						}
					}
				}
			}
			char buffer[10];
			n = sprintf(buffer, "%d", num_bookings);
			write(fd[fid-1][1], buffer, 10);
			for (i = 0; i < num_bookings; i++)
			{
				n = sprintf(buffer, "%d", records[i]);
				write(fd[fid-1][1], buffer, 10);
			}
			
			// wait for child
			wait(NULL);
			
			// close all the ends
			close(fd[fid-1][1]);
		}
	}
	fptr = fopen(filename, "a");
	fprintf(fptr, "     -End-     \n\n");
	
	// ###Room Booking - REJECTED###
	// open and append to the corresponding output file
	fprintf(fptr, "%s\n\n", title2);
	fclose(fptr);
	
	// create the pipe
	int fd2[10][2];
	for (i = 0; i < 10; i++)
		if (pipe(fd2[i]) < 0)
		{
			printf("Pipe creation error\n");
			exit(1);
		}
	
	int num_rejection;
	
	for (fid = 1; fid <= 10; fid++)	// for each facility
	{
		num_rejection = 0;
		childpid = fork();
		if (childpid == -1)		// creation error //
			printf("Child process creation error!\n");
		else if (childpid == 0)	// child execution //
		{
			// close the excessive end
			close(fd2[fid-1][1]);
			
			// open and append to the corresponding output file
			fptr = fopen(filename, "a");
			fprintf(fptr, "Child %d: [%s] has the following bookings:\n\n", getpid(), id2component(fid));
			fprintf(fptr, "ID   Date         Start   End     Type          Requester  Facility\n");
			fprintf(fptr, "================================================================================\n");
			
			// receive and output the related booking records
			
			int n, rid;	// record id
			char buffer[10];
			read(fd2[fid-1][0], buffer, 10);
			n = atoi(buffer);
			int records[n];
			for (i = 0; i < n; i++)
			{
				read(fd2[fid-1][0], buffer, 10);
				records[i] = atoi(buffer);
				//date = split(input, " ")[2]
				//requester = split(input, " ")[5]
				char period[2][6];
				sprintf(period[0], "%d:00", st[records[i]] + 9);
				sprintf(period[1], "%d:00", ed[records[i]] + 9);
				fprintf(fptr, "%-5d%-13s%-8s%-8s%-14s%-11s%s\n", records[i], "YYYY-MM-DD", period[0], period[1], "undefined", "undefined", id2component(fNum[records[i]][1]));
				for (k = 2; k <= fNum[records[i]][0]; k++)
					fprintf(fptr, "                                                           %s\n", id2component(fNum[records[i]][k]));
				num_rejection++;
			}
			
			fprintf(fptr, "################################################################################\n");
			if (num_rejection == 0)
				fprintf(fptr, "#              There is no any booking rejected for this facility.             #\n");
			else
				fprintf(fptr, "#            There is/are [%2d] booking(s) rejected for this facility.          #\n", num_rejection);
			fprintf(fptr, "################################################################################\n");
			fprintf(fptr, "\n\n\n");
			fclose(fptr);
			
			// close all the ends
			close(fd2[fid-1][0]);
			exit(0);
		}
		else	// parent execution //
		{
			// close the excessive end
			close(fd2[fid-1][0]);
			
			// distribute the related booking records to children
			int records[N], n;
			for (i = 0; i < num_requests; i++)	// for each scheduled booking
			{
				if (reqStatus[i][1] == 0)	// if the booking is rejected
				{
					// if the accepted booking involves the facility
					for (j = 0; j < fNum[reqStatus[i][0]][0]; j++)
					{
						if (fNum[reqStatus[i][0]][j+1] == fid)
						{
							records[num_rejection++] = reqStatus[i][0];
						}
					}
				}
			}
			char buffer[10];
			n = sprintf(buffer, "%d", num_rejection);
			write(fd2[fid-1][1], buffer, 10);
			for (i = 0; i < num_rejection; i++)
			{
				n = sprintf(buffer, "%d", records[i]);
				write(fd2[fid-1][1], buffer, 10);
			}
			
			// wait for child
			wait(NULL);
			
			// close all the ends
			close(fd2[fid-1][1]);
		}
	}
	fptr = fopen(filename, "a");
	fprintf(fptr, "     -End-     \n\n");
	fclose(fptr);
	
	// ###Performance###
	fptr = fopen("Summary.dat", "w");
	fprintf(fptr, "%s\n\n", title3);
	
	int total = total_accepted + total_rejected;
	fprintf(fptr, "Total Number of Bookings Received: [%3d]\n", total);
	fprintf(fptr, "      Number of Bookings Assigned: [%3d] (%.1f%%)\n", total_accepted, (float)total_accepted / total * 100);
	fprintf(fptr, "      Number of Bookings Rejected: [%3d] (%.1f%%)\n", total_rejected, (float)total_rejected / total * 100);
	fprintf(fptr, "\n");
	fprintf(fptr, "Utilization of Time Slot:\n\n");
	for (fid = 1; fid <= 10; fid++)	// for each facility
	{
		fprintf(fptr, "      %-13s - [%.1f%%]\n", id2component(fid), (float)utilization[fid - 1] / M * 100);
	}
	fprintf(fptr, "\n     -End-     \n\n");
	fclose(fptr);
}


/**
 * @brief      Main Program
 */
int main(){

	// ##################################################
	// ################## Input Module ##################
	// ##################################################
	// ***output module: for each request, i also need a variable named "type" (i.e. meeting/presentation/conference/device)
	int reqNum[N], st[N], ed[N], fNum[N][5];	// request id, starting time, ending time, number of facilities
	int i, j, k, reqno = 1, commandno;
	int *stdat, *request;	// stdat an array saving the starting time of the whole booking period(2 weeks)
	int* inp();			// initialize the function input and split
	char **split();    
	char all[N][80], **str,*dup;	//an array for saving all command for convenience for output
	char stry[80];
	printf("~~ WELCOME TO PolySME ~~\n");
	
	request=malloc(sizeof(int));
	request[0]=1;
	while(request[0]>0){
		printf("Please enter booking:\n");
		char input[80];
		fgets(input,80,stdin);
if(reqno>1){ /*check duplicate*/
printf("check duplicat input. commandno=%d\n",commandno);
for(i=0;i<commandno;i++){
	dup = strstr(input,all[i]);
	if(dup!=NULL){
	printf("There is an duplicated input!Please re-enter booking:\n");
	fgets(input,80,stdin);
	i=0;
	}
	}
}
size_t len = strlen(input);
		if (len > 0 && input[len-1] == '\n')
			input[--len] = '\0';
		strcpy(stry,input);
		
		// set the start date for the system
		if (reqno == 1 && input[3]!='B'){	//if this is the first command, take this book's date as the start time of the whole booking period
stdat = malloc(3*sizeof(int));
str = split(stry," ");
			str = split(str[2],"-");
			for(i=0;i<3;i++)
			stdat[i] = atoi(str[i]);
		}
		else if (reqno == 1 && input[3]=='B'){
printf("start getstdat functino\n");
			stdat=getstdat(stry);
		}
printf("the start date has been set %d %d %d\n",stdat[0],stdat[1],stdat[2]);		
		request = inp(reqno, input, stdat);

		if (request[0]>0){
			strcpy(all[commandno++],input);
			reqNum[reqno] = request[0];
			st[reqno] = request[1];
			ed[reqno] = request[2];
			fNum[reqno][0] = request[3];
			fNum[reqno][1] = request[4];
			if (request[3] == 3){
				fNum[reqno][2] = request[5];
				fNum[reqno][3] = request[6];
			}
			reqno++;
			commandno++;
		}
		else if(request[0] == 0)
		{
			// ###################################################
			// ###################################################
			// #################### PrintSchd ####################
			// ###################################################
			// ###################################################
			
			int num_requests = reqno;
			
			int **reqStatus=greedy(reqNum,num_requests,st,ed,fNum);
			printf("greedy finished\n");
			int total_accepted = 0;
			int total_rejected = 0;
			for (i=0;i<11;i++)
			{
				if (reqStatus[i][1] == 1)
					total_accepted++;
				else
					total_rejected++;
				//printf("request #%d %s\n", reqStatus[i][0], reqStatus[i][1]==1?"accept":"reject");
			}
			
			output2dat(reqNum, num_requests, st, ed, fNum, reqStatus, total_accepted, total_rejected);
			
			// ###################################################
			// ###################################################
			// ################# End of PrintSchd ################
			// ###################################################
			// ###################################################
		}
		else if (request[0] == -1){
			printf("end of program, thanks for using!");
			break;
		}
		else if (request[0] == -2){	// read input from a batch file
			int numreq = request[1];
			int place = 2;
			FILE *ff;
			char buf[80],**inputspl,**flename;
			char *lin = NULL, *word;
			size_t leng = 0;
			ssize_t readd;
			strcpy(buf,input);
			inputspl = split(buf," ");
			flename = split(inputspl[1],"-");
			ff = fopen(flename[0],"r");
			strcpy(all[commandno++],input);
			for (i=0;i<numreq;i++){
				getline(&lin,&leng,ff);
printf("line :%s is saving into the array all in place (commandno):%d\n",lin,commandno);
				strcpy(all[commandno++],lin);

				reqNum[reqno] = request[place++];
				st[reqno] = request[place++];
				ed[reqno] = request[place++];
				fNum[reqno][0] = request[place++];
				fNum[reqno][1] = request[place++];
				if (request[3] == 3){
					fNum[reqno][2] = request[place++];
					fNum[reqno][3] = request[place++];
				}
				reqno++;
			}
request[0] = reqno;
			
			
		}
printf("%d %d %d %d %d\n",reqNum[reqno-1],st[reqno-1],ed[reqno-1],fNum[reqno-1][0],fNum[reqno-1][1]);
printf("\n");
//char **info;
//info = getinfo(62,all);
//printf("the getinfo result is:%s %s %s\n",info[0],info[1],info[2]);
	}
	/*
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
	*/
	return 0;
}
