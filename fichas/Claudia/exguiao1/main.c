#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>   /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h>    /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#define FI "ficheiro.txt"


///////////////////////////////////////////EXERCICIO 1 /////////////////////////////////

int ex1 () {
char *buffer= malloc (sizeof(char)*200) ;
int fi = open (FI,O_RDONLY);
int bytes_writen=0;
int bytes_read=0;
if(fi<0) {
return 1;
}
else {
printf("arquivo aberto\n");

int novo_fi =open("ficheiro(1).txt", O_CREAT |O_TRUNC | O_WRONLY, 0600);

while ((bytes_read=read(fi, buffer, 100))>0) {

bytes_writen += write(novo_fi, buffer, bytes_read);
}

close(fi);
close(novo_fi);
}
free(buffer);

return 0;
}
//////////////////////EX2 ////////////////////////----como fazer com um ficheiro ??
int ex2 (){
    char *buffer= malloc (sizeof(char)*200) ;
    while ((read(STDIN_FILENO, buffer,200))>0) {
        write(STDOUT_FILENO,buffer,strlen(buffer));
    }

    free(buffer);
    return 0;

}

///////////////////////EX3/////////////////////////
int ex3 (){
    char *buffer = malloc (40*sizeof(char));
    int i=0;
    int fi = open (FI,O_RDONLY);
    while()
    readln(fi,);

}


