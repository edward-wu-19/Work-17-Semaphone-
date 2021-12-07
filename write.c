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

// struct sembuf{
//     short sem_op;
//     short sem_num;
//     short sem_flag;
// }

int main(){
    int semd;
    int shmd;
    int* previous;
    char input[1024];

    semd = semget(KEY, 0, 0);
    shmd = shmget(SHMKEY, 0, 0);

    struct sembuf sb;
    sb.sem_op = -1; // Down
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    semop(semd, &sb, 1);

    previous = shmat(shmd, 0, 0);
    char line[1024];

    if (*previous != -1){ // case where this is the first line
        int fd = open("story.txt", O_RDONLY);
        lseek(fd, -(*previous), SEEK_END);

        int foo = read(fd, line, *previous);
        if (foo == -1){
            printf("Error: %s\n", strerror(errno));
            return 0;
        }
        printf("Previous line: %s\n", line);

        printf("Please enter the next line: \n");
    }
    else{
        printf("Please enter the first line: \n");
    }

    fgets(line, 1024, stdin);
    *previous = strlen(line);

    int goo = open("story.txt", O_WRONLY | O_APPEND);
    if (goo == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }
    int err = write(goo, line, strlen(line));
    if (err == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    shmdt(previous);

    sb.sem_op = 1; // Up
    semop(semd, &sb, 1);

    return 0;
}