#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

// gcc 6277.sort -lpthread

//gcc 6277.sort  -lpthread -o a && ./a

// merge sort using sublists where each sublist is sorted using a thread recursively and then merging at the end
// o/p expected file is the sorted elements.
  
// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]

int a[100];

struct index
{
int left;
int right;
}index;


void Read_Matrix()
{ 

   FILE *fp; 
    
   int n;
   printf("In Read Matrix\n");       
   fp = fopen("matrix.txt", "r");
   fscanf(fp, "%d", &n);
  printf("no of elements=%d\n",n);
  
  for (int i=0;i<n;i++)
  {
    fscanf(fp, "%d", &a[i]);
  
  }
   for (int i=0;i<n;i++)
  {
  printf("%d ", a[i]);
  
  }
 }  
void merge(int a[],int left,int middle,int right)
{
    int i, j, k;
    int n1=middle-left+1;
    int n2=right-middle;
  
    
    int L[n1], R[n2];
  
 
    for (i=0;i<n1;i++)
        L[i]=a[left+i];
    for (j = 0; j < n2; j++)
        R[j]=a[middle+1+j];
  
    i = 0; j = 0;
    k = left; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            a[k] = L[i];
            i++;
        }
        else {
            a[k] = R[j];
            j++;
        }
        k++;
    }
  
   while (i < n1) {
        a[k] = L[i];
        i++;
        k++;
    }
  
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        a[k] = R[j];
        j++;
        k++;
    }
}
  
void *mergesort(void *args)
{ 
struct index *temp=(index *) args;
int left= temp->left;
int right= temp->right;
pthread_t th1;
pthread_t th2;
struct index left_array;
struct index right_array;
struct index left_array.left=left;
right_array.right=right;



int middle;
left_array.left=middle;
right_array.right=middle+1;

  if (left<right) {
  
        middle= left+(right-left) / 2;
  
        mergeSort(a,left,middle);
        mergeSort(a,middle+1,right);
        pthread_create (&th1, NULL,&mergeSort, &left_array) ;  
        pthread_join (th1, NULL);
        pthread_create (&th2, NULL, &mergeSort, &right_array) ;
        pthread_join(th2 , NULL);
        merge(a,left,middle,right);
    }
}  

/* Function to print an array */

/*Driver code */
int main()
{  

   struct index merger;
   
   Read_Matrix();
  
   merger.left=0;
   merger.right=0;
   
    mergeSort(&merger);

    return 0;
    
    
}































	





