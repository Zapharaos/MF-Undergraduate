# Projet : Game of Life
    Zapharaos, Matthieu Freitag, TP5
    

Voici ma réalisation du projet du cours de Techniques de Développement : « Game of Life » 

### Compilation avec CAIRO:
____________

Compiler :
~~~{.sh}
make
~~~

Compiler sur Mac OS:
~~~{.sh}
make TYPE=MAC
~~~

### Compilation sans CAIRO:
____________

Compiler :
~~~{.sh}
make MODE=TEXTE
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

~~~{.sh}
make doxy
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

Différentes versions disponibles (dans le dossier : Niveaux/ )
- v0 : Projet de base
- v1.0 : Niveau 1
- v2.0 : Niveau 2
- v3.0 : Niveau 3
- v4.0 : Niveau 4
- v5.0 : Niveau 5
- v5.1 : Niveau 5 amélioré

### Structure :
__________

Le projet est réparti dans différents dossiers :
- src/ => Fichiers sources du projet : *.c
- include/ => Headers du projet : *.h
- grilles/ => Fichiers grilles de base : grille*.txt
- doc/ => Documentation doxygen

### Fonctionnement :
__________

Le jeu comporte ces commandes :
- Entrer => Faire évoluer la grille
- n => Charge une nouvelle grille à partir d'un numéro (1 ; 2 ; 3 ; 4 ; 5 ; 6 ; 7 ; 8)
- c => Activer ou désactiver le comptage cyclique (avec ou sans les bords)
- v => Activer ou désactiver le vieillisement
- o =-> Tester si la grille est oscillante
