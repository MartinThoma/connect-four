SOURCE = connectfour
CFLAGS= -g -W -Wall -Werror -std=c99

make:
	gcc $(SOURCE).c $(CFLAGS) -o $(SOURCE)
	gcc $(SOURCE)-strange.c $(CFLAGS) -o $(SOURCE)-strange
	make clean

clean:
	rm -rf *.class *.html *.log *.aux
