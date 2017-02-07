
#include <stdio.h>
#include <assert.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
//#include "../commondef.h"
//#include "../modpub.h"
#include <linux/ioctl.h>


typedef enum
{
  AUTO_RAMPDOWN = 0,
  ABS_RAMPDOWN,
  REL_RAMPDOWN
}
RDMODE;

typedef struct
{
  RDMODE emRampDownMode;
  long lRampDownSteps;		
}
EXTENDOPTION;


typedef struct IOCTL_PARM{
	long lParm1;
	long lParm2;
	long lParm3;
	long lParm4;
	double lfParm1;
	double lfParm2;
	double lfParm3;
	int bParm1;
	int bParm2;
	EXTENDOPTION *pEO;
}IOCTL_PARM;

/* Use 'k' as magic number */
#define M_IOC_MAGIC  'k'
#define M_FAST_PMOVE         _IO(M_IOC_MAGIC, 19)
#define M_CHECKDONE          _IO(M_IOC_MAGIC, 11)

IOCTL_PARM p, parm;
int fd;
int fd1;
int fd2;

main(int argc, char *argv[])
{
    int a;
    int b, c;
    int ch, fl, fh, ad, dd;
    char chid[20];
    int done = 0;
    if (argc != 3) {
	printf("usage fastpmove ch step\n");
	printf("ex: fastpmove 1 100 \n");
	exit(0);
    }

    ch = atoi(argv[1]);
    fl = atoi(argv[2]);
    printf("fp3 ch=%d, step=%d  \n", ch, fl);

    a = 6;			//spank

    sprintf(chid, "/dev1/dmc1001_%d", ch);
    printf("ch=%d, %s\n", ch, chid);
//fd1=open("/dev1/dmc1001_6",2); //y
    fd1 = open(chid, 2);

    p.lParm1 = fl;
    ioctl(fd1, M_FAST_PMOVE, &p);

    done = 0;
    c=0;
    while (done == 0) {
	a = ioctl(fd1, M_CHECKDONE, &p);
	done = p.bParm1;
	printf("done=%d c=%d\n",done,c++);
	usleep(1000);
	if (c>4000) done=1;
    }

    close(fd1);
}
