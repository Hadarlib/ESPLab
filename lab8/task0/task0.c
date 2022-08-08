#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <elf.h>

struct fun_desc{
	char *name;
	void(*fun)();
};

//Elf32_Ehdr *header;
void* map_start;
int Currentfd = -1;
off_t fdSize;
int debugFlag = 0;

void examine()
{
    char fileName[1000];
    puts("hey doron, please enter file name:");
    scanf("%s", fileName);

    if (Currentfd > -1) // close any open file 
    {
        close(Currentfd);
    }

    if(map_start) // munmaup
    {
        munmap(map_start, fdSize);
    }

    Currentfd = open(fileName, O_RDONLY);
    if (Currentfd < 0) // file cannot be opened
    {
        printf("open failed");
        return;
    }

    fdSize = lseek(Currentfd, 0, SEEK_END);
    lseek(Currentfd, 0, SEEK_SET);
    map_start = mmap(0, fdSize, PROT_READ, MAP_SHARED, Currentfd, 0);

    if (map_start == MAP_FAILED || map_start == -1)
    {
        printf("mmap failed");
        close(Currentfd);
        Currentfd = -1;
        return;
    }

    Elf32_Ehdr* header = (Elf32_Ehdr*) map_start;

    if (fdSize < 5 || !(header->e_ident[0] == 0x7f && header->e_ident[1] == 'E' && header->e_ident[2] == 'L' && header->e_ident[3] == 'F'))
    {
        printf("File is not in Elf format");
        close(Currentfd);
        Currentfd = -1;
        munmap(map_start, fdSize);
        map_start = NULL;
        return;
    }

    // ----------------------------------------
    printf("1) Bytes 1,2,3 of the magic number (in ASCII): ");
    for (int i = 1; i < 4; i++)
        printf("%c ", header->e_ident[i]);
    printf("\n");
    // ----------------------------------------
    if(header->e_ident[EI_DATA] == 1)
        printf("2) Data encoded scheme of object file: 2's complement, little endian\n");
    else
        printf("2) Data encoded scheme of object file: 2's complement, big endian\n");
    // ----------------------------------------
    printf("3) Entry point (hexadecimal address): ");
    printf("%x\n", (header->e_entry));
    // ----------------------------------------
    printf("4) The file offset in which the section header table resides: ");
    printf("%d\n", (header->e_shoff));
    // ----------------------------------------
    printf("5) The number of section header entries: ");
    printf("%d\n", (header->e_shnum));
    // ----------------------------------------
    int sectionsNum = header->e_shnum;
    Elf32_Shdr* section = (Elf32_Shdr*)(map_start + header->e_shoff);
    // Elf32_Shdr* sh_strtab = &section[map_start->e_shstrndx];
    // const char* const names = map_start+sh_strtab->sh_offset;
    printf("6) The size of each section header entry: \n");
    for (int i=0; i<sectionsNum; i++)
    {
        printf("section number: %d Size: %d\n" , i, section->sh_size);
    	section++;
    }
    // ----------------------------------------
    printf("7) The file offset in which the program header table resides: ");
    printf("%x\n", (header->e_phoff));
    // ----------------------------------------
    printf("8) The number of program header entries: ");
    printf("%d\n", (header->e_phnum));
    // ----------------------------------------
     int ProgramsNum = header->e_phnum;
    Elf32_Phdr* program = (Elf32_Phdr*)(map_start + header->e_phoff);
    printf("9) The size of each program header entry:\n");
     for (int i=0; i<ProgramsNum; i++)
    {
        printf("program number: %d Size: %d\n" , i, program->p_filesz);
    	program++;
    }
    // ----------------------------------------
}

void debug()
{
    if (debugFlag)
    {
       printf("Debug flag now off"); 
       debugFlag = 0;
    }

    else
    {
        printf("Debug flag now on"); 
        debugFlag = 1; 
    }
}

void printSection(){
    printf("not implemented yet"); 
}

void printSymbol(){
    printf("not implemented yet"); 
}

void quit()
{
    if (Currentfd > -1)
        close(Currentfd);
    if (map_start)
        munmap(map_start, fdSize);
    exit(0);
}


// char* map(char *array, int array_length, char (*f) (char))
// {
//   char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
//   int i=0;
//   while (i<array_length)
//   {
//     mapped_array[i] = f(array[i]);
//     i++;
//   }
//   return mapped_array;
// }



int main(int argc, char *argv[])
{
    FILE* out = stdout;
	FILE* in = stdin;
	char* carray = calloc(5, sizeof(char));
	struct fun_desc menu[] = {
    {"Toggle Debug Mod" ,debug},
    {"Examine ELF File" , examine},
    {"Print Section Names", printSection},
    {"Print Symbols" , printSymbol},
    {"Quit" , quit},
    {NULL , NULL}};
	unsigned int bound = (int)(sizeof(menu))/sizeof(menu[0]) -1; 
	//bound = bound - 2; /*because the last null pointer and because the count starts from 0*/
	int i, user;
	while(1)
    {
		i = 0; 
		puts("Please choose a function:");
		while(menu[i].name != NULL){
			fprintf(out, "%d- %s\n" , i , menu[i].name);
			i++;
		}	
		scanf("%d", &user);
		if(user > bound || user < 0){
			puts("Not within bounds");
			exit(0);	
		}
		menu[user].fun();
		puts("Done.\n");
		
	}
	free(carray);
    return 0; 
}

