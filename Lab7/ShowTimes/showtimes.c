
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <signal.h>
#include <errno.h>


//these 2 need to be finalized yet.
static void DisplayTimeStats(double time, char* outbuf);
static void DoubleToStr(const double dblNum, char* strDblNum);

static void handler(int signo) 
{
	double ticks;
	struct tms tinfo;

	if ( (ticks = (double) sysconf(_SC_CLK_TCK)) == -1 )
	{
		char buf1[] = "Failed to determine clock ticks per second.";
		write(STDOUT_FILENO, buf1, sizeof(buf1));
	}
	else if( times(&tinfo) == (clock_t)-1 )
	{
		char buf2[]="Failed to get times information.";
		write(STDOUT_FILENO, buf2, sizeof(buf2));
	}
	else
	{	

		//TODO: WE ARE SUPPOSED TO DO THIS. BUT I DIDN'T GET THE DOUBLE -> STRING CONVERTER DONE
		//DisplayTimeStats(tinfo.tms_utime/ticks, "  User time: ");
		//DisplayTimeStats(tinfo.tms_stime/ticks, "  System time:   ");
		//DisplayTimeStats(tinfo.tms_cutime/ticks, "  Children's user time:  ");
		//DisplayTimeStats(tinfo.tms_cstime/ticks, "  Children's system time:   ");


		fprintf(stderr, "User time: %8.3f seconds\n", tinfo.tms_utime/ticks);
		fprintf(stderr, "System time: %8.3f seconds\n", tinfo.tms_utime/ticks);
		fprintf(stderr, "Children's user time: %8.3f seconds\n", tinfo.tms_utime/ticks);
		fprintf(stderr, "Children's system time: %8.3f seconds\n", tinfo.tms_utime/ticks);
	}
}

int main(void) {

	pid_t childpid;
	struct sigaction act;    
	act.sa_flags = 0;
	act.sa_handler = handler;

	if ( (sigemptyset(&act.sa_mask) == -1) ||
		(sigaction(SIGUSR1, &act, NULL) == -1) ) {
			perror("Failed to set up SIGUSR1 signal handler.");
			return ;
	}

	int processCount = 0;
	int processMax = 3;
	for (processCount=0;processCount<=processMax;processCount++)
	{
		// waste some time
			int i;
			int k;
			for(i = 0; i < 999999; i++) {
				for(k = 0; k < 99; k++) ;
			}

			if ((childpid = fork()) == -1) 
			{
				fprintf(stderr, "[%ld]:failed to create child %d: %s\n",
					(long)getpid(), i, strerror(errno));
				return 1;
			}
			if ( childpid == 0 ) 
			{   
				//send SIGUSR1 to child 
				fprintf(stderr, "Process %ld stats follow:\n", (long)getpid());
				if ( kill(getpid(), SIGUSR1) == -1 )
				{
					perror("Failed to send the SIGUSR1 signal.");
				}
			}
			else   //send SIGUSR1 to parent
			{
				while ( wait(NULL) > 0 ) ;

				fprintf(stderr, "Child has exited.\n\nParent%ld stats follow:\n", (long)getppid());
				if ( kill(getpid(), SIGUSR1) == -1 ) 
					perror("Failed to send the SIGUSR1 signal");
			}
	}
	return 0;
}

//THE DOUBLETOSTR CONVERSION FUNCTION IS ALL THAT IS LEFT.
static void DisplayTimeStats(double time, char* outbuf)
{
	//write(STDOUT_FILENO, outbuf, sizeof(outbuf));
	//char mytime[5] = {'h','e','l','l','o'};
	////DoubleToStr(time, mytime);
	//write(STDOUT_FILENO, mytime, sizeof(mytime));
}

static void DoubleToStr(const double dblNum, char* strDblNum)
{
	//double dbl = dblNum;
	//bool negflg;
	//// check if it's a negative number and set the flag if so
	//if ( dbl < 0 ) 
	//	dbl *= -1;
	//negflg = true;

	////store integer part in the string:

	//int intNum = (int)dbl;    
	//int intLen = (intNum == 0) ? 1 : IntLen(dbl); // if dbl num is as 0.xxxx

	//inside loop:
	//strDblNum[i] = (char)(intNum % 10 + '0');
	//intNum /= 10;

	//strDblNum[intLen] = '.';

	////store decimal part in the string:    
	//int dcount = intLen + 1;
	//double decNum = dbl - (int)dbl; 

	//Loop  <= EPS; EPS is floating point number precision
	//	decNum *= 10;
	//strDblNum[dcount] = (char)((int)decNum + '0');
	//decNum = decNum - (int)decNum;


	//strDblNum[dcount] = '\0';

	//// handle negative numbers properly
	//if ( negflg )
	//	MakeNegativeStr(strDblNum, dcount);   

}

