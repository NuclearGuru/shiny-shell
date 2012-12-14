#include "adv_shell_func.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void run(char* cmd, char** argv) {
	int j, fred=0, pid;
	FILE *save_stdout=stdout, *save_stdin=stdin;
	
	switch(pid=fork()) {
		case -1:
			printf("konnte Kindprozess nicht erstellen");
			return;
		case 0: // kindprozess
			for(j=0;argv[j]!=NULL;j++){
				if(fred!=2 && *argv[j]=='>') {
					printf("stdout -> %s\n", argv[j+1]);
					argv[j]=NULL;
					freopen(argv[j+1],"w",stdout);
					fred=2;
				}
				if(fred!=2 && *argv[j]=='<') {
					printf("stdin <- %s\n", argv[j+1]);
					argv[j]=NULL;
					freopen(argv[j+1],"r",stdin);
					fred=1;
				}
			}
			execvp(argv[0],argv);
			
			
			if(fred==2) {
				stdout=save_stdout;
				fclose(stdout);
			}
			if(fred>=1) {
				stdin=save_stdin;
				fclose(stdin);
			}
			break;
		default: // elternprozess
			wait(NULL);
			break;
	}
}

int cmp_str(const char *a, const char *b, int depth) {
	int i;
	
	for(i=0;i<depth;i++)
		if(a[i]!=b[i] || ((a[i]!='\0' && b[i]=='\0') || (a[i]=='\0' && b[i]!='\0')))
			return 0;
	
	return 1;
}

inline char **prepare_argv(char *buf, int *buflen, int *argc) {
	char **argv, var[30], *replace;
	int len=0, j=0, i=0,k=0, d=0, count=0, oldlen=0, varlen=0,replen=0;
	
	for(len=0;buf[len]!='\0';len++)
		if(buf[len]==' ') count++;
	*argc=count+1;
	oldlen=len;
	
	// ENV ersetzen
	for(i=0;i<len;i++) {
		if(buf[i]=='$') {
			sscanf(&buf[i], "$%s", var);
			replace = getenv(var);
			if(replace!=NULL) {
				varlen = strlen(var);
				replen = strlen(replace);
				d = replen-varlen;
				len += d < 0 ? d * (-1) : d;
				if(d>0 && *buflen<len) {
					buf = (char*)realloc(buf, len);
					*buflen = len;
				}
				
				for(j=len-1,k=oldlen-1;k>i+varlen;k--,j--)
					buf[j]=buf[k];
				strcpy(buf+i, replace);
			}
		}
	}

	// TODO Dieses malloc spinnt manchmal, z.b. bei ls -al $HOME
	argv = (char**)malloc((count+1)*sizeof(char*));
	argv[0]=buf;
	for(j=1,i=0;j<=count;i++)
		if(buf[i]==' ') {
			argv[j++]=buf+i+1;
			buf[i]='\0';
		}
	
	argv[j]=NULL;
	
	return argv;
}
