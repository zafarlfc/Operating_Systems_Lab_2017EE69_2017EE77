#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define BACKING_STORAGE_FILE "BACKING_STORE.bin"

struct TLB
{
	int8_t pnum;
	int8_t fnum;
}tlb[16];

struct page_table
{
	int8_t pnum;
	int8_t fnum;
	int validity;
}ptable[256];

struct physical
{
       int frame;
       char value[256];
}mem[256];

int main()
{       char result;
	char const* const fileName = "addresses.txt";
	FILE* file = fopen(fileName, "r"); 
	if(!file){
		printf("\n Unable to open : %s ", fileName);
		return -1;
	}
	
	int32_t logical;
	int32_t pnumber;
	int32_t offset;
	int frame;
	int32_t physical;
	int num[100];
	
	int total = 0;
	int pfault = 0;
	int tlbhit = 0;
	int i, a, s;
	
	// initializing the TLB
	for(i=0;i<16;i++)
	{
	     tlb[i].pnum=0;
	     tlb[i].fnum=0;
	}
	
	// initializing the page table
	for(i=0; i<256; i++)
	{
	     ptable[i].pnum = 0;
	     ptable[i].fnum = 0;
	     ptable[i].validity = 1;
	}
	
	// initializing the physical memory
	for(i=0; i<256; i++)
	{
		mem[i].frame=i;
		for(int j=0;j<256;j++){
		mem[i].value[j]=1;}
	}
	
	char line[8];

	// Reading the Logical Adderss from the address.txt 

	while (fgets(line, sizeof(line), file)) 
	{
		total++;
		sscanf(line, "%u", &logical);
		offset  = logical & 0xFF;
		pnumber = logical & 0xFF00;
		pnumber = pnumber >> 8;
		
		// Finding in the TLB
 		for(i=0; i<16; i++)
		{
			if(pnumber == tlb[i].pnum)
			{
				frame = tlb[i].pnum;
				tlbhit++;
				a=1;
				break;
			}
        	}
	 
		// if not in TLB then look in the page table 
	        if (a!=1)
        	{     
        		//tlbfault++;  
			for(i=0; i<256; i++)
			{
				if(pnumber==ptable[i].pnum && ptable[i].validity==1)
				{
					frame = ptable[i].fnum;
					s=1;
					break;		
        			}
        		}
        		
        		// if the value is found in the page table then insert it into TLB using FIFO
        		if(s==1)
        		{	
        			for(i=15;i>0;i--)
        			{
					tlb[i].pnum = tlb[i-1].pnum;
					tlb[i].fnum = tlb[i-1].fnum;
				}
				tlb[0].pnum = pnumber;
				tlb[0].fnum = frame ;
        		}
        	}        	
       	
        	if(s!=1 && a!=1)
      		{
      			//if the value is not in the page table then a page fualt occurs
        	     	pfault++;
        	     	
        	     	
  			int fd = open("BACKING_STORE.bin", O_RDONLY);
  			size_t pagesize = getpagesize();
 			char * region = mmap(NULL,pagesize,PROT_READ, MAP_FILE|MAP_PRIVATE,fd,0);
  			//int a=1;
  			//new stores the page from backing storage using frame number
			char new[256];
			//memcpy(new,region+pnumber*256,256);
			memcpy(new,region+pnumber,256);
                      int unmap_result = munmap(region,pagesize);
                      close(fd);
        	    	//Turning validity to 1
        	       for(i=0;i<256;i++){
				if(ptable[i].pnum==pnumber){
				     ptable[i].validity=1;}}
				     
				     
                       				     
        	    	 
        	    	//physical = frame<<8 | offset;
        	    	 //Storing value of new to required physical memory
        	    	 for (i=0; i<256; i++)
        	    	 {
        	    	       if(frame == mem[i].frame)
        	    	       {
        	    	           for(int d=0;d<256;d++){
        	    	           mem[i].value[d]=new[d];}	
        	    	           
        	    	       }
        	    	 	  
        	    	                       
        		 }
        	}
               //Finding physical address from offset and frame number
        	physical = frame<<8 | offset;
	        //Finding value from physical memory using frame number and offset
		for(i=0; i<256; i++)
		{
			if(frame == mem[i].frame)
			{
				result=mem[i].value[offset];
			     
			     
			        //printing the results
				printf("\nLogical Address = %u| Physical Address = %u| value = %x", logical, physical, result);
				break;
			}
		}
	


	}
	
	float pfault_rate = ((float)pfault/(float)total) * 100;
	float tlbhit_rate = ((float)tlbhit/(float)total) * 100;
	
	printf("\n\n\t\t TLB Hit Rate = %.2f \%\n", tlbhit_rate);
	printf("\n\t\t Page Fault Rate = %.2f \%\n", pfault_rate);
	
	//fclose(backing_storage);
	fclose(file);	

	return 0;
}

