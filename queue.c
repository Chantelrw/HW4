/*Honor Code:
I, Chantel Rose Walia, declare that I have completed this assignment completely and 
entirely on my own, without any unathorized consultation from others or unathorized access 
to online websites. I have read the UAB Academic Honor Code and understand that any breach of 
the UAB Academic Honor Code may result in severe penalties.
Student Signature/Initials: CRW
Date: 16 April 2023
*/

/* Implementation of a simple circular queue using a static array */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/* create the queue data structure and initialize it */
queue *queue_init(int n) {
	queue *q = (queue *)malloc(sizeof(queue));
	q->size = n;
	q->buffer = malloc(sizeof(jobs*)*n);
	q->start = 0;
	q->end = 0;
	q->count = 0;
	return q;
}

/* insert an item into the queue, update the pointers and count, and
   return the no. of items in the queue (-1 if queue is null or full) */
int queue_insert(queue *q, jobs *job) {
	if ((q == NULL) || (q->count == q->size))
	   return -1;

	q->buffer[q->end % q->size]=job;	
	q->end = (q->end + 1) % q->size;
	q->count++;

	return q->count;
}

/* delete an item from the queue, update the pointers and count, and 
   return the item deleted (-1 if queue is null or empty) */
jobs *queue_delete(queue *q) {
	if ((q == NULL) || (q->count == 0))
	   return (jobs *)-1;

	jobs *job = q->buffer[q->start];
	q->start = (q->start + 1) % q->size;
	q->count--;

	return job;
}

// function to initialize the variables in a job.
jobs add_job(int job_id){
	jobs job;
	job.job_id=job_id;
	job.status=0;
	job.start_time="-";
	job.end_time="-";
	job.print_status="Waiting";
	sprintf(job.fout,"%d.out",job_id);
	sprintf(job.ferr,"%d.err",job_id);
	return job;
}

//-----Below functions were pre-given in queue.c but haven't been used in my implementation------------
/* display the contents of the queue data structure */
void queue_display(queue *q) {
	int i;
	if (q != NULL && q->count != 0) {
		printf("queue has %d elements, start = %d, end = %d\n", 
			q->count, q->start, q->end);
		printf("queue contents: ");
		for (i = 0; i < q->count; i++)
	    		//printf("%d ", q->buffer[(q->start + i) % q->size]);
		printf("\n");
	} else
		printf("queue empty, nothing to display\n");
}

/* delete the queue data structure */
void queue_destroy(queue *q) {
	free(q->buffer);
	free(q);
}