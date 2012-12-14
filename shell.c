#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "adv_shell_func.h"

int main(void)
{
	char * buf, *pwd;
	char **argv;
	int i, buflen=64;
	
	/* die länge von buf kann sich später ändern (per realloc) 
	 * z.b. beim ersetzen der env-variablen
	 */
	buf = (char*)malloc(buflen*sizeof(char));	
	while(1) {
		{printf("$ ");
			fgets(buf, buflen-2, stdin);
		}
			// fgets ließt \n mit, deshalb dies
			i = strlen(buf);
			buf[i-1]='\0';
			if(*buf==0)
				continue;
			if(cmp_str(buf, "exit", 4)) {
				return 0;
			}
			if (cmp_str(buf, "cd", 2)) {
				chdir(buf+3);
				continue;
			}
			if (cmp_str(buf, "pwd", 3)) {
				getcwd(buf, buflen);
				printf("%s\n",buf);
				continue;
			}
			 
				argv = prepare_argv(buf, &buflen);
				
				run(argv);
    }
}
