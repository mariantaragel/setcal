setcal: setcal.c
	gcc -std=c99 -Wall -Wextra -Werror setcal.c -o setcal -g

clean:
	rm -rf setcal
