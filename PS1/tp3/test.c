#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdnoreturn.h>
#include <errno.h>
#include <sys/wait.h>
#include <libgen.h>
#include <fcntl.h>

#include "traitement.h"

// var to store number of a signal 
volatile sig_atomic_t sig = 0;

size_t count_lieutenant = 0;
size_t count_commandant = 0;

/********************************
 * Fonctions raler
 * *******************************/
noreturn void raler(char *message, int nb){
	perror(message);
	exit(nb);
}

/********************************
 * Fonction de sigaction
 * *******************************/
void handle(int signum){
    sig = signum;
}

void set_sigaction(int signal, void (*function) (int signum)){
    struct sigaction s;
    s.sa_handler = function;
    s.sa_flags = 0;
    if(sigemptyset(&s.sa_mask) == -1)
        raler("set sigemptyset", 8);
    if(sigaction(signal, &s, NULL) == -1)
        raler("set sigaction", 8);
}

void send_to_commandant(int id, int signal){
    if(kill(id, signal) == -1)
        raler("kill to commandant", 8);
}

void send_to_lieutenant(int id, int signal){
    if(kill(id, signal) == -1) {
        kill(id, SIGKILL);
        raler("kill to lieutenant", 8);
    }
}

/********************************
 * Fin du programme
 * *******************************/
void end(int pid, size_t end, int status, DIR *dir){

    if(closedir(dir) == -1)
        perror("close dir");

    if(unlink("ardoise") == -1)
        perror("unlink ardoise");

    if(fprintf(stdout, "COMMANDANT %zu \n", count_commandant) < 0)
        perror("fprintf commandant");

    send_to_lieutenant(pid, SIGTERM);

    if (wait(&status))
        exit(end);
}

/********************************
 * Fonction de lieutenant
 * *******************************/
void lieutenant(int fd, ssize_t n, char* file){
    // Open : ardoise
    if((fd = open("ardoise", O_RDONLY)) == -1) {
        perror("open lieutenant");
        send_to_commandant(getppid(), SIGUSR2);
        count_lieutenant++;
        return;
    }

    // Read : ardoise
    n = read(fd, &file, NAME_MAX);

    // Error : read
    if (n >= NAME_MAX || n == -1) {
        perror("read lieutenant");
        send_to_commandant(getppid(), SIGUSR2);
        count_lieutenant++;
        return;
    } else { // add : end
        file[n] = '\0';
    }

    // Close : ardoise
    if (close(fd) == -1) {
        perror("close lieutenant");
        send_to_commandant(getppid(), SIGUSR2);
        count_lieutenant++;
        return;
    } 

    // Error : treat
    if (traitement(file) == 0){
        send_to_commandant(getppid(), SIGUSR2);
        count_lieutenant++;
        return;
    }

    // End : success
    send_to_commandant(getppid(), SIGUSR1);
    count_lieutenant++;
}

