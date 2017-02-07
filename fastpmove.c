
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
#include "../commondef.h"
#include "../modpub.h"

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
    printf("faspmove  ch=%d, step=%d  \n", ch, fl);

    a = 6;			 
    sprintf(chid, "/dev1/dmc1001_%d", ch);
    printf("ch=%d, %s\n", ch, chid);
 
    fd1 = open(chid, 2);

    p.lParm1 = fl;
    ioctl(fd1, M_FAST_PMOVE, &p);

    done = 0;
    while (done == 0) {
	a = ioctl(fd1, M_CHECKDONE, &p);
	done = p.bParm1;
	 
	usleep(1000);
    }

    close(fd1);
}
