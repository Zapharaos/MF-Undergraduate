
all: explore
	
explore: explore.c
	cc -Wall -Wextra -Werror -Wvla explore.c -o explore

test: all FORCE
	@bash test.sh 1
	@bash test.sh 2
	@bash test.sh 3
	@bash test.sh 4
	@bash test.sh 5

FORCE: ;

clean:
	rm explore
