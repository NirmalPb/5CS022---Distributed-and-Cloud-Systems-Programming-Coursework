/*
*Workshop Task_01
*The file "WarAndPeace.txt" on Canvas contains the entire text of the book "War and Peace" by Leo Tolstoy.
Write an MPI program to count the number of times each letter of the alphabet occurs in the book. Count
both the upper case and the lowercase as the same. Ignore any letter with accents such as " é " and so on.
Your MPI program should work with any number of processes from 1 to 100 process. Only Process rank 0 (zero)
should read in the file and send the appropriate chunk of file to each other process. The other processes should 
not read in the file.You should submit this program as "workshoptask1.c" as part of your final portfolio 
submission. You can also upload it to the formative submission point for formative feedback.

*Nirmal Abykoon Mudiyanselage "1811342"
*5CS022 - Distributed and Cloud Systems Programming (Wolverhampton City Campus)
*/

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>


int main(int argc, char  **argv[])
{
	//letter count
	int templettercount[26]={0};
	int tag=0;
	int ch;
	
	//MPI Setup
	int size;
	int rank;
	MPI_Init(&argc,argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(rank==0){
		char *filename = "WarAndPeace.txt"; //file name
		char* chararray;
		FILE* charfile = fopen(filename,"r"); //opening the file
		long filelength=0;
		int lettercount[26]={0};
		int i=0;
		int chunksize=0;
		int rem=0;
		int start=0;
		int charcount;

		//read error check
		if (charfile==NULL){
			printf("Error importing File. Try Again... \n");
			exit(0);
		}

		// finding number of letters in the file
		while((ch=fgetc(charfile))!=EOF){
			if ((ch >= 'a' && ch <= 'z')|| (ch >= 'A'&& ch <= 'Z')){
				filelength++;
			}
		} 
		rewind(charfile);

		
		printf("File has %ld letters\n",filelength ); // displaying number of letters
		chararray=malloc(sizeof(char)*filelength);

		//store numbers in an array
		while((ch=fgetc(charfile))!=EOF){
			if ((ch >= 'a' && ch <= 'z')|| (ch >= 'A'&&ch <= 'Z')){
				chararray[i]=(char)ch;
				i++;
			}
		}

		//Send data to other ranks
		chunksize=filelength/size;
		charcount=filelength/size;
		rem = filelength%size;
		printf("Keeping %d letters in rank %d\n", charcount,rank);

		for (int receiver = 1; receiver < size; ++receiver)
		{
			start=charcount*receiver+1;
			
			if (receiver==(size-1)){
				charcount+=rem;
			}

			printf("Sending %d letters to rank %d...\n",charcount,receiver);
			MPI_Send((&chararray[start]),charcount,MPI_CHAR,receiver,tag,MPI_COMM_WORLD);
		}

		//calc results with rank 0
		for (int letter = 0; letter <= chunksize; ++letter)
		{
			ch=chararray[letter];
			if(ch >= 65 && ch <= 90){
				lettercount[(ch-65)]++;
			}
			if(ch >= 97 && ch <= 122){
				lettercount[(ch-97)]++;	
			}
		}
		
		//receive results
		for (int sender = 1; sender < size; ++sender)
		{
			MPI_Recv(templettercount,26,MPI_INT,sender,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

			//add results of other rank to main rank
			int index =0;
			for (int count = 0; count < 26; ++count)
			{
				lettercount[index]+=templettercount[index];
				index++;
			}
		}

		/*Print Results*/
		printf("\nLetter count:\n");
		for (int i = 0; i < 26; ++i){
			printf("Letter %c occurs %d times.\n",i+'a',lettercount[i]);
		}
		
		free(chararray);
		
	}else{
		
		MPI_Status status;
		int charcount;
		char* chararray;

		//probe for incoming message
		MPI_Probe(0,tag,MPI_COMM_WORLD,&status);
		
		//get the incoming char count
		MPI_Get_count(&status,MPI_CHAR,&charcount);

		//allocate local mem buffer
		chararray = malloc(sizeof(char)*charcount);

		//receive the message
		MPI_Recv(chararray,charcount,MPI_CHAR,0,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		//calc results with rank 0
		for (int letter = 0; letter <= charcount; ++letter)
		{
			ch=chararray[letter];
			if(ch >= 65 && ch <= 90){
				templettercount[(ch-65)]++;
			}
			if(ch >= 97 && ch <= 122){
				templettercount[(ch-97)]++;	
			}
		}

		//send results back
		MPI_Send((&templettercount[0]),26,MPI_INT,0,tag,MPI_COMM_WORLD);

		free(chararray);
	}

	MPI_Finalize();
	return 0;
}