#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<readline/readline.h>
#include<readline/history.h>

//gcc lab1.c -o lab1

void handler(int sig)
{
FILE *fp;
fp = fopen("log.bin","wb+");

pid_t chpid = wait(NULL);

if(fp == NULL)
{
    fp = fopen("log.bin","wb+");
    if(fp == NULL)
    {
        printf("File is not opening.");
        exit(1);
    }
}
    fprintf(fp,"Child pid %d ended (signal %s)\n",chpid,sig);
    fclose(fp);
}


int main()
{
int i=0,j=0,k=0;
char p[100];
char *array[100];
//char  str[100];
int result=0;

while (k<100)
    {
    array[k]=malloc(100*sizeof(char));
    k++;
    }
 
printf("Enter commands:");

gets(p);

ptintf("%s",p);

//scanf("%[^\n]",str);

printf("You entered: %s", p);
     
    while (p != NULL)
    { 
        *p = strtok (NULL, " ");// This tokenizes the string til space
        // and stores the results in p           
        strcpy( array[i++] ,p);
        printf("p = %s", p);                        
    } 
    
    for(;;) {
    
    if(strcmp(array[0],"exit")==0)
         exit(0);
      
      pid_t pid = fork();
      
      printf("pid=%d",pid);

	if (pid < 0) { 
	
	printf("Fork failed Unable to create child process.\n");
		return 1;
	}
	else if (pid == 0) { 
	
printf("I'm the Child => PPID: %d PID: %d\n", getppid(), getpid());
		printf("Calling hello.c from child process\n");
		execvp("./hello", array);
                           }

	else { 	
		printf("I'm the Parent => PID: %d\n", getpid());
		printf("Waiting for child process to finish.\n");
             } 
	
 }
 
 
  free(array);
 

}

































	





