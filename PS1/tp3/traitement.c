
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <errno.h>
#include "traitement.h"

#define SECONDS_PER_WEEK 7*24*60*60

static struct timespec duration = { SECONDS_PER_WEEK, 0 };
static unsigned int crash_after = 0;
static unsigned int call_number = 0;

int configuration (const char * config)
{
    srand (time (NULL));
    if (strcmp (config, "+slow") == 0)
    {
        duration = (struct timespec) { 0, 500*1000*1000 };
        crash_after = 0;
    }
    else if (strcmp (config, "-slow") == 0)
    {
        duration = (struct timespec) { 0, 500*1000*1000 };
        crash_after = 1 + (rand() % 10);
    }
    else if (strcmp (config, "+fast") == 0)
    {
        duration = (struct timespec) { 0, 10*1000*1000 };
        crash_after = 0;
    }
    else if (strcmp (config, "-fast") == 0)
    {
        duration = (struct timespec) { 0, 10*1000*1000 };
        crash_after = 1 + (rand() % 10);
    }
    else
    {
        long int d;
        unsigned int c;
        if (sscanf (config, "%lu:%u", &d, &c) != 2)
            return 0;
        duration = (struct timespec) { (d/1000), 1000000L*(d%1000) };
        crash_after = (c == 0 ? 0 : 1 + (rand() % c));
    }
    call_number = 0;
    return 1;
}

static void somesleep(int variation)
{
    /* Faire quelque chose, pendant un certain temps */
    struct timespec req = duration;
    struct timespec rem;
    int ret;
    if (variation > 0)
    {
        long t = 1000000000L*duration.tv_sec + duration.tv_nsec;
        long v = ( rand() % (2*variation) ) - variation;
        t += t*v/100;
        req.tv_sec = t/1000000000L;
        req.tv_nsec = t%1000000000L;
    }
    while ((ret=nanosleep (&req, &rem)) != 0 && errno == EINTR)
        req = rem;
    if (ret == -1)
        /* On ignore les erreurs de nanosleep ici */
        {/*perror ("attente malheureusement raccourcie");*/}
}

int traitement (const char * name)
{
    ++ call_number;

    struct stat stat;
    int existant = (lstat (name, &stat) != -1);
    if (! existant)
        perror ("ATTENTION, traitement");

    /* Faut-il s'arêter là ? */
    if (crash_after != 0 && call_number == crash_after)
    {
        /* abort(), en essayant d'éviter la création d'un fichier core */
        struct rlimit climit;
        if (getrlimit (RLIMIT_CORE, &climit) == 0)
        {
            climit.rlim_cur = 0;
            (void) setrlimit (RLIMIT_CORE, &climit);
            /* Si un de ces appels a échoué... on a fait ce qu'on a pu. */
        }
        abort();
    }

    /* La traitement lui-même */
    somesleep (0);

    return existant && (rand() % 4) < 3;
}

void sieste ()
{
    somesleep (20);
}
