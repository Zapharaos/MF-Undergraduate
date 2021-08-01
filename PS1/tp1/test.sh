#!/bin/bash

entete() {
	echo "====================="
	echo "-- $1 : $2"
	echo ""
}

check() {
	echo "-- Difference entre votre sortie et la sortie attendue :"
	diff -q -s -b <(cat "$1" | sort) <(cat "$2" | sort) > /dev/null 2>&1

	if [ $? -ne 0 ]; then
		echo "'$1' et '$2' diffèrent."
		echo ""
		echo "-- Affichez leur contenu pour voir toutes les diférences (l'ordre des lignes n'a pas d'importance)."
		echo "-- Ci-dessous le résultat de la commande 'diff' qui donne un appercu des différences des deux fichiers :"
		diff -b -y <(cat "$1" | sort) <(cat "$2" | sort)
	else
		echo "'$1' et '$2' sont identiques."
	fi

	echo ""
	echo "-- Difference des codes de sortie:"
	if [ "$3" = "$4" ]; then echo "Ils sont identiques."; else echo "Ils diffèrent : $4 attendu, $3 obtenu."; fi

	echo ""
	if diff -b <(cat "$1" | sort) <(cat "$2" | sort) > /dev/null 2>&1 && [ "$3" = "$4" ]; then echo "-- Résultat : test réussi."; else echo "-- Résultat : test échoué."; fi
	echo ""
}

test_1() {
	TEST="test-1"
	entete "$TEST" "Test d'appel sans argument"

	ETUDIANT="./test/${TEST}-etudiant.txt"
	CORRECTION="./test/${TEST}-correction.txt"

	./dirinfo      > "$ETUDIANT" 2>&1
	ETUDIANT_RET=$?

	#./dirinfo-corr > "$CORRECTION" 2>&1
	CORRECTION_RET=1

	check "$ETUDIANT" "$CORRECTION" "$ETUDIANT_RET" "$CORRECTION_RET"
}

test_2() {
	TEST="test-2"
	entete "$TEST" "Test d'appel avec 2 arguments"

	ETUDIANT="./test/${TEST}-etudiant.txt"
	CORRECTION="./test/${TEST}-correction.txt"

	./dirinfo a b     > "$ETUDIANT" 2>&1
	ETUDIANT_RET=$?

	#./dirinfo-corr a b > "$CORRECTION" 2>&1
	CORRECTION_RET=1

	check "$ETUDIANT" "$CORRECTION" "$ETUDIANT_RET" "$CORRECTION_RET"
}

test_3() {
	TEST="test-3"
	entete "$TEST" "Test d'appel avec un seul niveau de dossier"

	ETUDIANT="./test/${TEST}-etudiant.txt"
	CORRECTION="./test/${TEST}-correction.txt"

	mkdir -p "./test/${TEST}/photos"
	mkdir -p "./test/${TEST}/vide"
	mkdir -p "./test/${TEST}/musiques"
	mkdir -p "./test/${TEST}/scripts"
	mkdir -p "./test/${TEST}/documents"

	printf "\x49\x49\x2a\x00afjfsfjfkdsfjksfs" > "./test/${TEST}/photos/fichier1.tiff"
	printf "\x49\x49\x2a\x00afjfsfjfkdsfjksfs" > "./test/${TEST}/photos/fichier2.tiff"
	printf "\x4d\x4d\x00\x2aafjfsfjfkdsfjksfs" > "./test/${TEST}/photos/fichier3.tiff"

	printf "ID3fdjqklfjdsqkjffjsk000" > "./test/${TEST}/musiques/fichier1.mp3"
	printf "ID3fdjqklfjdsqkjffjsk000" > "./test/${TEST}/musiques/fichier2.mp3"

	printf "#!/bin/env php\x0adzdz" > "./test/${TEST}/scripts/fichier1.php"
	printf "#!/usr/bin/php\x0adz"   > "./test/${TEST}/scripts/fichier2.php"
	printf "#!/bin/sh\x0a"          > "./test/${TEST}/scripts/fichier3.sh"
	printf "#!/bin/bash"            > "./test/${TEST}/scripts/fichier4.sh"

	printf "\x50\x4b00fdjqklfjdsqkjffjsdqk"          > "./test/${TEST}/documents/fichier1.zip"
	printf "\x37\x7A\xBC\xAF\x27\x1Clfjdsqkjffjsdqk" > "./test/${TEST}/documents/fichier2.7z"
	printf "\x25PDF-fdjqklfjdsqkjffjsdqk"            > "./test/${TEST}/documents/fichier3.pdf"
	printf "\x25PDF-fdjqklfjds"                      > "./test/${TEST}/documents/fichier4.pdf"
	printf "\x37\x7A\xBC\x66\x22\x11fdjqklfjk"       > "./test/${TEST}/documents/fichier5.mp3"
	printf "....fdjqklfjdsqkjffjsdqk"                > "./test/${TEST}/documents/fichier6.toto"
	printf ""                                        > "./test/${TEST}/documents/fichier7.vide"
	printf "A"                                       > "./test/${TEST}/documents/fichier8.vide"
	printf "\x50\x4b00fdjqklfjdsqkjffjsdqk"          > "./test/${TEST}/documents/fichierzip.pdf"
	printf "ID3fdjqklfjdsqkjffjsk000"                > "./test/${TEST}/documents/fichiermp3.pdf"

	./dirinfo "./test/${TEST}" > "$ETUDIANT" 2>&1
	ETUDIANT_RET=$?

	#./dirinfo-corr "./test/${TEST}" > "$CORRECTION" 2>&1
	CORRECTION_RET=0

	check "$ETUDIANT" "$CORRECTION" "$ETUDIANT_RET" "$CORRECTION_RET"
}

