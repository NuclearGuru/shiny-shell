#include "adv_shell_func.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


inline void run(char** argv) {
	// fred: merkt sich dateiumlenkungen
	int j, fred=0, waitforchild=1, pid;
	
	// für die dateiumlenkung
	FILE *save_stdout=stdout, *save_stdin=stdin, *a, *b;
	
	for(j=0;argv[j]!=NULL;j++){
				if(fred!=2 && *argv[j]=='>') {
					printf("stdout -> %s\n", argv[j+1]);
					argv[j]=NULL;
					a = freopen(argv[j+1],"w",stdout);
					fred=2;
				}
				if(fred!=2 && *argv[j]=='<') {
					printf("stdin <- %s\n", argv[j+1]);
					argv[j]=NULL;
					b = freopen(argv[j+1],"r",stdin);
					fred=1;
				}
	}
	if(*argv[j-1]=='&') {
		waitforchild=0;
		argv[j-1]=NULL; // & nicht teil der parameterliste
	}
	
	switch(pid=fork()) {
		case -1: //fehler
		printf("konnte Kindprozess nicht erstellen");
			return;
		case 0: // kindprozess
			execvp(argv[0],argv);
			if(fred==2) {
				fclose(a);
				stdout=save_stdout;
			}
			if(fred>=1) {
				fclose(b);
				stdin=save_stdin;
			}
			break;
		default: // elternprozess
		// TODO auch auf zombies warten
			if(waitforchild)
				wait(NULL);
	}
}


/*Eigenimplementation von cmpstr. 
 * gibt 0 zurück wenn ungleich und 1 bei gleich innerhalb depth
 * und nicht mehr!
 */
int cmp_str(const char *a, const char *b, int depth) {
	int i;
	
	for(i=0;i<depth;i++)
		if(a[i]!=b[i] || ((a[i]!='\0' && b[i]=='\0') || (a[i]=='\0' && b[i]!='\0')))
			return 0;
	
	return 1;
}

/* "bereitet" den string vor
 * inline weil ich sie nur einmal aufrufe.
 */
inline char **prepare_argv(char *buf, int *buflen) {
	char **argv, var[30], *replace;
	int len=0, j=0, i=0,k=0, d=0, count=0, oldlen=0, varlen=0,replen=0;
	
	for(len=0;buf[len]!='\0';len++)
		if(buf[len]==' ') count++;
	oldlen=len;
	
	// ENV ersetzen
	for(i=0;i<len;i++) {
		if(buf[i]=='$') {
			sscanf(&buf[i], "$%s", var);
			replace = getenv(var);
			if(replace!=NULL) {
				//längen von var und replace bestimmen
				varlen = strlen(var);
				replen = strlen(replace);
				
				//differenz der stringlängen
				d = replen-varlen;
				len += d < 0 ? d * (-1) : d;
				
				// falls zu klein, string erweitern
				if(*buflen<len) {
					buf = (char*)realloc(buf, len);
					*buflen = len;
				}
				
				// für replace platzmachen
				for(j=len-2,k=oldlen-1;k>i+varlen;k--,j--)
					buf[j]=buf[k];
					
				//replace einfügen
				for(j=i,k=0;k<replen;k++,j++)
					buf[j]=replace[k];
			}
		}
	}
	
	// anzahl token + argv[0] + NULL
	argv = (char**)malloc((count+2)*sizeof(char*));
	
	// die pointer in argv zeigen dann auf stellen in buf
	// leerzeichen werden durch \0 ersetzt
	argv[0]=buf;
	for(j=1,i=0;j<=count;i++)
		if(buf[i]==' ') {
			argv[j++]=buf+i+1;
			buf[i]='\0';
		}
	
	argv[j]=NULL;
	
	return argv;
}
