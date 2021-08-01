#!/bin/bash

PROG=delegation
if ! test -x "./$PROG"; then
    echo "Exécutable $PROG introuvable dans ce répertoire"
    exit 1
fi

# Répertoire temporaire
TMPDIR=$(mktemp -d "tmp.XXX")
# Nom de ce script
TEST=$(basename $0 .sh)
# Fichier de log
LOG=$TEST.log
exec 2> $LOG
# pas de variable indéfinie, + log des commandes
set -ux

# Fonctions pour l'affichage
function debut_test () # numero titre
{
    if test -t 1; then
        echo -n "[  ] "
    fi
    echo -n "$1. $2"
}
function fin_test ()
{
    if test -t 1; then
        echo -e "\r[\033[1;32mok\033[0m]"
    else
        echo " ok"
    fi
}
function fail () # erreur
{
    if test -t 1; then
        echo -e "\r[\033[1;31mko\033[0m]"
    else
        echo " ko"
    fi
    echo "==> Échec : '$1'."
    echo "==> Log : '$LOG'."
    echo "==> Exit"
    rm -rf $TMPDIR
    exit 1
}

# Annoying verbose builtins
function push_dir () # dir
{
    pushd $1 > /dev/null 2>&1
}
function pop_dir () #
{
    popd > /dev/null 2>&1
}

########
#
# 1. Test du nombre d'arguments
#
function test_1 ()
{
    debut_test 1 "Test du nombre d'arguments"
    mkdir $TMPDIR/empty
    push_dir $TMPDIR/empty
    #
    ../../$PROG > /dev/null 2>&1
    EXITCODE=$?
    if test $EXITCODE -ne 6; then
        fail "Appel sans argument (exit $EXITCODE au lieu de 6)"
    fi
    #
    ../../$PROG un deux > /dev/null 2>&1
    EXITCODE=$?
    if test $EXITCODE -ne 6; then
        fail "Appel avec deux arguments (exit $EXITCODE au lieu de 6)"
    fi
    #
    ../../$PROG malarkey > /dev/null 2>&1
    EXITCODE=$?
    if test $EXITCODE -ne 6; then
        fail "Appel avec un argument absurde (exit $EXITCODE au lieu de 6)"
    fi
    #
    pop_dir
    rmdir $TMPDIR/empty
    fin_test
}

########
#
# 2. Test de fonctionnalité
#
function verifier_statistiques () # fichier
{
    awk '
{if ($1=="COMMANDANT") { cs=$2; ce=$3; }
else if ($1=="LIEUTENANT") { ls=$2; le=$3; }
else exit 1; }
END { exit (cs!=ls || ce!=le); }' < $1
    if test $? -ne 0; then
        echo "Mauvais format ou statistiques erronnées" >&2
        sed -e 's/^/| /' $1 >&2
        return 1
    else
        return 0
    fi
}
function test_2 ()
{
    debut_test 2 "Test de fonctionnalité"
    mkdir $TMPDIR/test-20
    touch $TMPDIR/test-20/fichier{00..19}.data
    push_dir $TMPDIR/test-20
    #
    echo -n " "
    for i in {0..9}; do
        echo -n "."
        ../../$PROG +fast > ../std.out 2> /dev/null
        EXITCODE=$?
        if test $EXITCODE -gt 1; then
            fail "Exit $EXITCODE au lieu de 0 ou 1"
        fi
        if ! verifier_statistiques ../std.out; then
            fail "Mauvais format ou statistiques erronnées (voir $LOG)"
        fi
        rm ../std.out
    done
    #
    pop_dir
    rm -rf $TMPDIR/test-20
    fin_test
}

########
#
# 3. Test des codes de retour
#

