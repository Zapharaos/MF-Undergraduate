/* bonjour, je n'ai pas vraiment compris le script de test :
 * 
 * 
 *
*/



#include <stdnoreturn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <sys/types.h>

#define N_MAX 20
#define BUF_SIZE 10
#define FILE "huissier_fifo"

#define MOD(x, N) ((x % N + N) % N)
#define RAND(max) (rand() + 1) % max

/********************************
 * Handle errors
 * *******************************/
noreturn void raler(char *message)
{
	perror(message);
	exit(1);
}

/********************************
 * Cast a string into an int
 * *******************************/
int string_to_int(char *arg)
{
    // variables
    char *endptr, *str;
    str = arg;

    errno = 0;
    long N = strtol(str, &endptr, 10);

    // check : error
    if ((errno == ERANGE && (N == LONG_MAX || N == LONG_MIN))
            || (errno != 0 && N == 0))
    {
        raler("strtol");
    }

    // if : not found
    if (endptr == str)
        raler("string_to_int nothing found");

    // cast to int (use of signed values later)
    return (int) N;
}

/********************************
 * Returns a value from a pipe
 * *******************************/
int read_from_pipe(int (*pipe)[2], int index)
{
    // variables : buffer
    int r;
    char buffer[BUF_SIZE];

    // read : pipe
    r = read(pipe[index][0], &buffer, BUF_SIZE);

    // check : error
    if (r >= BUF_SIZE || r == -1)
        raler("read_from_pipe");
    else
        buffer[r] = '\0';

    // cast : string to int
    return string_to_int(buffer);
}

/********************************
 * Sends a value to a pipe
 * *******************************/
void write_to_pipe(int (*pipe)[2], int index, int value)
{
    // variables : buffer
    int r;
    char buffer[BUF_SIZE];

    // cast : int to string
    if((r = snprintf(buffer, BUF_SIZE, "%d", value)) >= BUF_SIZE || r < 0)
        raler("snprintf write_to_pipe");

    // write : pipe
    if(write(pipe[index][1], &buffer, strlen(buffer)) == -1)
        raler("write write_to_pipe");
}

void write_way_value(int (*way)[2], int (*child)[2], int (*parent)[2],
    bool is_forward, int value, int prev, int next)
{
    // write : way (depending on the way)
    if(is_forward) 
        write_to_pipe(way, next, is_forward);
    else
        write_to_pipe(way, prev, is_forward);

    // write : value (depending on the way)
    if(is_forward)
        write_to_pipe(child, next, value);
    else
        write_to_pipe(parent, prev, value);
}

void close_fulle_pipe(int (*pipe)[2], int N)
{
    for(int i = 0; i < N; i++)
    { 
        if(close(pipe[i][0]) == -1)
            raler("close_pipe i 0");
        if(close(pipe[i][1]) == -1)
            raler("close_pipe i 1");
    }
}

/********************************
 * Close unnecessary pipes in an array
 * *******************************/
void close_pipe(int (*pipe)[2], int N, int index, int first, int second)
{
    // close : unnecessary pipe
    for(int i = 0; i < N; i++)
    {
        if(i == index || i == first || i == second)
            continue;
        
        if(close(pipe[i][0]) == -1)
            raler("close_pipe i 0");
        if(close(pipe[i][1]) == -1)
            raler("close_pipe i 1");
    }

    // close : unused part of pipes needed
    if(close(pipe[index][1]) == -1)
        raler("close_pipe index");
    if(close(pipe[first][0]) == -1)
        raler("close_pipe first");

    // if : second pipe needed
    if(second != first)
    {
        if(close(pipe[second][0]) == -1)
            raler("close_pipe second");
    }
}

/********************************
 * Close pipe after using it
 * *******************************/
void end_pipe(int (*pipe)[2], int index, int first, int second)
{
    // close : previously needed part of pipes
    if(close(pipe[index][0]) == -1)
        raler("end_pipe index");
    if(close(pipe[first][1]) == -1)
        raler("end_pipe first");

    // if : second pipe needed
    if(second != first)
    {
        if(close(pipe[second][1]) == -1)
            raler("end_pipe second");
    }
}

/********************************
 * Sends a message to "FILE"
 * *******************************/
