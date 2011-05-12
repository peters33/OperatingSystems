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
static int IntLen(int);

static void handler(int signo)
{
	double ticks;
	struct tms tinfo;

	if ( (ticks = (double) sysconf(_SC_CLK_TCK)) == -1 )
	{
		char buf1[] = "Failed to determine clock ticks per second.";
		write(STDOUT_FILENO, buf1, strlen(buf1));
	}
	else if( times(&tinfo) == (clock_t)-1 )
	{
		char buf2[]="Failed to get times information.";
		write(STDOUT_FILENO, buf2, strlen(buf2));
	}
	else
	{
		DisplayTimeStats(tinfo.tms_cutime/ticks, "\n  Process user time:  ");
		DisplayTimeStats(tinfo.tms_cstime/ticks, "\n  Process system time:   ");
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

	for (processCount=0;processCount<processMax;processCount++)
	{
		if ((childpid = fork()) == -1)
		{
			fprintf(stderr, "[%ld]:failed to create CopyDirectory1 child: %s\n",(long)getpid(), strerror(errno));
			return 1;
		}
		if ( childpid == 0 )
		{
			if (processCount == 0)
			{
				int t = 0;
				for (t=0;t<20;t++)
				{
					system("./copy1 root_dir copydir1");
				}

				fprintf(stderr, "\n\n  CopyDirectory1 process %ld stats follow:", (long)getpid());
				if ( kill(getpid(), SIGUSR1) == -1 )
				{
					perror("Failed to send the SIGUSR1 signal.");
				}
			}
			if (processCount == 1)
			{
				int t = 0;
				for (t=0;t<20;t++)
				{
					system("./copy2 root_dir copydir2");
				}

				fprintf(stderr, "\n\n  CopyDirectory2 process %ld stats follow:", (long)getpid());
				if ( kill(getpid(), SIGUSR1) == -1 )
				{
					perror("Failed to send the SIGUSR1 signal.");
				}
			}
			if (processCount == 2)
			{
				int t = 0;
				for (t=0;t<20;t++)
				{
					system("./copy3 root_dir copydir3");
				}

				fprintf(stderr, "\n\n  CopyDirectory3 process %ld stats follow:", (long)getpid());
				if ( kill(getpid(), SIGUSR1) == -1 )
				{
					perror("Failed to send the SIGUSR1 signal.");
				}
			}
			break;
		}
		else 
		{
			while ( wait(NULL) > 0 ) ;
			fprintf(stderr, "\n\n  Child has exited. Parent %ld stats follow:\n", (long)getppid());
			if ( kill(getpid(), SIGUSR1) == -1 ) 
				perror("Failed to send the SIGUSR1 signal");
		}
	}
	return 0;
}

//THE DOUBLETOSTR CONVERSION FUNCTION IS ALL THAT IS LEFT.
static void DisplayTimeStats(double time, char* outbuf)
{
	write(STDOUT_FILENO, outbuf, strlen(outbuf));
	char mytime[25];
	DoubleToStr(time, mytime);
	write(STDOUT_FILENO, mytime, strlen(mytime));
}

static void DoubleToStr(const double dblNum, char* strDblNum)
{
	double dbl = dblNum;
	double decNum;
	int signFlag = 0;
	int intVal = 0;
	int intLen = 0;
	int i;
	int dCount;

	// check for negative number
	if (dbl < 0) {
		dbl = -1 * dbl;
	}

	// get whole number
	intVal = (int)dbl;
	intLen = (intVal == 0 ? 1 : IntLen(intVal));

	for (i = intLen - 1; i >= 0; i--) {
		strDblNum[i] = (char)(intVal % 10 + '0');
		intVal = intVal / 10;
	}

	strDblNum[intLen] = '.';

	dCount = intLen + 1;
	decNum = dbl - (int) dbl;

	while (dCount < 16) {
		decNum = decNum * 10;
		strDblNum[dCount] = (char)((int)decNum + '0');
		decNum = decNum - (int)decNum;
		dCount++;
	}

	strDblNum[dCount] = '\0';
}

static int IntLen(int i) {

	int count = 0;
	while (i > 0) {
		count++;
		i = i / 10;
	}
	return count;
}
