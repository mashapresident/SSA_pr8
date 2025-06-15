#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define num_forks 3
#define delay_seconds 1

int main() {
    printf("starting process: pid = %d, ppid = %d\n\n", getpid(), getppid());

    for (int i = 0; i < num_forks; ++i) {
        printf("pid %d (ppid %d): before fork() in iteration %d\n", getpid(), getppid(), i);
        
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork failed");
            return 1;
        } else if (pid == 0) {
            printf("child process created! pid = %d, ppid = %d (from parent %d) in iteration %d\n", 
                   getpid(), getppid(), getppid(), i);
        } else {
            printf("parent process (pid = %d, ppid = %d) created child with pid = %d in iteration %d\n", 
                   getpid(), getppid(), pid, i);
        }
        
        sleep(delay_seconds); 
    }

    printf("pid %d (ppid %d): loop finished. doing final task...\n", getpid(), getppid());

    printf("pid %d (ppid %d): exiting.\n", getpid(), getppid());
    return 0;
}
