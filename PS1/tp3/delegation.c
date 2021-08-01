/* bonjour, je m'exuse mon main est un gros pate
 * notamment les switchs a rallonge et les creations de mask
 * malheuresement je n ai pas eu le temps de m en occuper
 * cordialement, matthieu freitag */


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

// boolean that is true if we received a term signal in commandant
int is_term_signal = 0;

// var to count how many times it succeeded and failed
size_t count_c[2];
size_t count_l[2];

/********************************
 * Handle errors
 * *******************************/

noreturn void raler(char *message, int nb)
{
	perror(message);
	exit(nb);
}

/********************************
 * Handle signals
 * *******************************/

// edit the global variable containing the signal's id
void handle(int signum)
{
    sig = signum;
}

// edit the global variable containing a boolean (if sigterm is received)
void handle_term()
{
    is_term_signal = 1;
}

// set a function to a given signal
void set_sigaction(int signal, void (*function) (int signum))
{
    struct sigaction s;
    s.sa_handler = function;
    s.sa_flags = 0;

    if(sigemptyset(&s.sa_mask) == -1)
        raler("set sigemptyset", 8);

    if(sigaction(signal, &s, NULL) == -1)
        raler("set sigaction", 8);
}

/********************************
 * Sends a given signal at a given id
 * *******************************/

// send a signal to a given id
void send_to_commandant(int id, int signal)
{
    // reset global variable
    sig = 0;
    is_term_signal = 0;

    // send the signal
    if(kill(id, signal) == -1)
        raler("kill to commandant", 8);
}

// send a signal to a given id and kill's it if failed
void send_to_lieutenant(int id, int signal)
{
    // reset global variable
    sig = 0;
    is_term_signal = 0;

    // send the signal
    if(kill(id, signal) == -1)
    {
        // if failed : kill the pid
        kill(id, SIGKILL);
        raler("kill to lieutenant", 8);
    }
}

/********************************
 * End program
 * *******************************/

void end_lieutenant()
{
    // prints the result
    if(fprintf(stdout, "LIEUTENANT %zu %zu\n", count_l[0], count_l[1]) < 0)
        perror("fprintf lieutenant");
    exit(0);
}

void end_commandant(int pid, size_t end, int status, DIR *dir)
{

    // close : current directory
    if(closedir(dir) == -1)
        perror("close dir");

    // remove : ardoise
    if(unlink("ardoise") == -1)
        perror("unlink ardoise");

    // print : count commandant
    if(fprintf(stdout, "COMMANDANT %zu %zu\n", count_c[0], count_c[1]) < 0)
        perror("fprintf commandant");

    // send : SIGTERM to lieutenant
    send_to_lieutenant(pid, SIGTERM);

    // wait for the lieutenant to exit
    if (wait(&status))
        exit(end);
}

/********************************
 * Treat program
 * *******************************/

