#include "util.h"
#include <dirent.h>
extern int system_call(); /*added to fix the warning*/
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define O_RDONLY 0
#define O_WRONLY 1
#define SYS_LSEEK 19
#define SYS_EXIT 1
#define SYS_GETDENTS 141

/*d_types*/
#define DT_REG 8
#define DT_DIR 4
#define DT_FIFO 1
#define DT_SOCK 12
#define DT_LNK 10
#define DT_BLK 6
#define DT_CHR 2

#define BUF_SIZE 8192

void debug_print(char* arg1, char* arg2, char* arg3, char* arg4, int returnValue, int debug);

struct linux_dirent {
    long           d_ino;
    long           d_off;
    unsigned short d_reclen;
    char           d_name[];
};


int main(int argc, char *argv[])
{
    int fd, nread, debug, i, str, close, pre;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    long bpos;
	char *pref, *d_types, *flame;
	char d_type;

    /* reading from argv */
	 for (i = 1; i < argc; i++)
	{
        if ((strcmp(argv[i], "-D")) == 0) debug = 1;
		if ((strncmp(argv[i], "-p", 2)) == 0)
		{
			pre = 1;
			pref = argv[i]+2;
		}
	}	

	 /* starting flame */
	flame = "flame2 is ON!\n";
	str = system_call(SYS_WRITE,STDERR, flame, strlen(flame));
    debug_print("4", "0", flame, "14", str, debug);	

    fd = system_call(SYS_OPEN, ".", O_RDONLY, 0777);
   	debug_print("5", "'.'", "0", "0777", fd, debug);

    if (fd == -1){ /* error */
        system_call(SYS_EXIT, 0x55);}

	nread = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);
	debug_print("141", itoa(fd), "buf", "8192", nread, debug);	

	if (nread == -1){ /* error */
		system_call(SYS_EXIT, 0x55);}

	
	for (bpos = 0; bpos < nread;) 
	{
		d = (struct linux_dirent *) (buf + bpos);
		if (pre == 1)
		{
			if ((strncmp(pref,d->d_name,strlen(pref))==0))
			/* file name start with <prefix> */
			{
				d_type = *(buf + bpos + d->d_reclen -1);
				d_types = (d_type == DT_REG) ?  "regular" :
							(d_type == DT_DIR) ?  "directory" :
							(d_type == DT_FIFO) ? "FIFO" :
							(d_type == DT_SOCK) ? "socket" :
							(d_type == DT_LNK) ?  "symlink" :
							(d_type == DT_BLK) ?  "block dev" :
							(d_type == DT_CHR) ?  "char dev" : "???";
				system_call(SYS_WRITE, STDERR,"name file is: ", 14 );
				system_call(SYS_WRITE, STDERR, d->d_name, strlen(d->d_name) );
				if (debug == 1){
					system_call(SYS_WRITE, STDERR,"      length of file: ", 22);
					system_call(SYS_WRITE,STDERR,itoa(d->d_reclen), sizeof(d->d_reclen) );
				}
				system_call(SYS_WRITE, STDERR,"      file type is: ", 20);
				system_call(SYS_WRITE, STDERR, d_types, strlen(d_types) );
				system_call(SYS_WRITE, STDERR,"\n", 2 );	
			}		
		}
		
		else /* same as 2a */
		{
			system_call(SYS_WRITE,STDERR,"name file is: ", 14 );
			system_call(SYS_WRITE, STDOUT, d->d_name, strlen(d->d_name));
			if (debug==1)
			{							
				system_call(SYS_WRITE, STDERR,"      length of file: ", 22);
				system_call(SYS_WRITE, STDOUT,itoa(d->d_reclen), strlen(itoa(d->d_reclen)));
			}
			system_call(SYS_WRITE,STDOUT,"\n",1);
		}
	
		bpos += d->d_reclen;
	}
	/* closing file */
	close = system_call(SYS_CLOSE, fd);
	debug_print("6", itoa(fd), "0", "0", close, debug);
	return 0;
}



void debug_print(char* arg1, char* arg2, char* arg3, char* arg4, int returnValue, int debug)
{
  if(debug > 0)
  {
    system_call(SYS_WRITE, STDOUT, "system call [arg1, arg2, arg3, arg4, ret code] = ", 49);
    system_call(SYS_WRITE, STDOUT, arg1, strlen(arg1));
    system_call(SYS_WRITE, STDOUT, ", ", 2);
    system_call(SYS_WRITE, STDOUT, arg2, strlen(arg2));
	system_call(SYS_WRITE, STDOUT, ", ", 2);
    system_call(SYS_WRITE, STDOUT, arg3, strlen(arg3));
	system_call(SYS_WRITE, STDOUT, ", ", 2);
    system_call(SYS_WRITE, STDOUT, arg4, strlen(arg4));
	system_call(SYS_WRITE, STDOUT, ", ", 2);
	system_call(SYS_WRITE, STDOUT, itoa(returnValue), strlen(itoa(returnValue)));
    system_call(SYS_WRITE, STDOUT , "\n",1);
  }

}
