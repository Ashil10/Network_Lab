// LEAKY BUCKET ALGORITHM::

// The leaky bucket algorithm is a method of congestion control
//where variable number of requests are temporarily stored and organized
//into a set-rate output of packets in an asynchronous transfer mode.
//It is used to implement traffic policing and shaping in Ethernet and Cellular data networks.

//Below given is a C language implementation of the same.

#include <stdio.h>
#include <stdlib.h>
#define min(x,y) (x<y)?x:y

int main()
{
	int outrate,drop=0,bsize,rem=0,nsec;
	int input[20]={0},i=0,ch,x,flag;
	
	// This specifies the bucket size. It is the amount of data which can be hold in the buffer.
	printf("Enter Bucket(Bytes) Size : ");
	scanf("%d",&bsize);
	
	// This specifies the constant output rate, in which data will be given out. It is usually lower than input rate, inorder to prevent any delays.
	printf("Enter output rate(Bytes/sec) : ");
	scanf("%d",&outrate);
	
	do
	{
		// This specifies each input data. 
		printf("Enter the size(Bytes) of packet coming at sec %d : ",i+1);	
		scanf("%d",&input[i]);
		i++;
		printf("Enter 1 to continue/ 0 to quit :");
		scanf("%d",&ch);
	}while(ch);
	
	nsec=i;
	printf("\nTime Recieved\t Sent \t Dropped \t Remaining\n");
	for(i=0;i<nsec ||rem ;i++)
	{	
		flag=0;
		printf("%d",i+1);
		printf(" \t%d",input[i]);
		if(input[i]+rem>bsize){
			flag=input[i];
			printf("\t%3d",min(rem,outrate));
			//If the bucket contains more data, after an output.
			if(rem-outrate>0)
				rem=rem-outrate;
			else
				rem=0;
			printf("%10d%15d\n",flag,rem);
			continue;
		}	
		printf("\t%3d",min(input[i]+rem,outrate));
		
		//If the bucket contains more data, when the new input is added.
		if((x=input[i]+rem-outrate)>0)
		{
			if(x>bsize)
			{
				rem=bsize;
				drop=x-bsize;
			}
			else
			{
				rem=x;
				drop=0;
			}
		}
		else
		{
			drop=0;
			rem=0;
		}
		
		/*
		if(flag!=0)
		{
			rem -= (flag+outrate);
			printf("%10d%15d\n",flag,rem);
			flag=0;
		}

		else
				
		*/
			printf("%10d%15d\n",drop,rem);
	}
	return 0;
}

