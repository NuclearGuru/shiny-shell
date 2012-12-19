CFLAGS = -Wall

shell: shell.o adv_shell_func.o
	gcc -o shell shell.o adv_shell_func.o 

shell.o: shell.c
	gcc -c shell.c 

adv_shell_func.o: adv_shell_func.c
	gcc -c adv_shell_func.c 

debug: clean
	gcc -g -o shell shell.c adv_shell_func.c 

clean:
	rm -f shell *.o
