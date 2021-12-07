#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define KEY 24601
#define SHMKEY 24602

union semun {
  int val;                  //used for SETVAL
  struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
  unsigned short  *array;   //used for SETALL
  struct seminfo  *__buf;
};

int main(int argc, char* argv[]){
    if (argc < 2){
        printf("Unexpected number of arguments: Please indicate whether you would like to create (-c) or remove (-r) the resources\n");
    }
    else{
        int semd, shmd;
        if (strcmp(argv[1], "-c") == 0){
            shmd = shmget(SHMKEY, sizeof(int), IPC_CREAT | 0640);
            semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
            union semun us;
            us.val = 1;
            int r = semctl(semd, 0, SETVAL, us);

            int foo = open("story.txt", O_TRUNC | O_CREAT, 0644);
            if (foo == -1){
                printf("Error: %s\n", strerror(errno));
                return 0;
            }

            int *first;
            first = shmat(shmd, 0, 0);
            *first = -1;

            shmdt(first);
        }
        else if(strcmp(argv[1], "-r") == 0){
            semctl(semd, IPC_RMID, 0);

            int foo = open("story.txt", O_RDONLY);
            if (foo == -1){
                printf("Error: %s\n", strerror(errno));
                return 0;
            }

            struct stat st;
            stat("story.txt", &st);
            char *buff = malloc(st.st_size);

            int goo = read(foo, buff, st.st_size);
            if (goo == -1){
                printf("Error: %s\n", strerror(errno));
                return 0;
            }

            printf("Story:\n======================\n");
            printf("%s", buff);
            printf("======================\nHope you enjoyed it!\n");
            free(buff);

            shmctl(shmd, IPC_RMID, 0);
            semctl(semd, 0, IPC_RMID, 0);
        }
        else{
            printf("Unexpected argument received: Please indicate whether you would like to create (-c) or remove (-r) the resources\n");
        }
    }

    return 0;
}