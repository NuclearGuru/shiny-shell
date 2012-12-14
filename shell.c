#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "adv_shell_func.h"

int main(void)
{
	char * buf, *pwd;
	char **argv;
	int argc, buflen=64;
	
	buf = (char*)malloc(buflen*sizeof(char));
	
	while(1) {
		printf("$ ");
		if ( gets(buf) != NULL ) {
			
			if(cmp_str(buf, "exit", 4)) {
				return 0;
			}
			if (cmp_str(buf, "cd", 2)) {
				chdir(buf+3);
				continue;
			}
			if (cmp_str(buf, "pwd", 2)) {
				getcwd(buf, buflen);
				printf("%s\n",buf);
				continue;
			}
			 
				argv = prepare_argv(buf, &buflen, &argc);
				
				run(argv[0], argv);
			
				/* TODO
				 * Eigentlich soll man mit malloc allokierten Speicher 
				 * wieder freigeben, aber hier stört der sich dran
				 */
//				free(argv);
		}
    }
}
