#include <stdio.h>
#include <unistd.h>
#include "semlib.h"
 
int semInit(key_t key)
{
    int semid;
    // semaphore를 하나 만들고 id를 return
    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  {
		perror("semget");
        return -1;
    }
    return semid;
}
// 초기 값 지정 
int semInitValue(int semid, int value)
{
    union semun {
            int     val;
    } semun;
    semun.val = value;
    // SETVAL : 세마포어 값 지정
    if (semctl(semid, 0, SETVAL, semun) < 0)  {
		perror("semctl");
        return -1;
    }
    return semid;
}
 
int semWait(int semid)
{
    struct sembuf   semcmd;
    semcmd.sem_num = 0;
    // wait이므로 -1
    semcmd.sem_op = -1;
    // SEM_UNDO : 버그 등으로 프로세스가 죽었을 때 가지고 있는 세마포어를 반납
    semcmd.sem_flg = SEM_UNDO;
    // wait
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int semTryWait(int semid)
{
    struct sembuf semcmd;
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    // IPC_NOWAIT : 만약 0인 경우 wait하지 않고 false return 
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int semPost(int semid)
{
    struct sembuf   semcmd;
    semcmd.sem_num = 0;
    // 반납 +1
    semcmd.sem_op = 1;
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
    return 0;
}
// 현재 값을 가져온다
int semGetValue(int semid)
{
    union semun {
            int     val;
    } dummy;
    // GETVAL를 통해 세마포어 값을 가져옴
    return semctl(semid, 0, GETVAL, dummy);
}
// 세마포어 삭제 
int semDestroy(int semid)
{
    union semun {
            int     val;
    } dummy;
    // IPC_RMID로 지운다.
    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  {
		perror("semctl");
        return -1;
    }
    close(semid);
    return 0;
}
