# Projet : Game of Life
	Zapharaos, Matthieu Freitag, TD5

Voici ma réalisation du projet du cours de Techniques de Développement : « Game of Life »

### Compilation :
____________

Compiler :
~~~{.sh}
make
~~~

Compiler sur Mac OS :
~~~{.sh}
make TYPE=MAC
~~~

Le programme compile de base avec un affichage graphique (Cairo et X requis).

Compiler sans affichage graphique :
~~~{.sh}
make MODE=TEXTE
~~~

Compiler sans affichage graphique sur MAC OS:
~~~{.sh}
make TYPE=MAC MODE=TEXTE
~~~

### Execution :
__________

~~~{.sh}
./bin/main <numero de la grille>
~~~

**ATTENTION :** numero de la grille  = 1 ; 2 ; 3 ; 4 ; 5 ; 6 ; 7 ; 8

### Documentation :
__________

~~~{.sh}
doxygen
~~~

### Archivage :
__________

~~~{.sh}
make dist
~~~

### Nettoyage :
__________

~~~{.sh}
make clean
~~~

### Versions :
_________

Les versions vX.0 sont des versions stables (à privilégier).
Les versions vX.Y sont des versions instables (à éviter).

Différentes versions disponibles :
- v0 : Projet de base
- v1.0 : Niveau 1
- v2.0 : Niveau 2
- v3.0 : Niveau 3
- v4.0 : Niveau 4
- v5.0 : Niveau 5

### Structure :
__________

Le projet est réparti dans différents dossiers :
- src/ => Fichiers sources du projet : *.c
- include/ => Headers du projet : *.h
- grilles/ => Fichiers grilles de base : grille*.txt
- lib/ => Librairies : *.a

        
