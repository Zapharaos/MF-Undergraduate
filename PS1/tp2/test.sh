#!/bin/bash

entete() {
	echo "====================="
	echo "-- $1 : $2"
	echo ""
}

check() {
	EQ=0
	RET=0

	echo "-- Difference entre votre sortie et la sortie attendue :"
	diff -q -s -b <(sort "$1") <(sort "$2")  > /dev/null 2>&1

	if [ $? -ne 0 ]; then
		echo "'$1'" et "'$2'" diffèrent :
		diff -b -y <(sort "$1") <(sort "$2")
	else
		echo "'$1'" et "'$2'" sont identiques.
		EQ=1
	fi

	echo ""
	echo "-- Difference des codes de sortie:"
	if [ "$3" = "$4" ]; then echo "Ils sont identiques."; RET=1; else echo "Ils diffèrent : $4 attendu, $3 obtenu."; fi

	echo ""
	if [ $EQ -eq 1 -a $RET -eq 1 ]; then echo "-- Résultat : test réussi."; else echo "-- Résultat : test échoué."; fi
	echo ""
}

test_1() {
	TEST="test-1"
	entete "$TEST" "Test d'appel sans argument"

	ETUDIANT="test/${TEST}-etudiant.txt"
	CORRECTION="test/${TEST}-correction.txt"

	./explore      > "$ETUDIANT" 2>&1
	ETUDIANT_RET=$?

	CORRECTION_RET=255

	check "$ETUDIANT" "$CORRECTION" "$ETUDIANT_RET" "$CORRECTION_RET"
}

test_2() {
	TEST="test-2"
	entete "$TEST" "Test d'appel avec 2 arguments"

	ETUDIANT="test/${TEST}-etudiant.txt"
	CORRECTION="test/${TEST}-correction.txt"

	./explore a  b  > "$ETUDIANT" 2>&1
	ETUDIANT_RET=$?

	CORRECTION_RET=255

	check "$ETUDIANT" "$CORRECTION" "$ETUDIANT_RET" "$CORRECTION_RET"
}

test_3() {
	TEST="test-3"
	entete "$TEST" "Test d'appel dossier"

	ETUDIANT="test/${TEST}-etudiant.txt"
	CORRECTION="test/${TEST}-correction.txt"

	mkdir -p "${TEST}/rep1"
	mkdir -p "${TEST}/rep1/rep2"
	mkdir -p "${TEST}/rep1/rep3"
	mkdir -p "${TEST}/rep1/rep3/rep4"
	mkdir -p "${TEST}/rep1/rep3/rep5"
	mkdir -p "${TEST}/rep1/rep3/rep5/rep6"
   
	touch "${TEST}/rep1/fichier1-1"
	touch "${TEST}/rep1/fichier1-2"
	touch "${TEST}/rep1/rep2/fichier2-1"
	touch "${TEST}/rep1/rep3/fichier3-1"
	touch "${TEST}/rep1/rep3/fichier3-2"
	touch "${TEST}/rep1/rep3/fichier3-3"
	touch "${TEST}/rep1/rep3/rep4/fichier4-1"
	touch "${TEST}/rep1/rep3/rep5/rep6/fichier6-1"
	
	./explore "${TEST}"  > "$ETUDIANT" 2>&1
	ETUDIANT_RET=$?
	
	CORRECTION_RET=5

	check "$ETUDIANT" "$CORRECTION" "$ETUDIANT_RET" "$CORRECTION_RET"
}

test_4() {
	TEST="test-4"
	entete "$TEST" "Test d'appel hauteur max"

	ETUDIANT="test/${TEST}-etudiant.txt"
	CORRECTION="test/${TEST}-correction.txt"
    
	DIR="${TEST}/"
	mkdir -p $DIR
    
	for i in `seq 1 254`; do cd $DIR; TMP="$i"; mkdir -p $TMP; DIR=$TMP; done

	touch "fichier"
    
	for i in `seq 1 254`; do cd "..";done
    
	./explore "${TEST}" > "$ETUDIANT" 2>&1
	ETUDIANT_RET=$?

	CORRECTION_RET=255

	check "$ETUDIANT" "$CORRECTION" "$ETUDIANT_RET" "$CORRECTION_RET"
}

test_5() {
	TEST="test-5"
	entete "$TEST" "Test d'appel en cas de problème"

	ETUDIANT="test/${TEST}-etudiant.txt"
	CORRECTION="test/${TEST}-correction.txt"

	./explore a > "$ETUDIANT" 2>&1

	ETUDIANT_RET=$?
	CORRECTION_RET=255

	check "$ETUDIANT" "$CORRECTION" "$ETUDIANT_RET" "$CORRECTION_RET"
}


# Verifie l'argument
if [ $# -lt 1 ] || [ $# -gt 1 ] || [ $1 -lt 1 ] || [ $1 -gt 5 ]; then
	echo "Usage: $0 <numero du test (1 - 5)>" 1>&2
	exit 1;
fi


# Lance le test
if [ $1 -eq 1 ]; then
	test_1;
elif [ $1 -eq 2 ]; then
	test_2;
elif [ $1 -eq 3 ]; then
	test_3;
elif [ $1 -eq 4 ]; then
	test_4;
elif [ $1 -eq 5 ]; then
    test_5;
fi
