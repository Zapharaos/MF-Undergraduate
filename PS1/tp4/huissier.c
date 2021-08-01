#include <stdnoreturn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 10
#define FILE "huissier_fifo"

/********************************
 * Handle errors
 * *******************************/
noreturn void raler(char *message)
{
	perror(message);
	exit(1);
}

/********************************
 * Main program
 * *******************************/
int main (int argc, char **argv)
{
    (void) argv;

    // check : arguments
    if (argc > 2)
    {
        fprintf(stderr, "Trop d'arguments\nUsage: ./huissier\n");
        exit(1);
    }

    // buffer
    int r;
    char buffer[BUF_SIZE];

    errno = 0;

    // create named pipe
    if(mkfifo(FILE, 0666) == -1)
        raler("mkfifo huissier");
    
    // open : pipe
    int fd;
    if((fd = open(FILE, O_RDONLY)) == -1)
        raler("open huissier");

    // write : pipe
    r = read(fd, &buffer, BUF_SIZE);

    // check : result after read
    if (r >= BUF_SIZE || r == -1)
    {
        close(fd);
        raler("read huissier");
    }
    else
    {
        buffer[r] = '\0';
    }

    // close : pipe
    if(close(fd) == -1)
    {
        raler("close huissier");
    }

    if(fprintf(stdout, "%s", buffer) < 0)
    {
        raler("fprintf huissier");
    }

    return 0;
}