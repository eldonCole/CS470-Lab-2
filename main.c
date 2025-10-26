#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    const char *commands[10][3] = {
        // 2D array of command/argument pairs for 10 child processes
        // each inner array represetns argument list for execvp
        {"echo", "Hello Cole!", NULL}, //chile 1 prints greeting
        {"ls", NULL, NULL}, // child 2 lists directory contents
        {"pwd", NULL, NULL}, // child 3 prints working directory
        {"date", NULL, NULL}, // child 4 prints date
        {"whoami", NULL, NULL}, // child 5 prints current user
        {"uname", "-a", NULL}, // child 6 prints system info
        {"echo", "Process 7 running", NULL}, // child 7 prints messsage
        {"echo", "Learning fork() and exec()", NULL}, // for and exec message
        {"echo", "Child process demo", NULL}, // demo message
        {"echo", "All processes complete.", NULL} //process completion message
    };
    // print the pid of the parent process before forking
    printf("Parent PID: %d\n", getpid());
    //loop to create 10 child processes
    for (int i = 0; i < 10; i++) {
        pid_t pid = fork();                 // duplicate the current process
        if (pid < 0) {                      // negative return => fork failed
            perror("fork"); 
            exit(1);                        // terminate program on failure
        } else if (pid == 0) {              // child
            // Identify the child and the command it will execute
            printf("Child %d PID: %d executing %s\n",
                   i + 1, getpid(), commands[i][0]);
            execvp(commands[i][0], (char *const *)commands[i]);
            perror("execvp");                  // only runs if exec fails
            _exit(127);
        }
    }

    // parent waits for all children
    for (int i = 0; i < 10; i++) {
        int status;                         // stores child’s termination info
        pid_t finished = wait(&status);     // wait for one child to finish
        if (WIFEXITED(status))              // child exited normally
            printf("Parent: Child %d exited normally, status=%d\n",
                   finished, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))       // child was killed by a signal
            printf("Parent: Child %d killed by signal %d\n",
                   finished, WTERMSIG(status));
    }
    // All children have finished; print final confirmation
    printf("Parent process (%d) finished waiting for all children.\n", getpid());
    return 0;
}
