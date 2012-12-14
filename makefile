shell: shell.c adv_shell_func.c
	gcc -o shell shell.c adv_shell_func.c

debug: clean
	gcc -g -o shell shell.c adv_shell_func.c
	
clean:
	rm -f shell
