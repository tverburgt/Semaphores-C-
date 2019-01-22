#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include <iostream>
#define SEMKEY 10000//152
#define SHM_KEY 5000//9852
using namespace std;


main()
{

struct sembuf vsembuf, psembuf, v1sembuf, p1sembuf, v2sembuf, p2sembuf;
int semid, i;
int *ptr1;
int shimid;
//1 represent no of semaphores we want.
semid = semget(SEMKEY,1,0777|IPC_CREAT); 
int shmid = shmget(SHM_KEY, 9,0777|IPC_CREAT);
ptr1 = (int*)shmat(shmid,0,0); //ptr points to first location in mem.


/************************SEMAPHORES***********************/
//This is the wait 0 semaphore.
psembuf.sem_num= 0;           //(wait)                   //Data.
psembuf.sem_op = -1;
psembuf.sem_flg = SEM_UNDO;

//This is the signal 0 semaphore.
vsembuf.sem_num = 0;        //        (signal)
vsembuf.sem_op = 1;
vsembuf.sem_flg = SEM_UNDO;

//This is the wait 1 semaphore.
p1sembuf.sem_num= 1;                                     //Space.
p1sembuf.sem_op = -1;
p1sembuf.sem_flg = SEM_UNDO;

//This is the signal 1 semaphore.
v1sembuf.sem_num = 1;
v1sembuf.sem_op = 1;
v1sembuf.sem_flg = SEM_UNDO;

//This is the wait 2 semaphore.
p2sembuf.sem_num= 2;                                      //Free.
p2sembuf.sem_op = -1;
p2sembuf.sem_flg = SEM_UNDO;

//This is the signal 2 semaphore.
v2sembuf.sem_num = 2;
v2sembuf.sem_op = 1;
v2sembuf.sem_flg = SEM_UNDO;
/*************************SEMAPHORES***********************/

ptr1[1] = 2;
int inc = 2;

while(1)
{
semop(semid, &psembuf, 1);  //wait(data)
semop(semid, &p2sembuf, 1); //wait(free)

cout <<"Number (Position: " <<ptr1[1]<<") -> "<< ptr1[ptr1[1]] << endl;


ptr1[1] = ptr1[1] + 1;   //The value of the pointer is incremented. 

	if(ptr1[1] > 9)
	{
		ptr1[1] =2;
	}




/*
	if(inc < 8)
	{	
		inc++;
		ptr1[1] = inc;
		
	}
	else
	{	
		ptr1[1] = 2;	
		inc = 2;
	}
*/
semop(semid, &v2sembuf, 1); //signal(free)
semop(semid, &v1sembuf, 1); //signal(space) 
}
}
