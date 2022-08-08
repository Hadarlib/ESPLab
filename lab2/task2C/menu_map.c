#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
	FILE* in = stdin;
	char ch = fgetc(in);
	return ch;
}


char quit(char c){
	if(c == 'q')
		exit(0);
	return c; 
}
