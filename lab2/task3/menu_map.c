#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fun_desc{
	char *name;
	char(*fun)(char);
};

char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  int index = 0;
  while(index< array_length){
  	*(mapped_array + index) = (*f)(*(array + index));
  	index = index + 1;
  }
  return mapped_array;
}

char encrypt(char c){
 if(c>=65 && c<=122)
    return c+2;
  else
    return c; 
}

char decrypt(char c){
 if(c>=65 && c<=122)
    return c-2;
  else
    return c; 
}

char dprt(char c){
	printf("The value of c in decimal is: %d\n",c);
	return c; 
}

char cprt(char c){
	if(c>=65 && c<=122)
    	printf("%c\n" ,c);
	else
		printf("%c\n" , '*');
    return c; 
}

char my_get(char c){
	char ch = fgetc(stdin);
	return ch;
}
char quit(char c){
	if(c == 'q')
		exit(0);
	return c; 
}
char censor(char c) {
  if(c == '!')
    return '*';
  else
    return c;
}


int main(int argc, char **argv){
	FILE* out = stdout;
	FILE* in = stdin;
	char* carray = calloc(5, sizeof(char));
	struct fun_desc menu[] = {{"Censor" ,censor},{"Encrypt" , encrypt},{"Decrypt", decrypt},{"Print dec" , dprt},{"Print string" , cprt},{"Get string", my_get}, {"Quit", quit},{NULL , NULL}};
	int bound = (int)(sizeof(menu))/sizeof(menu[0]); 
	bound = bound - 2; /*because the last null pointer and because the count starts from 0*/
	while(1){
		int i = 0; 
		puts("Please choose a function:");
		while(menu[i].name != NULL){
			fprintf(out, "%d) %s\n" , i , menu[i].name);
			i++;
		}	
		int user;
		scanf("%d", &user);
		fgetc(in);
		if(user > bound || user < 0){
			puts("Not within bounds");
			exit(0);	
		}
		puts("Within bounds");
		carray = map(carray, 5, menu[user].fun);
		puts("Done\n");	
		
	}
	free(carray);
	
	
return 0; 
}