int treat_commandant(DIR* dir, struct dirent* dp, int fd, int pid, int status)
{
    // Ignore "." & ".."
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
        return -1;

    // Open or create ardoise
    if((fd = open("ardoise", O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        perror("open commandant");
        end_commandant(pid, 2, status, dir);
    }

    // Write ardoise
    if(write(fd, &(dp->d_name), strlen(dp->d_name)) == -1)
    {
        perror("write commandant");
        end_commandant(pid, 2, status, dir);
    }

    // Close ardoise
    if(close(fd) == -1)
    {
        perror("close commandant");
        end_commandant(pid, 2, status, dir);
    }

    return 0;
}

/********************************
 * Program
 * *******************************/

void delegation()
{

    // variables : commandant
    int pid;
    int status = 0;
    struct dirent* dp;

    // variables : lieutenant
    int fd = 0;
    ssize_t n = 0;
    char file[NAME_MAX];

    sigset_t old_mask;

    // masks : commandant

    sigset_t mask;
    if(sigemptyset(&mask) == -1)
        raler("sigemptyset", 8);
    if(sigaddset(&mask, SIGUSR1) == -1)
        raler("sigdelset", 8);
    if(sigaddset(&mask, SIGUSR2) == -1)
        raler("sigdelset", 8);
    if(sigaddset(&mask, SIGCHLD) == -1)
        raler("sigdelset", 8);

    // suspend : commandant 

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

    // suspend : lieutenant

    sigset_t susp_son;
    if(sigfillset(&susp_son) == -1)
        raler("sigfillset", 8);
    if(sigdelset(&susp_son, SIGUSR1) == -1)
        raler("sigdelset", 8);
    if(sigdelset(&susp_son, SIGTERM) == -1)
        raler("sigdelset", 8);

    // set action for signals
    set_sigaction(SIGUSR1, handle);

    DIR *dir;
    // open : current directory
    if((dir = opendir(".")) == NULL)
        raler("open dir", 7);

    switch(pid = fork())
    {

        case -1: // error
            perror("fork");
            break;

        case 0: // lieutenant

            // set action for signals
            set_sigaction(SIGTERM, handle);

            while(1)
            {

                // block : SIGUSR1 SIGTERM
                if(sigprocmask(SIG_BLOCK, &mask_son, &old_mask) == -1)
                    raler("sigprocmask", 8);

                switch(sig)
                {
                    case SIGUSR1: // OK signal from commandant
                        
                        // Open : ardoise
                        if((fd = open("ardoise", O_RDONLY)) == -1)
                        {
                            perror("open lieutenant");
                            send_to_commandant(getppid(), SIGUSR2);
                            count_l[1]++;
                            break;
                        }

                        // Read : ardoise
                        n = read(fd, &file, NAME_MAX); 

                        // Error : read
                        if (n >= NAME_MAX || n == -1)
                        {
                            perror("read lieutenant");
                            send_to_commandant(getppid(), SIGUSR2);
                            count_l[1]++;
                            break;
                        }
                        else
                        {
                            // add : end
                            file[n] = '\0';
                        }

                        // Close : ardoise
                        if (close(fd) == -1)
                        {
                            perror("close lieutenant");
                            send_to_commandant(getppid(), SIGUSR2);
                            count_l[1]++;
                            break;
                        } 

                        // Error : treat
                        if (traitement(file) == 0)
                        {
                            printf("traitement 0\n");
                            send_to_commandant(getppid(), SIGUSR2);
                            count_l[1]++;
                            break;
                        }

                        // End : success
                        send_to_commandant(getppid(), SIGUSR1);
                        count_l[0]++;

                        break;

                    case SIGTERM: // term signal from commandant
                        end_lieutenant();
                        break;

                    default: // if nothing received yet

                        // wait : SIGUSR1 SIGTERM
                        sigsuspend(&susp_son);

                        switch(sig)
                        {
                            case SIGUSR1: // OK signal from commandant
                                
                                // Open : ardoise
                                if((fd = open("ardoise", O_RDONLY)) == -1)
                                {
                                    perror("open lieutenant");
                                    send_to_commandant(getppid(), SIGUSR2);
                                    count_l[1]++;
                                    break;
                                }

                                // Read : ardoise
                                n = read(fd, &file, NAME_MAX); 

                                // Error : read
                                if (n >= NAME_MAX || n == -1)
                                {
                                    perror("read lieutenant");
                                    send_to_commandant(getppid(), SIGUSR2);
                                    count_l[1]++;
                                    break;
                                }
                                else
                                {
                                    // add : end
                                    file[n] = '\0';
                                }

                                // Close : ardoise
                                if (close(fd) == -1)
                                {
                                    perror("close lieutenant");
                                    send_to_commandant(getppid(), SIGUSR2);
                                    count_l[1]++;
                                    break;
                                } 

                                // Error : treat
                                if (traitement(file) == 0)
                                {
                                    send_to_commandant(getppid(), SIGUSR2);
                                    count_l[1]++;
                                    break;
                                }

                                // End : success
                                send_to_commandant(getppid(), SIGUSR1);
                                count_l[0]++;

                                break;

                            case SIGTERM: // term signal from commandant
                                end_lieutenant();
                                break;

                        }
                        break;
                }

                // reset at old mask
                if(sigprocmask(SIG_SETMASK, &old_mask, NULL) == -1)
                    raler("sigprocmask", 8);
            }
            break;

        default: // commandant

            // set action for signals
            set_sigaction(SIGUSR2, handle);
            set_sigaction(SIGCHLD, handle);
            set_sigaction(SIGTERM, handle_term);

            while ((dp = readdir(dir)) != NULL)
            {
                // treat commandant task, returns -1 if dp->name is . or ..
                if(treat_commandant(dir, dp, fd, pid, status) == -1)
                    continue;

                // Warn : lieutenant
                send_to_lieutenant(pid, SIGUSR1);

                // Nap
                sieste();

                // block : SIGUSR1 SIGUSR2 SIGTERM SIGCHLD
                if(sigprocmask(SIG_BLOCK, &mask, &old_mask) == -1)
                    raler("sigprocmask", 8);

                // react depending on the signals received
                if (is_term_signal)
                {
                    end_commandant(pid, 4, status, dir);
                }
                else if(sig == SIGUSR1)
                {
                    count_c[0]++;
                }
                else if (sig == SIGUSR2)
                {
                    count_c[1]++;
                }
                else if (sig == SIGCHLD)
                {
                    end_commandant(pid, 3, status, dir);
                }
                else {

                    sigsuspend(&susp);

                    if (is_term_signal == SIGTERM)
                    {
                        end_commandant(pid, 4, status, dir);
                    }
                    else if(sig == SIGUSR1)
                    {
                        count_c[0]++;
                    }
                    else if (sig == SIGUSR2)
                    {
                        count_c[1]++;
                    }
                    else if (sig == SIGCHLD)
                    {
                        end_commandant(pid, 3, status, dir);
                    }
                }

                // reset at old mask
                if(sigprocmask(SIG_SETMASK, &old_mask, NULL) == -1)
                    raler("sigprocmask", 8);

                // Reset errno at 0
                errno = 0;
            }

            // programm success
            end_commandant(pid, 0, status, dir);

    }
}

int main (int argc, char **argv)
{

    if (argc == 1)
        raler("sans argument", 6);
    
    if (argc > 2)
        raler("trop d'arguments", 6);

    if(configuration(argv[1]) == 0)
        raler("argument absurde", 6);

    delegation();

}