test_4() {
	TEST="test-4"
	entete "$TEST" "Test d'appel avec sous-dossiers"

	ETUDIANT="./test/${TEST}-etudiant.txt"
	CORRECTION="./test/${TEST}-correction.txt"

	mkdir -p "./test/${TEST}/photos"
	mkdir -p "./test/${TEST}/vide"
	mkdir -p "./test/${TEST}/a/vide"
	mkdir -p "./test/${TEST}/b/c/d/musiques"
	mkdir -p "./test/${TEST}/e/g/scripts"
	mkdir -p "./test/${TEST}/documents"
	mkdir -p "./test/${TEST}/documents/f"

	printf "\x49\x49\x2a\x00afjfsfjfkdsfjksfs" > "./test/${TEST}/photos/fichier1.tiff"
	printf "\x49\x49\x2a\x00afjfsfjfkdsfjksfs" > "./test/${TEST}/photos/fichier2.tiff"
	printf "\x4d\x4d\x00\x2aafjfsfjfkdsfjksfs" > "./test/${TEST}/photos/fichier3.tiff"

	printf "ID3fdjqklfjdsqkjffjsk000" > "./test/${TEST}/b/c/d/musiques/fichier1.mp3"
	printf "ID3fdjqklfjdsqkjffjsk000" > "./test/${TEST}/b/c/d/musiques/fichier2.mp3"

	printf "#!/bin/env php\x0adzdz" > "./test/${TEST}/e/g/scripts/fichier1.php"
	printf "#!/usr/bin/php\x0adz"   > "./test/${TEST}/e/g/scripts/fichier2.php"
	printf "#!/bin/sh\x0a"          > "./test/${TEST}/e/g/scripts/fichier3.sh"
	printf "#!/bin/bash"            > "./test/${TEST}/e/g/scripts/fichier4.sh"

	printf "\x50\x4b00fdjqklfjdsqkjffjsdqk"          > "./test/${TEST}/documents/fichier1.zip"
	printf "\x37\x7A\xBC\xAF\x27\x1Clfjdsqkjffjsdqk" > "./test/${TEST}/documents/fichier2.7z"
	printf "\x25PDF-fdjqklfjdsqkjffjsdqk"            > "./test/${TEST}/documents/fichier3.pdf"
	printf "\x25PDF-fdjqklfjds"                      > "./test/${TEST}/documents/fichier4.pdf"
	printf "\x37\x7A\xBC\x66\x22\x11fdjqklfjk"       > "./test/${TEST}/documents/fichier5.mp3"
	printf "....fdjqklfjdsqkjffjsdqk"                > "./test/${TEST}/documents/f/fichier6.toto"
	printf ""                                        > "./test/${TEST}/documents/f/fichier7.vide"
	printf "A"                                       > "./test/${TEST}/documents/f/fichier8.vide"
	printf "\x50\x4b00fdjqklfjdsqkjffjsdqk"          > "./test/${TEST}/documents/f/fichierzip.pdf"
	printf "ID3fdjqklfjdsqkjffjsk000"                > "./test/${TEST}/documents/f/fichiermp3.pdf"

	./dirinfo "./test/${TEST}" > "$ETUDIANT" 2>&1
	ETUDIANT_RET=$?

	#./dirinfo-corr "./test/${TEST}" > "$CORRECTION" 2>&1
	CORRECTION_RET=0

	check "$ETUDIANT" "$CORRECTION" "$ETUDIANT_RET" "$CORRECTION_RET"
}


# Verifie l'argument
if [ $# -lt 1 ] || [ $# -gt 1 ] || [ $1 -lt 1 ] || [ $1 -gt 4 ]; then
	echo "Usage: $0 <numero du test (1 - 4)>" 1>&2
	exit 1;
fi

mkdir "./test/" 2> /dev/null

# Lance le test
if [ $1 -eq 1 ]; then
	test_1;
elif [ $1 -eq 2 ]; then
	test_2;
elif [ $1 -eq 3 ]; then
	test_3;
elif [ $1 -eq 4 ]; then
	test_4;
fi
