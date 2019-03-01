#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



Elf32_Phdr *read_exec_file(FILE *opfile);
int count_kernel_sectors(Elf32_Phdr *Phdr);
void write_bootblock(FILE *image, FILE *bbfile, Elf32_Phdr *Phdr);
void extent_opt(Elf32_Phdr *Phdr_bb, Elf32_Phdr *Phdr_k, int kernelsz);



Elf32_Phdr *read_exec_file(FILE *opfile)
{
  FILE * file = opfile; //##
  Elf32_Ehdr * Ehdr;
  Elf32_Phdr * Phdr;

  if( (Ehdr= (Elf32_Ehdr *) malloc(52)) == NULL) 	//malloc Ehdr
  {
    printf("malloc Ehdr failed!\n");
    exit(-1);
  }

  fread(Ehdr, 52, 1, file); 						 //get the ELF header
  
  fseek(file, Ehdr->e_phoff, SEEK_SET);  //seek the first program header
  if( (Phdr = (Elf32_Phdr *) malloc( ((Ehdr)->e_phnum + 1)*sizeof(Elf32_Phdr) )) == NULL)  //malloc Phdr
  {
    printf("malloc Phdr failed!\n");
    exit(-1);
  }

  fread(Phdr, sizeof(Elf32_Phdr), Ehdr->e_phnum, file);  //get all program header

  Phdr[Ehdr->e_phnum] = Phdr[Ehdr->e_phnum - 1];
  Phdr[Ehdr->e_phnum].p_flags = -1; 		 //identify the end
	
  free(Ehdr);
  Ehdr = NULL;
	
  return Phdr;
}

int count_kernel_sectors(Elf32_Phdr *Phdr)
{
  uint8_t sectors_num = 0;
	
  int i;
  for(i = 0; Phdr[i].p_flags != -1; i++)  //calculate the number of Byte
    sectors_num += Phdr[i].p_filesz;
		
  sectors_num = sectors_num/512 + (sectors_num%512 != 0); //calculate the number of sector
  return sectors_num;
}

void write_bootblock(FILE *image, FILE *file, Elf32_Phdr *Phdr)
{
  char buffer[512];
  int i;
  int cur_capacity = 0;

  for(i = 0; Phdr[i].p_flags != -1; i++)
  {
    fseek(file, Phdr[i].p_offset, SEEK_SET);
    fread(buffer + cur_capacity, Phdr[i].p_filesz, 1, file);
    cur_capacity += Phdr[i].p_filesz;
  }
  fwrite(buffer, cur_capacity, 1, image);
}

void write_kernel(FILE *image, FILE *knfile, Elf32_Phdr *Phdr, int kernelsz)
{
  char buffer[kernelsz*512];
  int i;
  int cur_capacity = 0;

  for(i = 0; Phdr[i].p_flags != -1; i++)
  {
    fseek(knfile, Phdr[i].p_offset, SEEK_SET);
    fread(buffer + cur_capacity, Phdr[i].p_filesz, 1, knfile);
    cur_capacity += Phdr[i].p_filesz;
  }

  fseek(image,512,SEEK_SET);
  fwrite(buffer, cur_capacity, 1, image);
  fseek(image,0,SEEK_SET);
}

void record_kernel_sectors(FILE *image, int kernelsz)
{
  fseek(image, 508, SEEK_SET);
  int size = kernelsz*512;
  fwrite(&size, 4, 1, image);
}

void extent_opt(Elf32_Phdr *Phdr_bb, Elf32_Phdr *Phdr_k, int kernelsz)
{
  printf("bootblock:\n");
  printf("sectors: 1\n");
  printf("size: 512\n");

  printf("kernel:\n");
  printf("sectors:2\n");
  printf("size: %d\n",kernelsz*512);
}

int main()
{
  Elf32_Phdr * bootblock_phdr, * kernel_phdr;
  int kernel_size;
  
  FILE * image_file;
  if((image_file = fopen("image", "wb+")) == NULL)
  {
    printf("open image failed!\n");
    exit(1);
  }
  
  FILE * bootblock_file;
  if((bootblock_file = fopen("bootblock", "r"))==NULL)
  {
    printf("open bootblock failed!\n");
    exit(1);
  }
  
	FILE * kernel_file;
  if((kernel_file = fopen("kernel", "r"))==NULL)
  {
    printf("open kernel failed!\n");
    exit(1);
  }

  bootblock_phdr = read_exec_file(bootblock_file);
  kernel_phdr = read_exec_file(kernel_file);
  
  kernel_size = count_kernel_sectors(kernel_phdr);
 
  write_bootblock(image_file, bootblock_file, bootblock_phdr);
  write_kernel(image_file, kernel_file, kernel_phdr, kernel_size);
  
  record_kernel_sectors(image_file, kernel_size);
  
  extent_opt(bootblock_phdr, kernel_phdr, kernel_size);
	
  fclose(image_file);
  fclose(bootblock_file);
  fclose(kernel_file);
	
  return 0;
}