# 31. Test du code de retour sur erreur d'écriture ardoise
function test_31 () 
{
    debut_test 31 "Test du code de retour sur erreur d'écriture ardoise"
    mkdir $TMPDIR/test-20
    touch $TMPDIR/test-20/fichier{00..19}.data
    push_dir $TMPDIR/test-20
    #
    ../../$PROG +slow > /dev/null 2>&1 &
    PID=$!
    sleep 3; chmod -w ardoise
    wait $PID
    EXITCODE=$?
    if test $EXITCODE -ne 2; then
        fail "Exit $EXITCODE au lieu de 2"
    fi
    #
    pop_dir
    rm -rf $TMPDIR/test-20
    fin_test
}
# 32. Test du code de retour sur crash de lieutenant
function test_32 () 
{
    debut_test 32 "Test du code de retour sur crash de lieutenant"
    mkdir $TMPDIR/test-20
    touch $TMPDIR/test-20/fichier{00..19}.data
    push_dir $TMPDIR/test-20
    #
    ../../$PROG -fast > /dev/null 2>&1
    EXITCODE=$?
    if test $EXITCODE -ne 3; then
        fail "Exit $EXITCODE au lieu de 3"
    fi
    #
    pop_dir
    rm -rf $TMPDIR/test-20
    fin_test
}
# 33. Test du code de retour sur réception de SIGTERM
function test_33 () 
{
    debut_test 33 "Test du code de retour sur réception de SIGTERM"
    mkdir $TMPDIR/test-20
    touch $TMPDIR/test-20/fichier{00..19}.data
    push_dir $TMPDIR/test-20
    #
    ../../$PROG +slow > /dev/null 2>&1 &
    PID=$!
    sleep 3; kill -TERM $PID
    wait $PID
    EXITCODE=$?
    if test $EXITCODE -ne 4; then
        fail "Exit $EXITCODE au lieu de 4"
    fi
    #
    pop_dir
    rm -rf $TMPDIR/test-20
    fin_test
}
# 34. Test du code de retour sur répertoire illisible
function test_34 ()
{
    debut_test 34 "Test du code de retour sur répertoire illisible"
    mkdir $TMPDIR/empty
    chmod -r $TMPDIR/empty
    push_dir $TMPDIR/empty
    #
    ../../$PROG +slow > /dev/null 2>&1
    EXITCODE=$?
    if test $EXITCODE -ne 7; then
        fail "Exit $EXITCODE au lieu de 4"
    fi
    #
    pop_dir
    rmdir $TMPDIR/empty
    fin_test
}
# 35. Test du code de retour lorsque kill échoue
function test_35 ()
{
    debut_test 35 "Test du code de retour lorsque kill échoue"
    mkdir $TMPDIR/junk
    push_dir $TMPDIR/junk
    #
    # simple interposition
    cat > k.c <<EOF
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
int kill(pid_t pid, int sig) { errno = ELIBBAD; return -1; }
EOF
    gcc -shared -fPIC k.c -o k.so
    # Note : arguments étranges pour qu'on puisse pkiller le fils
    LD_PRELOAD=./k.so ../../$PROG "499:501" > /dev/null 2>&1
    EXITCODE=$?
    if test $EXITCODE -ne 8; then
        fail "Exit $EXITCODE au lieu de 8"
    fi
    pkill -KILL -f -x "../../$PROG 499:501"
    #
    pop_dir
    rm -rf $TMPDIR/junk
    fin_test
}

function test_3 ()
{
    test_31
    test_32
    test_33
    test_34
    test_35
}

########
#
# 4. Test en charge
#
function test_4 ()
{
    debut_test 4 "Test en charge"
    mkdir $TMPDIR/test-1000
    push_dir $TMPDIR/test-1000
    for i in {0..9}; do
        for j in {0..9}; do
            echo "$i$j"{0..9}
        done
    done | xargs touch
    #
    echo -n " "
    for i in {0..19}; do
        echo -n "."
        ../../$PROG 0:5000 > ../std.out 2> /dev/null
        EXITCODE=$?
        if test $EXITCODE -le 1; then
            if ! verifier_statistiques ../std.out; then
                fail "Format incorrect ou statistiques erronnées"
            fi
        elif test $EXITCODE -ne 3; then
            fail "Exit $EXITCODE au lieu de soit 0/1 soit 7"
        fi
        rm ../std.out
    done
    #
    pop_dir
    rm -rf $TMPDIR/test-1000
    fin_test
}

########
#
# 5. Test mémoire avec valgrind
#
function test_5 ()
{
    debut_test 5 "Test mémoire avec valgrind"
    mkdir $TMPDIR/test-1000
    push_dir $TMPDIR/test-1000
    for i in {0..9}; do
        for j in {0..9}; do
            echo "$i$j"{0..9}
        done
    done | xargs touch
    #
    valgrind --leak-check=full \
             --log-file=$TMPDIR/valgrind.log \
             --error-exitcode=100 \
             ../../$PROG 0:0 > /dev/null 2>&1 \
        || test $? -ne 100 \
        || fail "Erreur mémoire"
    valgrind --leak-check=full \
             --log-file=$TMPDIR/valgrind.log \
             --error-exitcode=100 \
             ../../$PROG 0:0 > /dev/null 2>&1 \
        || test $? -ne 100 \
        || fail "Erreur mémoire"
    #
    pop_dir
    rm -rf $TMPDIR/test-1000
    fin_test
}

# Main

if test $# -eq 0; then
    echo ""
    echo "Usage: $0 numéro [numéro...]"
    echo ""
    echo "avec numéro dans la liste suivante :"
    sed -n -e 's/[ \t]*debut_test \([0-9][0-9]*\) /\1 : /p' $0
    echo ""
fi

for t in "$@"; do
    if  declare -F "test_$t" > /dev/null; then
        test_$t
    else
        echo "Test <$t> inconnu"
    fi
done
rm -rf $TMPDIR
exit 0
