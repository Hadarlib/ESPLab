#include "util.h"
extern int system_call(); /*added to fix the warning*/
#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_EXIT 1

#define O_RDONLY 0
#define O_WRONLY 1

#define SYS_LSEEK 19

void debug_print(char* arg1, char* arg2, int arg3, char* arg4, int returnValue, int debug);


int main (int argc , char* argv[], char* envp[])
{
	int debug, word, read, i, write, counter, input, output, close;
	char *enterInput, *inputPath, *outputPath;;
	char c[1];
	
	word = 0;
	debug = 0;
	counter = 0;
	input = STDIN;
	output = STDOUT;
	inputPath = "STDIN";
	outputPath = "STDOUT";

    /* reading from argv */
    for (i = 1; i < argc; i++)
	{
        if ((strcmp(argv[i], "-D")) == 0) debug = 1;

		if ((strncmp(argv[i], "-i", 2)) == 0) 
		{
			input = system_call(SYS_OPEN, argv[i]+2, O_RDONLY, 0777);
			inputPath = argv[i]+2;
			if (input==-1) /* error */
				system_call(SYS_EXIT,0x55);			
		} 
		if ((strncmp(argv[i], "-o", 2)) == 0) 

		{
			output = system_call(SYS_OPEN, argv[i]+2, O_WRONLY, 0777);
			outputPath = argv[i]+2;
			if (output==-1) /* error */
				system_call(SYS_EXIT,0x55);
		}
	}
	
	if (debug == 1)
	{
		/*print open system call*/
		if(input != STDIN)
			debug_print("5", inputPath, 0, "0777", input, debug);
		if(output != STDOUT)
			debug_print("5", outputPath, 1, "0777", output, debug);
		/*print input path*/
		system_call(SYS_WRITE, STDERR, "input path: " ,12);
		system_call(SYS_WRITE, STDERR, inputPath, strlen(inputPath));
		system_call(SYS_WRITE,STDERR,"\n",1);
		/*print output path*/
		system_call(SYS_WRITE, STDERR, "output path: ",13);
		system_call(SYS_WRITE, STDERR, outputPath,strlen(outputPath));
		system_call(SYS_WRITE, STDERR, "\n",1);
	}
	
	if (input == STDIN)
	{
		enterInput = "plese enter your input:\n";
		system_call(SYS_WRITE, STDOUT, enterInput, strlen(enterInput));
	}

    while ((read = system_call(SYS_READ, input, &c, 1)) > 0) 
	{
		if (c[0] == '\n') /* end of input */
        {
			if (word ==1){
				counter++;}
			write = system_call(SYS_WRITE,output, itoa(counter) ,strlen(itoa(counter)));
			system_call(SYS_WRITE, output , "\n",1);
			debug_print("4", itoa(output), counter, itoa(strlen(itoa(counter))), write, debug);
			counter = 0; /* reset counter */
			word = 0; 
        }
		else if(c[0]==' ')
		{
			debug_print("3", itoa(input), c[0], "1", read, debug);
			if(word == 1) /* end of word */
			{
            	counter++;
				word =0;
			}
		}
		else
		{
			debug_print("3", itoa(input), c[0], "1", read, debug);
			word = 1;
		}
	}

	/* closing files if needed */
	if (input != STDIN)
	{
		close = system_call(SYS_CLOSE,input);
		debug_print("6", itoa(input), 0, "0", close, debug);
	}

	if (output != STDOUT)
	{
		close = system_call(SYS_CLOSE,output);
		debug_print("6", itoa(output), 0, "0", close, debug);

	}
  	return 0;
}



void debug_print(char* arg1, char* arg2, int arg3, char* arg4, int returnValue, int debug)
{
  if(debug > 0)
  {
    system_call(SYS_WRITE, STDOUT, "system call [arg1, arg2, arg3, arg4, ret code] = ", 49);
    system_call(SYS_WRITE, STDOUT, arg1, strlen(arg1));
    system_call(SYS_WRITE, STDOUT, ", ", 2);
    system_call(SYS_WRITE, STDOUT, arg2, strlen(arg2));
	system_call(SYS_WRITE, STDOUT, ", ", 2);
    system_call(SYS_WRITE, STDOUT, itoa(arg3), strlen(itoa(arg3)));
	system_call(SYS_WRITE, STDOUT, ", ", 2);
    system_call(SYS_WRITE, STDOUT, arg4, strlen(arg4));
	system_call(SYS_WRITE, STDOUT, ", ", 2);
	system_call(SYS_WRITE, STDOUT, itoa(returnValue), strlen(itoa(returnValue)));
    system_call(SYS_WRITE, STDOUT , "\n",1);
  }

}
