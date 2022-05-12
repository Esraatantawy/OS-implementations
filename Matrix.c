#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


int input1[20][20];
int input2[20][20];
int row_1,col_1,row_2, col_2;
int outputByRow[20][20];
int outputByElement[20][20];
pthread_t th[50];
pthread_t th2[50];
int k;

typedef struct indices{
  int row;
  int coloumn;
}indices;




void readFromFile()
{ 	int i,j,k;
	char *name=(char *)malloc(50);
	
	
        printf("Enter filename: ");
	scanf("%s",name);
        FILE *fp=fopen(name,"r");
        
        while(fp==NULL)
        { printf("Invalid file name\n");
        printf("Please try again: ");
        scanf("%s",name);
        fp=fopen(name,"r");
        }
  	
	fscanf(fp,"%d %d\n",&row_1,&col_1);
	for(i=0;i<row_1;i++)
	{ for(j=0;j<col_1;j++)
	  { fscanf(fp,"%d",&input1[i][j]);
	   }
	   fscanf(fp,"\n");	
	}
	fscanf(fp,"%d %d\n",&row_2,&col_2);
	for(i=0;i<row_2;i++)
	{ for(j=0;j<col_2;j++)
	  {  fscanf(fp,"%d",&input2[i][j]);
	   }
	   fscanf(fp,"\n");	
	}

	printf("Matrix 1 is: \n");
	for(i=0;i<row_1;i++)
	{	for(j=0;j<col_1;j++)
		{ printf("  %d  ",input1[i][j]);
		}	
		printf("\n");
	}	
       printf("Matrix 2 is: \n");
	for(i=0;i<row_2;i++)
	{	for(j=0;j<col_2;j++)
		{ printf("  %d  ",input2[i][j]);
		}	
		printf("\n");
	}
	
	fclose(fp);


}

void multByElement(void *args)
{  
	indices *temp = (indices *)args;
	int current;
	
	pthread_t id =pthread_self();
	
	for(int i=0;i<(row_1*col_2);i++)
	{ if(pthread_equal(id,th[i]))
	{  current = i;
		break;
	}
	}
	
 			for(k=0;k<col_1;k++){
      outputByElement[temp[current].row][temp[current].coloumn]+=input1[temp[current].row][k]*input2[k][temp[current].coloumn];}

}

void multByRow(void *args)
{  
	int *temp = (int *)args;
	int current;
	int i;
	
	pthread_t id =pthread_self();
	
	for(i=0;i<(row_1);i++)
	{ if(pthread_equal(id,th2[i]))
	{  current = i;
		break;
	}
	}
	
	for(i=0;i<col_2;i++)
		for(k=0;k<col_1;k++){
      outputByRow[temp[current]][i]+=input1[temp[current]][k]*input2[k][i];}

}





int main()
{  	int i,j,k,p;
	
	readFromFile();	 //Read input matrices from file
    
    indices multIndices[row_1*col_2];
    int colIndices[col_2];
     
     //Check dimension condition is satisfied
      if(col_1 != row_2)
 	{ printf("Multiplication cannot take place\n");
	 exit(0);}
     	
     	//Zero the output arrays
     	 for(i=0;i<row_1;i++)
 	 for(j=0;j<col_2;j++)
 	{  outputByElement[i][j]=0;}
 	
     	 for(i=0;i<row_1;i++)
 	 for(j=0;j<col_2;j++)
 	{ outputByRow[i][j]=0;}
 	  
 	  
 	
 	
 	//Fill array of indices with combinations needed for multiplication
 	p=0;
 	for(i=0;i<row_1;i++)
 		for(j=0;j<col_2;j++) 
 	{  multIndices[p].row = i;
 	   multIndices[p].coloumn = j;
 	   p++;}
 	   
 	 for(i=0;i<row_1;i++)
 	 colIndices[i]=i;
 	
 	// element by element threading
 	double time_spent;
  	clock_t begin = clock();
  
  	for(i=0;i<(row_1*col_2);i++)
 	    {   	
 	     pthread_create(&th[i],NULL,&multByElement,&multIndices);
 	    }
 
	
	for(i=0;i<(row_1*col_2);i++)
	{ pthread_join(th[i],NULL);
	}
	
	clock_t end = clock();
	time_spent += (double)(end - begin)/CLOCKS_PER_SEC;
	
	printf("*********ELEMENT BY ELEMENT*********\n");
	printf("Matrix result is: \n");
	for(i=0;i<row_1;i++)
	{	for(j=0;j<col_2;j++)
	          printf("   %d   ",outputByElement[i][j]);
	          printf("\n"); 
	}
	printf("Time elapsed for element by element thread is: %f \n",time_spent); 

                
        //row by row threading
        time_spent=0;
        begin = clock();
        
  
  	for(i=0;i<row_1;i++)
 	    {   	
 	     pthread_create(&th2[i],NULL,&multByRow,&colIndices);
 	    }
 	    
 
	
	for(i=0;i<row_1;i++)
	{ pthread_join(th2[i],NULL);
	}
	
	end = clock();
	time_spent += (double)(end - begin)/CLOCKS_PER_SEC;
	
	
	printf("*********ROW BY ROW*********\n");
	printf("Matrix result is: \n");
	for(i=0;i<row_1;i++)
	{	for(j=0;j<col_2;j++)
		{ 
	          printf("   %d   ",outputByRow[i][j]);}
	          printf("\n"); 
	}
	printf("Time elapsed for row by row thread is: %f \n",time_spent); 
        
        
	return 0;
	}
