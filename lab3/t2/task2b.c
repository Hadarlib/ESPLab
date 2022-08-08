#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};


void PrintHex(unsigned char buffer[], int length, FILE* output){
	unsigned char ch; 
	int i = 0;
	while(i < length){
		ch = buffer[i];
		fprintf(output, "%02X ", ch);
		i++;
	}
	fprintf(output,"%c", '\n');	
}

void readVirus(virus* vir, FILE* input){
	char size [2]; 
	if(fread(size, sizeof(char) , 2, input) <= 0){
		free(vir);
		return;
	}
	short number = (size[1] << 8) + size[0];
	vir->SigSize = number;
	vir->sig = malloc(sizeof(char)*number);
	if(fread(vir->sig, sizeof(char) , number, input) <= 0){
		free(vir->sig);
		free(vir);
		return;
	}
	if(fread(vir->virusName, sizeof(char) , 16, input) <= 0){
		free(vir->sig);
		free(vir);
		return;
	}		
}

void printVirus(virus* vir, FILE* output){
	fprintf(output,"%s %s\n", "Virus name:", vir->virusName);
	fprintf(output,"%s %d\n", "Virus size:", vir->SigSize);
	fprintf(output,"%s\n", "signature:");
	PrintHex(vir->sig, vir->SigSize, output);
	fprintf(output,"%c", '\n');	
}

void list_print(link virus_list, FILE output){
	if(virus_list == NULL)
		return;
	printVirus(virus_list->vir, output);
	list_print(virus_list->nextVirus, output);
} 

 
link* list_append(link* virus_list, link* to_add){
	if(to_add->vir->SigSize == 0){
		free(to_add);
		return virus_list;
	}
	if(virus_list == NULL)
		return to_add;
	to_add->nextVirus = virus_list;
	virus_list = to_add;
	return virus_list; 
} 
     
void list_free(link *virus_list){
	if(virus_list == NULL)
		return;
	free(virus_list->vir->sig);
	free(virus_list->vir);
	list_free(virus_list->nextVirus);
	free(virus_list);
}
void detect_virus(char *buffer, unsigned int size, link *virus_list){
	if(virus_list != NULL){
		int sigSize = virus_list->vir->SigSize;
		unsigned char signature[sigSize];
		memcpy(signature, virus_list->vir->sig, sigSize);
		unsigned int i = 0; 
		while(i<size){
			unsigned char cut[sigSize];
			memcpy(cut, buffer+i, sigSize);
			int ret = memcmp(cut,signature, sigSize);
			if(ret==0){
				fprintf(stdout, "%s\n", "Virus detected");
				fprintf(stdout, "%s %d\n" , "Byte location:", i);
				fprintf(stdout, "%s %s\n" , "Virus name:", virus_list->vir->virusName);
				fprintf(stdout, "%s %d\n" , "Virus size:", sigSize);
			}
			i++;
		}
		detect_virus(buffer, size, virus_list->nextVirus);
	}
}

void kill_virus(char *fileName, int signatureOffset, int signatureSize){
	FILE* file = fopen(fileName, "rb+");
	fseek(file, signatureOffset, SEEK_SET);
	char nop[]= {0x90};
	unsigned int i =0;
	while (i<=signatureSize){
		fwrite(nop, 1, 1, file);
		i++;
	}
	fclose(file);
}

int main(int argc, char **argv){
	FILE* output = stdout;
	FILE* input;
	FILE* input2;
	if(output == NULL){
		perror("fopen");
		exit(0);
	}
	char menu[6][40] = {"Load signatures", "Print signatures" , "Detect viruses", "Fix file", "Quit"};
	link* virus_list = NULL;
	while(1){
		int i = 0; 
		puts("Please choose a function:");
		while(i<5){
			fprintf(stdout, "%d) %s\n" , i+1 , menu[i]);
			i++;
		}	
		int user;
		scanf("%d", &user);
		fgetc(stdin);
		if(user==1){
			puts("Please enter file name:");
			char doc[100]; 
			char doc1[100]; 
			fgets(doc, 100, stdin);
			sscanf(doc, "%s\n", doc1);
			input = fopen(doc1, "r"); 
			if(input == NULL){
				perror("fopen");
				exit(0);
			}
			while(!feof(input)){
				virus *vir = malloc(sizeof(virus));
				readVirus(vir, input);
				link *to_add = malloc(sizeof(link));
				to_add->vir = vir;
				to_add->nextVirus = NULL;
				virus_list = list_append(virus_list,to_add); 
			} 	
		}
		else if(user==2){
			list_print(virus_list, output);
		}
		else if(user==3){
			puts("Please enter file name:");
			char doc[100]; 
			char doc1[100]; 
			fgets(doc, 100, stdin);
			sscanf(doc, "%s\n", doc1);
			input2 = fopen(doc1, "r");
			char buffer[10000];
			int i = 0; 
			while(fgetc(input2) != EOF)
				i++;
			fseek(input2, 0, SEEK_SET);
			if (i>10000)
				i=10000;
			fread(buffer, 1 ,i, input2);
			detect_virus(buffer,i, virus_list);
			fclose(input2);
		}
		else if(user==4){
			puts("Please enter file name:");
			char doc[100]; 
			char doc1[100]; 
			fgets(doc, 100, stdin);
			sscanf(doc, "%s\n", doc1);
			int offset;
			puts("Please enter signature offset:");
			scanf("%d", &offset);
			int size;
			puts("Please enter signature size:");
			scanf("%d", &size);
			kill_virus(doc1,offset,size);
		}
		else{
			list_free(virus_list);
			fclose(input);
			fclose(output);
			exit(0);
		}	
		
	}
	return 1; 
}
