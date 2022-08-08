#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int charToNumber(char numberChar);

/*task 1d*/ 

int main(int argc, char **argv) {
    	FILE* out = stdout;
	FILE* in = stdin;
	char ch;
	int counter = 0; 
	char d = '.';
	int debug = 0; 
	int plus = 0;
	int minus = 0; 
	int numberE; 
	int i = 1; 
	int found = 0; 
	int file = 0; 
	while(i < argc){
		if(strncmp(argv[i], "-i",2)==0){
			file = 1; 
			in = fopen(argv[i]+2,"r");
		}
		if(strcmp(argv[i], "-D")==0)
			debug = 1; 
		if(strncmp(argv[i], "+e",2)==0){
			plus = 1;
			found = 1;  
		}
		if(strncmp(argv[i], "-e" , 2)==0){
			minus = 1; 
			found = 1; 
		}
		if((plus==1 || minus==1)&& found==1){
			char numberChar = argv[i][2];
			numberE = charToNumber(numberChar);
			if(numberE == -1)
				return 1; 
			found = 0; 
		}
		i = i + 1; 
	}
	int numbertmp = numberE;
	int first = 0; 
	char firstChar;  
 	while ((ch = fgetc(in)) != EOF){
		if(plus==1){
			if(first==0){
				firstChar = ch;
				first = 1;
			}
			if(ch == '\n'){
				while(numberE > 0){
					if(debug==1){
						fprintf(stderr,"%d", 0); 
						fprintf(stderr,"%s", " "); 
						fprintf(stderr,"%d\n", firstChar);        
					}
					fprintf(out,"%c", firstChar); 
					numberE = numberE - 1; 
				}
			numberE = numbertmp; 
			first = 0; 
			}
			if(debug==1 && ch!= '\n'){
				fprintf(stderr,"%d", ch); 
				fprintf(stderr,"%s", " "); 
				fprintf(stderr,"%d\n", ch);        
			}
			fprintf(out,"%c", ch); 
		}

		else if(minus==1){
			if(ch == '\n'){
				fprintf(out,"%c", ch);
				counter = 0 ;  
			}
			else if(counter < numberE){
				if(debug==1){
					fprintf(stderr,"%d", ch); 
					fprintf(stderr,"%s", " "); 
					fprintf(stderr,"%d\n", 0);
				}
				counter++; 	
			}
			else if(counter >= numberE){
				fprintf(out,"%c", ch);
				if(debug==1){
					fprintf(stderr,"%d", ch); 
					fprintf(stderr,"%s", " "); 
					fprintf(stderr,"%d\n", ch);
				}
			} 
		}
		else{
			if (ch >= 'A' && ch<='Z'){
				if(debug==1){
			        fprintf(stderr,"%d", ch);
		 			fprintf(stderr,"%s", " ");  
					fprintf(stderr,"%d\n", d);
					counter = counter + 1;
				}
				fprintf(out,"%s",".");	
		  	}
			else if(ch != '\n'){
				if(debug==1){
					fprintf(stderr,"%d", ch); 
					fprintf(stderr,"%s", " "); 
					fprintf(stderr,"%d\n", ch);        
				}
				fprintf(out,"%c", ch); 
		   	}
			if(ch == '\n'){
				if(debug==1){
					fprintf(stderr, "The number of letters: %d\n", counter);
					counter = 0 ;
				}
				fprintf(out, "%s\n", " ");
			}
		}
			
	}


		
        
	
if(file==1)
	fclose(in);
return 0;

}

int charToNumber(char numberChar){
	int number; 
	if(numberChar >='A' && numberChar<='F') 
		number = numberChar - 'A' + 10; 
	else if(numberChar >='0' && numberChar <='9')
		number = numberChar - '0'; 
	else{
		puts("Number is out of range");
		return -1; 
	}
	return number; 

}
