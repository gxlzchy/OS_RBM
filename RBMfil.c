#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define N 20


int* inp(int reqno, char input[], int styear,int stmonth,int stday){
	/**
	 * param[int] firstword to save the place of first word
	 * param[char*] fac for saving the facility user entered
	 * param[char*] dat for the date
	 * param[char*] time for the time
	 * param[char*] dur for the duration
	 * param[char*] caller for the caller
	 */
	printf("styear is %d \n",styear);
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
					int *add,stdat[3];
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
printf("the start day before put the line in file to inp: %d %d %d\n",styear,stmonth,stday);

						add = inp(reqno,line,styear,stmonth,stday);
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
printf("the start day is : %d %d %d\n",stdat[0],stdat[1],stdat[2]);
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
					int stda[3];
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


int main(){
int reqNum[N], st[N], ed[N], fNum[N][5];	// request id, starting time, ending time, number of facilities
	int i, j, k, reqno = 1, commandno;
	int *stdat, *request;	// stdat an array saving the starting time of the whole booking period(2 weeks)
	int* inp();			// initialize the function input and split
	char **split();    
	char all[N][80], **str;	//an array for saving all command for convenience for output
	char input[80],stry[80],stryy[80];
	printf("~~ WELCOME TO PolySME ~~\n");
	
	request=malloc(sizeof(int));
	request[0]=1;
	while(request[0]>0){
		printf("Please enter booking:\n");
		fgets(input,80,stdin);
printf("%c\n",input[3]);		
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
			strcpy(stryy,input);
			stdat=getstdat(stryy);
		}
printf("the start date has been set %d %d %d\n",stdat[0],stdat[1],stdat[2]);		
		request = inp(reqno, input, stdat[0],stdat[1],stdat[2]);
printf("after the inp back to main:%d %d %d %d %d\n",reqNum[reqno-1],st[reqno-1],ed[reqno-1],fNum[reqno-1][0],fNum[reqno-1][1]);
printf("\n");

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
			printf("print the schedule");
		}
		else if (request[0] == -1){
			printf("end of program, thanks for using!");
			break;
		}
		else if (request[0] == -2){	// read input from a batch file
			int numreq = request[1];
			int place = 2;
			strcpy(all[commandno++],input);
			for (i=0;i<numreq;i++){
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
				commandno++;
			}
			
			
		}
printf("%d %d %d %d %d\n",reqNum[reqno-1],st[reqno-1],ed[reqno-1],fNum[reqno-1][0],fNum[reqno-1][1]);
printf("\n");
	}
return 0;
}



	

