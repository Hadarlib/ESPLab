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

void* map_start = NULL;
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

    if (map_start == MAP_FAILED)
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
    Elf32_Shdr* shdr = (Elf32_Shdr*)(map_start + header->e_shoff);
    printf("6) The size of each section header entry: \n");
    for (int i=0; i<sectionsNum; i++)
    {
        printf("section number: %d Size: %d\n" , i, shdr->sh_size);
    	shdr++;
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

char* getType(int i)
{
    char* type = "";
    switch(i)
    {
        case 0:
            type =  "NULL";
            break;
        case 1:
            type =  "PROGBITS";
            break;
        case 2:
            type =  "SYMTAB";
            break;
        case 3:
            type =  "STRTAB";
            break;
        case 4:
            type =  "RELA";
            break;
        case 5:
            type =  "HASH";
            break;
        case 6:
            type =  "DYNAMIC";
            break;
        case 7:
            type = "NOTE";
            break;
        case 8:
            type = "NOBITS";
            break;
        case 9:
            type = "REL";
            break;
        case 10:
            type ="SHLIB";
            break; 
        case 11:
            type = "DYNSYM";
            break;
        case 0x70000000:
            type = "LOPROC";
            break;
        case 0x7fffffff:
            type ="HIPROC";
            break;
        case 0x80000000:
            type ="LOUSER";
            break;       
        case 0xffffffff:
            type ="HIUSER";
            break;                              
    }

    return type;
}

void printSection()
{
    if (map_start == NULL)
    {
        printf("file is not defined"); 
        return;
    }

    Elf32_Ehdr *ehdr = (Elf32_Ehdr*) map_start;
    Elf32_Shdr *shdr = (Elf32_Shdr *)(map_start + ehdr->e_shoff);
    unsigned int sectionsNum = ehdr->e_shnum;
    Elf32_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
    const char *const sh_strtab_p = map_start + sh_strtab->sh_offset;

    printf("Section Headers:\n");
    printf("[Nr] Name                 Addr      Off     Size    Type");

    if(debugFlag)
        printf("      Name offset");
    printf("\n");

    for(int i = 0 ; i < sectionsNum ; i++)
    {
        Elf32_Addr addr = shdr->sh_addr;
        Elf32_Off offset = shdr->sh_offset;
        Elf32_Word size = shdr->sh_size;
        printf("[%2d] %-20s %08x  %06x  %06x  %-10s", i, sh_strtab_p + shdr->sh_name, addr, offset, size, getType(shdr->sh_type));
        if(debugFlag)
            printf("%06x",shdr->sh_name);
        printf("\n");
        shdr++;
    }
    printf("\n");

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


int main(int argc, char *argv[])
{
    FILE* out = stdout;
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

