#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "aurras.h"


char* concat(int argc,char* argv[]){
    int commandindex=1;
    char* command = malloc(MESSAGESIZE);
    
    while ( commandindex<argc ){
        strcat (command, argv[commandindex]);
        strcat (command, " ");
        commandindex++;
    }
    return command;
}


int main(int argc, char *argv[]) {
    char *string = malloc(MESSAGESIZE);
    
    
    //argv[2] = "\"ls\"";
    string = concat(argc,argv);
    int bytesRead = strlen( string);
     
        if(string[bytesRead - 1] == '\n') string[--bytesRead] = 0;  // no fim da string poe NULL
        
        int client_server_fifo = open("client_server_fifo", O_WRONLY); // abrir pipes com nome
        int server_client_fifo = open("server_client_fifo", O_RDONLY);

        write(client_server_fifo, string, bytesRead);   // escreve para o pipe
        close(client_server_fifo);                      // fecha o pipe


        // le o que vem do server
        while((bytesRead = read(server_client_fifo, string, MESSAGESIZE)) > 0)
            write(STDOUT_FILENO, string, bytesRead);   
              
        close(server_client_fifo);

    
    
    return 0;
}
