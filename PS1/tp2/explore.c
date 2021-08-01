#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdnoreturn.h>
#include <errno.h>
#include <sys/wait.h>
#include <libgen.h>


#define SIZE 1024
#define FAILS 255

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

noreturn void raler(char *message){
    perror(message);
    exit(FAILS);
}

// calcul des hauteurs

int attendre_fils (int nb){
    int i, status, tmp, ret = 0;
    //attente des codes de retour de tous les fils
    for (i = 0 ; i < nb ; i++) {
        if(wait(&status)==-1){
            raler("wait");
        }

        if (WIFEXITED(status) && (tmp = WEXITSTATUS(status)) < FAILS) {
            //choisir le maximum
            ret = MAX(ret,tmp);
        }
        else {
            return (FAILS);
        }
    }

    return ret+1;
}


noreturn void repertoire (char *repertoire,  char *executable){
    char exec[SIZE];
    int n;
    
    // construction du chemin vers l'executable à partir du répertoire du processus fils
    if ((n= snprintf(exec, SIZE, "../%s", executable)) >= SIZE || n<0) {
        fprintf(stderr, "erreur snprintf\n");
        exit(FAILS);;
    }
    
    execl(exec, exec, repertoire, NULL);
    raler("execl");
}
    

void explore (char *dirpath, char *executable){
    DIR *dir;
    int n, counter, position;
    struct dirent *dp;
    char *rep;
    char copy[SIZE];
    char elt[SIZE];
    
    counter =0;
    //copie de dirpath pour l'utiliser dans basename 
    
    if ((n=snprintf(copy, SIZE, "%s", dirpath)) >= SIZE || n<0){
        fprintf(stderr, "erreur snprintf\n");
        exit(FAILS);
    }
    
    //récupération du dernier niveau du dirpath
    
    rep = basename(copy);
    
    if((dir=opendir(rep))==NULL){
        //vérifier si rep est un fichier
        if(errno == ENOTDIR){
            fprintf(stdout, "%s est une feuille\n", dirpath);
            exit(0);
        }
        
        else{
            raler("opendir");
        }
    }

    
    while ((dp =readdir(dir))!=NULL){
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
            continue;
        }
        
        //construction du chemin pour l'affichage
        if ((n=snprintf(elt, SIZE, "%s/%s", dirpath, dp->d_name)) >= SIZE || n<0){
            fprintf(stderr, "erreur snprintf");
            exit(FAILS);
        }
        
        switch (fork()) {
            case -1:
                raler("fork");
            case 0:

                if(closedir(dir)==-1){
                    raler("closedir");
                }
                //changement du répertoire courant du processus fils
                if(chdir(rep)==-1){
                    fprintf(stderr, "dirname: %s\n", rep);
                    raler("chdir");
                }
        
                repertoire(elt, executable);
        }
        // incrémentation du nombre de fils
        counter ++;
        
    }
    
    
    position = attendre_fils(counter);

   
    if(closedir(dir)==-1){
        raler("closedir");
    }
    
    if (position < FAILS){
        fprintf(stdout, "%s est à la hauteur %d\n", dirpath, position);
        exit(position);
    }
    else{
        fprintf (stderr, "%s hauteur max atteinte\n", dirpath);
        exit (FAILS);
    }
    
}


int main(int argc, char **argv){
    
    if (argc == 1) {
        fprintf(stderr, "Argument manquant\n");
        fprintf(stderr, "Usage: %s <chemin d'un dossier>\n", argv[0]);
        return(FAILS);
    }

    
    if (argc > 2) {
        fprintf(stderr, "Trop d'argument\n");
        fprintf(stderr, "Usage: %s <chemin d'un dossier>\n", argv[0]);
        return(FAILS);
    }
    
    explore(argv[1], argv[0]);
    
}