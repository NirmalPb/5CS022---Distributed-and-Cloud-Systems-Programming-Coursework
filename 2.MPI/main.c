/*
 Create an MPI program that will calculate 4 statistical measures for a file of numbers. These are the total,
 the average, that largest positive number and the largest negative number. The MPI program must read the 
 numbers in from the file in the Rank 0 process and send the appropriate share of numbers to the processes 
 of other Ranks. These processes will then calculate the 4 statistical measures for the number that they have 
 and then send those back to the Rank 0 process, which will consolidate all the values from all the Ranks into 
 the final 4 values, and print them out. Your program must be able to work with a minimum of 2 processes and a 
 maximum of 10 processes. The Rank 0 process should share the numbers between all the other Ranks as equally as 
 possible. This task will contribute 15% of the marks to the Portfolio.
*/
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

//find the total
float findtotal(float *numarray , int arraysize){
	float total = 0;

	for (int i = 0; i < arraysize; ++i)
	{
		total+=numarray[i];
	}
	return total;
}

//find the avg
float findavg(float *numarray , int arraysize){
	float total = 0;

	for (int i = 0; i < arraysize; ++i)
	{
		total+=numarray[i];
	}
	float avg = total/6170;
	return avg;

}

//find the largest number
float findlargest(float *numarray , int arraysize){
	float largest = numarray[0];

	for (int i = 0; i < arraysize; ++i)
	{
		if(largest < numarray[i]){
			largest = numarray[i];
		}
	}
	return largest;
}

//find the smallest number
float findsmallest(float*numarray , int arraysize){
	float smallest = numarray[0];

	for (int i = 0; i < arraysize; ++i)
	{
		if(smallest > numarray[i]){
			smallest = numarray[i];
		}
	}
	return smallest;


}

int main(int argc, char  **argv[])
	
{
	//MPI Setup
	int size;
	int rank;
	//tags
	int tag=0;
	int tottag=1;
	int avgtag=2;
	int largetag=3;
	int smallesttag=4;

	float *nums;
	float finalTotal = 0;
	float finalAverage = 0;
	float finalLargest = 0;
	float finalSmallest;

	MPI_Init(&argc,argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if (rank == 0){
		
		FILE *fp = NULL;  
		float val = 0.0;  
		int count=0; 
		int chunksize=0;
		int rem=0;
		int start=0;
		int numcount;
		char c;  

		// for results from other ranks
		float* total;
		float* average;
		float* largest;
		float* smallest;


		//rank error check
		if (size<=1)
		{
			printf("Must have more than 2 ranks curently %d\n",size );
			exit(1);
		}

		// Count the how many numbers there are  
		fp = fopen("NumbersForMPI.txt", "r");

		while(!feof(fp)){    
			if(fscanf(fp,"%f%c",&val,&c)>0){
			      count++;    
			} 
		}  
		fclose(fp);

		printf("\nNumber Count = %d\n\n",count);  

		// Allocate the necessary memory  
		nums = malloc(count * sizeof(float));

		total = malloc((size-1) * sizeof(float)); 
		average = malloc((size-1) * sizeof(float)); 
		largest = malloc((size-1) * sizeof(float)); 
		smallest = malloc((size-1) * sizeof(float)); 

		// Read the numbers into memory  
		fp = fopen("NumbersForMPI.txt", "r");
		for(int i=0; i<count; i++){    
			fscanf(fp,"%f%c",&(nums[i]),&c);  
		}  
		fclose(fp);

		//send the numbers to other ranks
		chunksize=count/(size-1);
		numcount=count/(size-1);
		rem = count% (size-1);

		//send numbers
		for (int i = 1; i < size; ++i)
		{
			start = numcount*(i-1);
			
			if (i==(size-1)){
				numcount+=rem;
			}

			printf("Sending %d numbers to rank %d...\n\n",numcount,i);
			MPI_Send((&nums[start]),numcount,MPI_FLOAT,i,tag,MPI_COMM_WORLD);
		}

		//recv results from other ranks
		float temptotal [1];
		float tempavg [1];
		float templargest[1]; 
		float tempsmallest [1];
		
		for (int sender = 1; sender < size; ++sender)
		
		{
			MPI_Recv(temptotal,1,MPI_FLOAT,sender,tottag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(tempavg,1,MPI_FLOAT,sender,avgtag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(templargest,1,MPI_FLOAT,sender,largetag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(tempsmallest,1,MPI_FLOAT,sender,smallesttag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

			total[sender-1]=temptotal[0];
			average [sender-1] = tempavg[0]; 
			largest [sender-1] = templargest[0]; 
			smallest [sender-1] = tempsmallest[0]; 

		}

		//find the final total
		finalTotal=findtotal(total,(size-1));

		//find the final average
		finalAverage = findtotal(average,(size-1));

		//find largest final positive number
		finalLargest = findlargest(largest,(size-1));

		//the largest final negative number
		finalSmallest = findsmallest(smallest,(size-1));
		

		//Print the final results
		printf("\nTotal = %f\n",finalTotal);
		printf("Average = %f\n",finalAverage);
		printf("Largest positive number = %f\n",finalLargest);
		printf("Largest negative number = %f\n\n",finalSmallest);

		free(total);
		free(average);
		free(largest);
		free(smallest);
		free(nums);
		

	}

	else{

		MPI_Status status;
		int numcount;

		//probe for incoming message
		MPI_Probe(0,tag,MPI_COMM_WORLD,&status);
		
		
		//get the incoming float count
		MPI_Get_count(&status,MPI_FLOAT,&numcount);

		//allocate local mem buffer
		nums = malloc(sizeof(float)*numcount);

		//receive the message
		MPI_Recv(nums,numcount,MPI_FLOAT,0,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		
		//find the final total
		finalTotal=findtotal(nums,numcount);

		//find the final average
		finalAverage = findavg(nums,numcount);

		//find largest final positive number
		finalLargest = findlargest(nums,numcount);

		//the largest final negative number
		finalSmallest = findsmallest(nums,numcount);

		//Send the results back
		MPI_Send(&finalTotal,1,MPI_FLOAT,0,tottag,MPI_COMM_WORLD);
		MPI_Send(&finalAverage,1,MPI_FLOAT,0,avgtag,MPI_COMM_WORLD);
		MPI_Send(&finalLargest,1,MPI_FLOAT,0,largetag,MPI_COMM_WORLD);
		MPI_Send(&finalSmallest,1,MPI_FLOAT,0,smallesttag,MPI_COMM_WORLD);

		free(nums);
	}

	MPI_Finalize();
	return 0;
}