void handle_winner(int index)
{
    int r;
    int fd;
    char buffer[BUF_SIZE];

    // get : sentence with the index
    if((r = snprintf(buffer, BUF_SIZE, "%d won !\n", index)) >= BUF_SIZE || r < 0)
    {
        unlink(FILE);
        raler("snprintf winner in choisir");
    }

    // open : pipe
    if((fd = open(FILE, O_WRONLY)) == -1)
    {
        unlink(FILE);
        raler("open fifo in choisir");
    }

    // write : pipe
    if(write(fd, &buffer, strlen(buffer)) == -1)
    {
        close(fd);
        unlink(FILE);
        raler("write fifo in choisir");
    }

    // close : pipe
    if(close(fd) == -1)
    {
        unlink(FILE);
        raler("close fifo in choisir");
    }

    // print : winner
    if(fprintf(stdout, "%d won !\n", index) < 0)
    {
        unlink(FILE);
        raler("fprintf winner in choisir");
    }

    // remove : pipe
    if(unlink(FILE) == -1)
        raler("unlink fifo in choisir");
}

/********************************
 * Sends a message using pipes 
 * *******************************/
void treat_process(int N, int index, int (*child)[2], int (*parent)[2], int (*way)[2])
{
    // values
    int value;
    int last_value = N_MAX;

    // indexes
    int next = MOD(index+1, N);
    int prev = MOD(index-1, N);

    // booleans
    bool loop = true;
    bool is_alive = true;
    bool is_forward = true;

    // close : unnecessary part of pipes
    close_pipe(child, N, index, next, next);
    close_pipe(parent, N, index, prev, prev);
    close_pipe(way, N, index, prev, next);

    while(loop)
    {

        // get : way (i.e child to parent or parent to child)
        is_forward = read_from_pipe(way, index);

        // get : value
        if(is_forward == 1) // child to parent
            value = read_from_pipe(child, index);
        else if(is_forward == 0) // parent to child
            value = read_from_pipe(parent, index);

        // if : process is already eliminated
        if(is_alive == false)
        {

            // if : end of election value
            if((is_forward && getpid() == value - 1)
                || (!is_forward && getpid() == value + 1))
            {
                loop = false;
                continue;
            }

            // sends : way and value to the next element in the ring
            write_way_value(way, child, parent, is_forward, value, prev, next);

            // if : end of election value
            if(value > N_MAX)
                loop = false;

            continue;
        }

        // else if : winner
        if(value == last_value)
        {
            // use of named pipe
            handle_winner(index);

            // sends : way and "end of election" value to other process in the ring
            write_way_value(way, child, parent, is_forward, getpid(), prev, next);

            // exit the loop
            loop = false;
            continue;
        }

        // else : update value

        switch(value)
        {
            case -1: // previous process is out
                // swap way and new value
                is_forward = !is_forward;
                value = RAND(N_MAX);
                break;
            case 0: // eliminated
                value = -1;
                is_alive = false;
                break;
            default: // update : value
                value--;
                break;
        }

        // update : last value
        last_value = value;

        // sends : way and value to the next element in the ring
        write_way_value(way, child, parent, is_forward, value, prev, next);
    }

    // close : last part of pipes
    end_pipe(child, index, next, next);
    end_pipe(parent, index, prev, prev);
    end_pipe(way, index, prev, next);
}

/********************************
 * Choose a process 
 * *******************************/
void choose(int N)
{
    // arrays for pipes
    int child[N_MAX][2];
    int parent[N_MAX][2];
    int way[N_MAX][2];

    // init : pipes
    for(int i = 0; i < N; i++)
    {
        if(pipe(child[i]) == -1)
        {
            close_fulle_pipe(child, i);
            raler("pipe child[i]");
        }
        if(pipe(parent[i]) == -1)
        {
            close_fulle_pipe(parent, i);
            raler("pipe parent[i]");
        }
        if(pipe(way[i]) == -1)
        {
            close_fulle_pipe(way, i);
            raler("pipe way[i]");
        }
    }

    // random index
    srand(time(NULL));
    int value = RAND(N_MAX);

    // init : way from child to parent
    write_way_value(way, child, parent, true, value, -1, 0);
    
    pid_t pid;

    // creates a ring of N processes
    for(int index = 0; index < N; index++)
    {
        // if : error
        if((pid = fork()) == -1)
            raler("fork N");

        // if : son
        if(pid == 0)
        {
            treat_process(N, index, child, parent, way);
            exit(0);
        }
    }

    // wait : for all process to end
    for (int i = 0; i < N; i++) {
        if(wait(NULL) == -1)
            raler("wait N");
    }
}

/********************************
 * Main program
 * *******************************/
int main (int argc, char **argv)
{
    // if : arguments not compatible

    if (argc == 1)
    {
        fprintf(stderr, "Argument manquant\nUsage: ./choisir <N>\n");
        exit(1);
    }

    if (argc > 2)
    {
        fprintf(stderr, "Trop d'arguments\nUsage: ./choisir <N>\n");
        exit(1);
    }

    // else

    choose(string_to_int(argv[1]));

    return 0;
}