int main (int argc, char **argv){

    if (argc == 1)
        raler("sans argument", 6);
    
    if (argc > 2)
        raler("trop d'arguments", 6);

    if(configuration(argv[1]) == 0)
        raler("argument absurde", 6);

    DIR *dir;
    // open : current directory
    if((dir = opendir(".")) == NULL)
        raler("open dir", 7);

    // set action for signals
    set_sigaction(SIGUSR1, handle);
    set_sigaction(SIGTERM, handle);

    // variables : commandant
    int pid;
    int status;
    struct dirent* dp;

    // variables : lieutenant
    int fd;
    char file[NAME_MAX];

    // masks : global
    sigset_t old_mask;

    // masks : commandant

    sigset_t mask;
    if(sigemptyset(&mask) == -1)
        raler("sigemptyset", 8);
    if(sigaddset(&mask, SIGUSR1) == -1)
        raler("sigdelset", 8);
    if(sigaddset(&mask, SIGUSR2) == -1)
        raler("sigdelset", 8);
    if(sigaddset(&mask, SIGTERM) == -1)
        raler("sigdelset", 8);
    if(sigaddset(&mask, SIGCHLD) == -1)
        raler("sigdelset", 8);

    sigset_t susp;
    if(sigfillset(&susp) == -1)
        raler("sigfillset", 8);
    if(sigdelset(&susp, SIGUSR1) == -1)
        raler("sigdelset", 8);
    if(sigdelset(&susp, SIGUSR2) == -1)
        raler("sigdelset", 8);
    if(sigdelset(&susp, SIGTERM) == -1)
        raler("sigdelset", 8);
    if(sigdelset(&susp, SIGCHLD) == -1)
        raler("sigdelset", 8);

    // masks : lieutenant
    
    sigset_t mask_son;
    if(sigemptyset(&mask_son) == -1)
        raler("sigemptyset", 8);
    if(sigaddset(&mask_son, SIGUSR1) == -1)
        raler("sigdelset", 8);
    if(sigaddset(&mask_son, SIGTERM) == -1)
        raler("sigdelset", 8);

    sigset_t susp_son;
    if(sigfillset(&susp_son) == -1)
        raler("sigfillset", 8);
    if(sigdelset(&susp_son, SIGUSR1) == -1)
        raler("sigdelset", 8);
    if(sigdelset(&susp_son, SIGTERM) == -1)
        raler("sigdelset", 8);

    switch(pid = fork()){

        case -1: // error
            perror("fork");
            break;

        case 0: // lieutenant

            while(1) {

                if(sigprocmask(SIG_BLOCK, &mask_son, &old_mask) == -1)
                    raler("sigprocmask", 8);

                switch(sig)
                {
                    case SIGUSR1:
                        lieutenant(fd, 0, file);
                        break;
                    case SIGTERM:
                        exit(0);
                    default:
                        if(sigsuspend(&susp_son) == -1)
                            raler("sigsuspend", 8);
                        switch(sig)
                        {
                            case SIGUSR1:
                                lieutenant(fd, 0, file);
                                break;
                            case SIGTERM:
                                exit(0);
                        }
                        break;
                }

                if(sigprocmask(SIG_SETMASK, &old_mask, NULL) == -1)
                    raler("sigprocmask", 8);
            }
            break;

        default: // commandant

            set_sigaction(SIGUSR2, handle);
            set_sigaction(SIGCHLD, handle);

            while ((dp = readdir(dir)) != NULL){

                // Ignore "." & ".."
                if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
                    continue;

                // Open or create ardoise
                if((fd = open("ardoise", O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1){
                    perror("open commandant");
                    end(pid, 2, status, dir);
                }

                // Write ardoise
                if(write(fd, &(dp->d_name), strlen(dp->d_name)) == -1){
                    perror("write commandant");
                    end(pid, 2, status, dir);
                }

                // Close ardoise
                if(close(fd) == -1){
                    perror("close commandant");
                    end(pid, 2, status, dir);
                }

                // Warn : lieutenant
                send_to_lieutenant(pid, SIGUSR1);

                // Nap
                sieste();

                if(sigprocmask(SIG_BLOCK, &mask, &old_mask) == -1)
                    raler("sigprocmask", 8);

                // handle signal
                switch(sig){
                    case SIGUSR1: // lieutenant success
                        count_commandant++;
                        break;
                    case SIGUSR2: // lieutenant issue
                        count_commandant++;
                        break;
                    case SIGTERM: // end of program
                        end(pid, 4, status, dir);
                        break;
                    case SIGCHLD:
                        end(pid, 3, status, dir);
                        break;
                    default:
                        if(sigsuspend(&susp) == -1)
                            raler("sigsuspend", 8);
                        switch(sig){
                            case SIGUSR1: // lieutenant success
                                count_commandant++;
                                break;
                            case SIGUSR2: // lieutenant issue
                                count_commandant++;
                                break;
                            case SIGTERM: // end of program
                                end(pid, 4, status, dir);
                                break;
                            case SIGCHLD:
                                end(pid, 3, status, dir);
                                break;
                        }
                        break;
                }

                if(sigprocmask(SIG_SETMASK, &old_mask, NULL) == -1)
                    raler("sigprocmask", 8);

                // Reset errno at 0
                errno = 0;
            }
            // programm success
            end(pid, 0, status, dir);
    }

    if(closedir(dir) == -1)
        perror("close dir");
}