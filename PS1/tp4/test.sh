#!/bin/bash

bold=$(tput bold)
normal=$(tput sgr0)



existance(){
	if ! test -x "./$1"; then
 	   echo "Exécutable $1 introuvable dans ce répertoire"
	    exit 1
	fi

}

existance choisir
existance huissier

TEST="TEST"

check_stderr(){
	[ -s "$1" ]
	if [ $? -ne 0 ] ; then echo -e "\t$1: vide (ok)"; return 0;  else OK_C=0; echo -e "\t$1: (fail), présence d'erreurs"; return 1; fi
}

check_ret(){
	if [ $1 -ne 0 ]; then echo -e "\tcode de sortie $2: (fail) ($1 obtenu, 0 attendu)"; return 1; else echo -e "\tcode de sortie $2: (OK)"; return 0; fi

}


compare(){
	diff -q -s -b $1 $2 > /dev/null 2>&1
	RET=$?
	if [ $RET -ne 0 ]; then return 1; else return 0; fi
}


argument_check(){
	touch argument	
	touch sortie
	P=0
	if [ $1 = "choisir" ]; then 
		 ./"$1"  > argument 2>&1
		RET1=$?
		echo "Argument manquant" > sortie
		echo "Usage: ./$1 <N>" >> sortie
		compare argument sortie
		P=$?
		if [ $P -ne 0 ]; then echo -e "\t$1.c, il faut afficher:\n\t\tArgument manquant\n\t\tUsage: ./$1 <N>"; fi
		if [ $RET1 -ne 1 ]; then echo -e "\tCode de sortie $1.c sans argument, $RET1 obtenu, 1 attendu"; P=1; fi	
	fi
	MSG1="\t$1.c, il faut afficher:\n\t\tTrop d'arguments\n\t\tUsage: ./$1"
	./"$1" a b > argument 2>&1 
	RET1=$?
	echo "Trop d'arguments" > sortie
	if [ $1 = "choisir" ]; then echo "Usage: ./$1 <N>" >> sortie; else echo "Usage: ./$1" >> sortie; fi
	compare argument sortie
	RET2=$?
	rm sortie argument
	[ $RET2 -ne 0 ] && [ $1 = "choisir" ] && echo -e "${MSG1} <N>" && P=1;
	[ $RET2 -ne 0 ] && [ $1 = "huissier" ] && echo -e "${MSG1}" && P=1; 
	if [ $RET1 -ne 1 ]; then echo -e "\tCode de sortie $1.c avec 2 arguments, $RET1 obtenu, 1 attendu"; P=1; fi
	if [ $P -eq 0 ]; then return 0; else return 1; fi	
}



test1(){
	echo "${bold}$TEST: Vérification du nombre d'arguments${normal}"
	P=0
	argument_check huissier
	if [ $? -eq 0 ]; then echo -e "\t$TEST huissier: ${bold}success${normal}"; else echo -e "\t$TEST huissier: ${bold}fail${normal}"; P=1; fi
	
	argument_check choisir
	if [ $? -eq 0 ]; then echo -e "\t$TEST: choisir: ${bold}success${normal}"; else echo -e "\t$TEST: choisir: ${bold}fail${normal}"; P=1; fi 
	[ $P -eq 1 ] && exit 1
}


test2(){ # Vérification pour voir si'il n'y a pas de blocage
	MSG="TEST de blocage"
	timeout 15s ./huissier > /dev/null 2>&1 & 
	PID=$!
	P=1
	timeout 10s  ./choisir 4 > /dev/null 2>&1
	test $? -eq 124 && echo -e "\t Blocage: choisir (timeout)" && P=0
	wait $PID
	test $? -eq 124 && echo -e "\t Blocage huissier (timeout)" && P=0
	[ $P -eq 0 ] && echo -e "\t${MSG}: fail${normal}" && exit 1
}



check_behavior(){ 
	echo -e "\tSortie et codes de retour ${normal}"
	./huissier > sortieh 2> huissier_stderr &
	PID=$!
	./choisir $1 > sortiec 2> Choisir_stderr
	RETC=$?
	wait $PID
	RETH=$?
	echo -e "\t${bold}choisir:${normal}"
	check_stderr choisir_stderr
	RET1=$?
	check_ret $RETC choisir
	RET2=$?
	if [ $RET1 -eq 0 ] && [ $RET2 -eq 0 ]; then R1=0; else R1=1; fi
	
	echo -e "\t${bold}huissier: ${normal}"
	check_stderr huissier_stderr
	RET1=$?
	check_ret $RETH huissier
	RET2=$?
	if [ $RET1 -eq 0 ] && [ $RET2 -eq 0 ]; then R2=0; else R2=1; fi
	
	if [ $R1 -eq 0 ] && [ $R2 -eq 0 ]; then echo -e "\t${bold}${TEST}: success"; else echo -e "\t${TEST}: fail${normal}"; fi 
	
	rm sortieh sortiec huissier_stderr Choisir_stderr

}


test3(){
	echo "${bold}$TEST: Vérification pour N=5 ${normal}"	
	check_behavior 5
	echo "${bold}$TEST: Vérification pour N=8 ${normal}"
	check_behavior 8
}



test4(){
	echo "${bold}$TEST: fonctionnalité FIFO${normal}"
	touch sortieh
	touch sortiec
	./huissier > sortieh 2> /dev/null &
	./choisir 4 > sortiec 2> /dev/null
	SPC=1
	head -n 1 sortiec > verif
	diff -q -s -b verif sortieh > /dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo -e "\tLes résultats affichés par choisir et huissier sont diffèrents"; SPC=0
	else
		echo -e "\tLes résultats affichés par choisir et huissier sont identiques"
	fi
	if test -p "huissier_fifo"; then
    		echo -e "\tTube nommé huissier_fifo non supprimé"
   		SPC=0
	fi
	if [ $SPC -eq 0 ]; then echo -e "\t${bold}${TEST}: fail"; else echo -e "\t${TEST}: success${normal}"; fi 
	rm sortiec sortieh verif

}



test5(){
	echo  "${bold}$TEST: Vérification du code de retour en cas d'erreur ${normal}"
	[ -p "huissier_fifo" ] && rm "huissier_fifo"  
	mkfifo "huissier_fifo"
	./huissier > /dev/null 2>&1
	if [ $? -ne 1 ]; then echo -e "\t${bold}${TEST}: fail ($? obtenu 1 attendu)"; else echo -e "\t${TEST}: success${normal}"; fi 
	rm huissier_fifo
}

for i in `seq 1 5`; do test$i; done

