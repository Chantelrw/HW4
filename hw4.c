/*Honor Code:
I, Chantel Rose Walia, declare that I have completed this assignment completely and 
entirely on my own, without any unathorized consultation from others or unathorized access 
to online websites. I have read the UAB Academic Honor Code and understand that any breach of 
the UAB Academic Honor Code may result in severe penalties.
Student Signature/Initials: CRW
Date: 16 April 2023
*/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>

/* 
Name: Chantel Rose Walia
BlazerId: CWALIA
Assignment: HW4
To Compile: type gcc -Wall -o hw4 hw4.c queue.c -lpthread
To Run: ./hw4 <max # of concurrent threads>
        then type submit, showjobs, submithistory or exit.
*/

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int P,running;
jobs job_array[1024];
queue *job_queue;

void *run_job(void *arg){
    jobs *job=(jobs *)arg;
    time_t t1, t2;
    char error[2]="3\0";

    // updating the global running variable
    pthread_mutex_lock(&mutex);
    running++;
    pthread_mutex_unlock(&mutex);
    
    // setting current job status as 1 (running)
    job->status=1;
    job->print_status="Running";
    
    int status; // child status
    int pipefd[2];
    
    // creating pipe
    if(pipe(pipefd)<0){
        perror("pipe");
        exit(-1);
    }

    // forking
    pid_t pid=fork();

    t1=time(NULL); // getting start time

    if(pid==0){ // child
        int fdout,fderr;

        // create and replace standard output stream with the file <job_id>.out
        if ((fdout = open(job->fout, O_CREAT | O_TRUNC | O_WRONLY, 0755)) == -1) {
            printf("Error opening file <job_id>.out for output\n");
            exit(-1);
        }
        dup2(fdout, 1);

        // create and replace standard error stream with the file <job_id>.err
        if ((fderr = open(job->ferr, O_CREAT | O_TRUNC | O_WRONLY, 0755)) == -1) {
            printf("Error opening file <job_id>.err for output\n");
            exit(-1);
        }
        dup2(fderr, 2);
        
        execvp(job->args[0],job->args);
        
        // using pipe to communicate with parent when a job is unsuccessful
        close(pipefd[0]); // closing the reading end
        write(pipefd[1], error, 2); // writing to pipe
        close(pipefd[1]); // closing the writing end

        perror("execvp");
        exit(-1);

    }else if(pid>0){
        wait(&status);
        t2=time(NULL);
        
        // reading from pipe
        char e[2];
        close(pipefd[1]); // closing the writing end
        read(pipefd[0], e, 2); // reading the pipe into char[] e
        close(pipefd[0]); // closing the reading end
        
        if(strcmp(e,"3\0")==0){ // checking if the job was unsuccessful and setting the status accordingly
            job->status=3;
            job->print_status="Unsuccessful";
        }else{
            job->status=2;
            job->print_status="Successful";
        }

        char *c1=strtok(ctime(&t1),"\n"); // storing start time
        job->start_time=strdup(c1); 
        
        char *c2=strtok(ctime(&t2),"\n"); // storing end time
        job->end_time=strdup(c2);
        
        if(!WIFEXITED(status)){
            job->status=3;
            job->print_status="Unsuccessful";
            printf("Error %d\n",WIFEXITED(status));
            perror("execvp");
            exit(-1);
        }

    }else{ // fork error
        perror("fork");
        exit(-1);
    }

    // updating the global running variable
    pthread_mutex_lock(&mutex);
    running--;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// function with infinite loop that checks the queue and runs the jobs on different threads.
void *run_all_jobs(void *arg){
    jobs *job;
    for(;;){
        pthread_mutex_lock(&mutex);
        if(running<P && job_queue->count>0){
            job=queue_delete(job_queue); // "dequeue-ing" the next job from the queue
            pthread_create(&job->tid, NULL, run_job, job);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1.5);
    }
    return NULL;
}


int main(int argc, char **argv) {
    pthread_t tid;
    char command[1024];
    
    if (argc != 2) {
        printf("Usage: %s <max # of threads that can run consecutively>\n",argv[0]);
        exit(-1);
    }
    P=atoi(argv[1]); // max concurrent threads value

    if(P==0)
        P=1;
    else if(P>8)
        P=8;   // setting max value of as 8

    job_queue=queue_init(1024);  // initializing the job_queue
    
    // creating a separate thread that continuously checks the job_queue and runs the jobs
    pthread_create(&tid, NULL, run_all_jobs, NULL);
    
    long number_of_jobs=0; // total number of jobs submitted by the user

    while(1){
        printf("\n                  COMMANDS                 \n");
        printf("| 1) Submit                               |\n");
        printf("| 2) Showjobs                             |\n");
        printf("| 3) Submithistory                        |\n");
        printf("| 4) Exit                                 |\n");
        printf("\nEnter command: ");

        fgets(command,sizeof(command),stdin); // getting input from user and storing in command.
        
        char* tokens[1024];
        char* tok=strdup(strtok(command,"\n")); // removing the end line character from command
        tok=strtok(tok," "); // getting first word in command

        char print_command[strlen(command)-strlen(tok)+1]; // creating a char[] that stores the command yo print
        for(int str=0;str<strlen(command);str++){
            print_command[str]=command[str+strlen(tok)+1];
        }
        
        int j=0; // tokenizing the command
        while(tok!=0){
            tokens[j++]=tok;
            tok=strtok(NULL," ");
        }

        if(strcmp(tokens[0],"submit")==0){ // checking if command inputed was 'submit'

            jobs job=add_job(number_of_jobs); // initializing the job

            job.tid=number_of_jobs; // thread id

            for(int k=0;k<j-1;k++)
                job.args[k]=strdup(tokens[k+1]);

            job.command=strdup(print_command); // command that will be printed

            job_array[number_of_jobs]=job; // job is added to global array
            
            queue_insert(job_queue,&job_array[number_of_jobs]); // job is added to queue
            printf("Job #%ld Added to queue\n\n",number_of_jobs);
            
            number_of_jobs++;
            
        }else if(strcmp(tokens[0],"showjobs")==0){

            printf("\n               SHOWJOBS              \n");
            printf("|JobID    Command             Status|\n");
            
            for(int T=0;T<number_of_jobs;T++){
                if(job_array[T].status<2){ // printing only waiting and running jobs
                    printf("|%ld         %s                  %s|\n",
                    job_array[T].job_id,job_array[T].command,job_array[T].print_status);
                }
            }

        }else if(strcmp(tokens[0],"submithistory")==0){
            printf("\n                                 SUBMITHISTORY                                      \n");
            printf("|JobID    Command            Start_time               End_time               Status|\n");
            
            for(int T=0;T<number_of_jobs;T++){ // printing all jobs and their details
                printf("|%ld        %s            %s              %s            %s|\n",
                job_array[T].job_id, job_array[T].command, job_array[T].start_time,
                job_array[T].end_time, job_array[T].print_status);
            }

        }else if(strcmp(tokens[0],"exit")==0)
            exit(-1);
        else
            printf("INVALID COMMAND!!\n");
    }
    return 0;

}