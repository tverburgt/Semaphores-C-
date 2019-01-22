#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<string>
#include<sys/shm.h>
#include <iostream>
#define SEMKEY 10000
#define SHM_KEY 5000
using namespace std;

int main()
{
struct sembuf vsembuf, psembuf, v1sembuf, p1sembuf, v2sembuf, p2sembuf;
int semid;
int *ptr;

//1 represent no of semaphores we want.
semid = semget(SEMKEY,1,0777|IPC_CREAT); 
//Initialise semaphore 0 represents which semaphore in set
semctl(semid,0, SETVAL, 0);  //data
semctl(semid,1, SETVAL, 8);  //space
semctl(semid,2, SETVAL, 1);  //free

//Create an 8 byte block of shared memory.
int shmid = shmget(SHM_KEY, 9,0777|IPC_CREAT);

//ptr points to first location in mem.
ptr = (int*)shmat(shmid,0,0); 

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
ptr[0] = 2;
 
while(true)
{
semop(semid, &p1sembuf, 1); //wait(space)
semop(semid, &p2sembuf, 1); //wait(free)
	
cout <<"Enter a number (Position: " <<ptr[0]<<") -> ";
cin >> ptr[ptr[0]];

ptr[0] = ptr[0] + 1;  //The value of the pointer is incremented. 

	if(ptr[0] > 9)
	{
		ptr[0] = 2;
	}
semop(semid, &v2sembuf, 1); //signal(free)
semop(semid, &vsembuf, 1);  //signal(data)
}
}

