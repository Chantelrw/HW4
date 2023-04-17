# HW4

## Name: Chantel Rose Walia

Honor Code: 

I, Chantel Rose Walia, declare that I have completed this assignment completely and entirely on my own, without any unathorized consultation from others or unathorized access to online websites. I have read the UAB Academic Honor Code and understand that any breach of the UAB Academic Honor Code may result in severe penalties.

Student Signature/Initials: CRW

Date: 16 April 2023

## Details of code 

To Compile: type gcc -Wall -o hw4 hw4.c queue.c -lpthread
To Run: ./hw4 -max # of concurrent threads-
        then type submit, showjobs, submithistory or exit.

The code supports the submit, showjobs, submithistory and exit commands, and accordingly outputs to the "job_id.out" and "job_id.err" files.

I have also used pipes to communicate when a job is unsuccessful from child to parent process.
