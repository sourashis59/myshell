#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include<sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("\nNumber of arguments must be 2, provided: %d\n", argc);
        return 1;
    } 
    char *firstCommand = argv[1];
    char *secondCommand = argv[2];
    printf("\npipe operator called, commands: %s, %s\n", firstCommand, secondCommand);
    
    int pipeFd[2];
    // Create pipe
    if (pipe(pipeFd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }    
    
    pid_t pid = fork();
    if (pid < 0) {
        perror("\nFork failed\n");
        exit(1);
    }

    //child process
    if (pid == 0) {
        printf("\nChild process created\n");
        // Redirect stdout to the pipe write end
        if (dup2(pipeFd[1], STDOUT_FILENO) == -1) {
            perror("\ndup2 error\n");
            exit(EXIT_FAILURE);
        }

        // Close the file descriptor as it's no longer needed
        // close(pipeFd[1]);


        // //execute command1
        // if (execv(firstCommand, NULL) == -1) {
        //     perror("\nexecv error\n");
        //     exit(1);
        // }
        execlp(firstCommand, firstCommand, NULL);

        //code will not run after this, because other process starts
    }

     // Fork the second child process
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("\nfork error\n");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        // Child process 2 (right side of pipe): Executes the second command
        // Close the write end of the pipe
        close(pipeFd[1]);
        // Redirect stdin to the read end of the pipe
        dup2(pipeFd[0], STDIN_FILENO);
        // Close the read end of the pipe
        close(pipeFd[0]);
        // Execute the second command (e.g., "wc -l")
        execlp(secondCommand, secondCommand, NULL);
        // If execlp fails
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    // Close both ends of the pipe in the parent process
    close(pipeFd[0]);
    close(pipeFd[1]);

    // Wait for both child processes to finish
    waitpid(pid, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}