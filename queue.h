/*Honor Code:
I, Chantel Rose Walia, declare that I have completed this assignment completely and 
entirely on my own, without any unathorized consultation from others or unathorized access 
to online websites. I have read the UAB Academic Honor Code and understand that any breach of 
the UAB Academic Honor Code may result in severe penalties.
Student Signature/Initials: CRW
Date: 16 April 2023
*/

/* Header file for the simple circular queue example */
#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <pthread.h>

typedef struct jobs{
	long job_id; // job id
	pthread_t tid; // thread id
	char* args[1024]; // execvp args
	char* command; // print args
	char* start_time; // job start time
	char* end_time; // job end time
	int status;  //0-> Waiting, 1-> Running, 2->Finished, 3->Error
	char *print_status; // string equivalent of int status for printing
	char fout[20]; // output filename
	char ferr[20]; // error filename
}jobs;

typedef struct _queue {
	int size;    /* maximum size of the queue */
	jobs **buffer; /* queue buffer */
	int start;   /* index to the start of the queue */
	int end;     /* index to the end of the queue */
	int count;   /* no. of elements in the queue */
} queue;

queue *queue_init(int n);
int queue_insert(queue *q, jobs *job);
jobs  *queue_delete(queue *q);
void queue_display(queue *q);
void queue_destroy(queue *q);

jobs add_job(int job_id);

#endif
