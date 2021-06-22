#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#define MESSAGESIZE 4096

char* tasks[2048];
int taskStatus[2048];
int filtros[2048][5];



int filtroMaxValue[2048];
char* filtroNome[2048];
char* filtroApp[2048];

int users[5];

//int usersAlto = 0;
//int users[1] = 0;
//int usersEco = 0;
//int usersRapido = 0;
//int usersLento = 0;


char* args[100];

int pids[2048];

int iTask = 0;


int pid;
int pidOut;

char** parse_conf(int* narg,char file_path[]){
    
    char* buffer = calloc(MESSAGESIZE, sizeof(char));
    int conf = open(file_path,O_RDONLY);
    int i=0;
    char* token;
    char* tokenAux;

    char** args = malloc(sizeof(char*)*100);
    read(conf, buffer, MESSAGESIZE);

    while((token = strtok_r(buffer, "\n", &buffer))) {
        
        while((tokenAux = strtok_r(token, " ", &token))){
            args[i] = strdup(tokenAux);
        i++;     
    
        } 
           

    }
    *narg = i;
    return args;


    
}
void save_state(){
    int fd = open("./etc/state.conf", O_CREAT | O_TRUNC | O_WRONLY, 0666);

    char messag[64];
    sprintf(messag, "%d\n",users[0]);
    write(fd, messag, strlen(messag));
    
    sprintf(messag, "%d\n",users[1]);
    write(fd, messag, strlen(messag));
    
    sprintf(messag, "%d\n",users[2]);
    write(fd, messag, strlen(messag));
    
    sprintf(messag, "%d\n",users[3]);
    write(fd, messag, strlen(messag));
    
    sprintf(messag, "%d",users[4]);
    write(fd, messag, strlen(messag));
    
    close(fd);
}

void chld_handler(int sig){

    pid_t pidfilho = wait(NULL);
    
    int i;
    for (i = 0; i < iTask; i++)
    {
        if (pidfilho == pids[i])
        {
        
        printf(" %d <-\n",pidfilho);
        
        printf(" %d <-\n",pids[i]);

        users[0] -= filtros[i][0];
        users[1] -= filtros[i][1];
        users[2] -= filtros[i][2];
        users[4] -= filtros[i][3];
        users[3] -= filtros[i][4]; 

        taskStatus[i] = 1;
    
        }
        
    }
    

}

void chld_handler_filtro(int sig){
    
    wait(NULL);

}

int main(int argc, char *argv[]) {


    mkfifo("client_server_fifo", 0644);
    mkfifo("server_client_fifo", 0644);
   
    int nargs;
    char** conf_args = parse_conf(&nargs ,argv[1]);
    
    int nFiltros = 0;
    for (int l=0;l < nargs ; l = l + 3){
        
        filtroNome[nFiltros]     = strdup(conf_args[ l ]);    
        filtroApp[nFiltros]      = strdup(conf_args[ l + 1]); 
        filtroMaxValue[nFiltros] = atoi(conf_args[ l + 2]); 
        nFiltros++;
      
    }
      
      
    while(1) {
        
        
        char* buffer = calloc(MESSAGESIZE, sizeof(char));
        int client_server_fifo = open("client_server_fifo", O_RDONLY);
        int server_client_fifo = open("server_client_fifo", O_WRONLY);
        
        if(read(client_server_fifo, buffer, MESSAGESIZE)>0){ 
        
        if(strncmp(buffer, "transform", 9) == 0){

            char command[MESSAGESIZE];
            strcpy(command, buffer + 9);
            
            taskStatus[iTask] = 0;
            tasks[iTask] = strdup(buffer);// transform hasdjbasd hbasdhf eco eco eco
            
            iTask++;

            char message[64];
            sprintf(message, "pending\n"); 
            
            write(server_client_fifo, message, strlen(message));
            sprintf(message, "processing\n");
            
            write(server_client_fifo, message, strlen(message));
            close(server_client_fifo);

            char* token;
            char* args[100];
            int i = 0;
            char* rest = command;
            char* tmp;

            while((token = strtok_r(rest, " ", &rest))) {
                if(i == 0) {
                    args[0] = strdup(token);
                    i++;
                } else {
                    args[i] = strdup(token);
                    i++;
                }
            }

            char* id = strdup(args[1]);
        

            char* output1 = malloc(MESSAGESIZE);
         
            strcat(output1,"./tmp/outputTMP1_");
            strcat(output1,id);
            
            char* output2 = malloc(MESSAGESIZE);
            strcat(output2,"./tmp/outputTMP2_");
            strcat(output2,id);

            int pidaux;
            int status;
            for (size_t i = 0; i < 5; i++)
            {
                filtros[iTask-1][i] = 0;
            }
            
            for (size_t k = 2; k < i; k++){
                if (strcmp(args[k],"alto") == 0){
                    users[0]++;
                    filtros[iTask-1][0]++;
                }else if (strcmp(args[k],"baixo") == 0){
                    users[1]++;
                    filtros[iTask-1][1]++;
                }else if (strcmp(args[k],"eco") == 0){
                    users[2]++;     
                    filtros[iTask-1][2]++;    
                }else if (strcmp(args[k],"rapido") == 0){
                    users[3]++;    
                    filtros[iTask-1][3]++;
                }else if (strcmp(args[k],"lento") == 0){
                    users[4]++;
                    filtros[iTask-1][4]++;
                }
            }
            
            save_state();
            
            if((pids[iTask-1] = fork()) == 0) { // tarefas ser executadas concorrentemente

                for (int j=2; j < i;j++){
                    wait(&status);
                    if((pidaux = fork()) == 0) { // filtros ser executados concorrentemente
                    
                        int ifd;
                        int ofd;
    
                        if(i==3){// se so tiver 1 filtro
                            ifd = open(args[0], O_RDONLY, 0666);
                            ofd = open(args[1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
    
                        }else if(j==2){ // se tiver mais que um filtro e for a primeira aplicacao de filtro
                            ifd = open(args[0], O_RDONLY, 0666);
                            ofd = open(output1, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    
                        }else if(j==i-1){ // se tiver mais que um filtro e for a ultima aplicacao de filtro
                            ifd = open(output2, O_RDONLY, 0666);
                            ofd = open(args[1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
                        }else{// aplicacoes de filtro intermedias
                            ifd = open(output1, O_RDONLY, 0666);
                            ofd = open(output2, O_CREAT | O_WRONLY, 0666);
    
                            tmp = output1;
                            output1 = output2;
                            output2 = tmp;
                        }
    
                        dup2(ifd,0);// stdin -> ifd
                        dup2(ofd,1);// stdout -> ofd
    
                        close(ifd);
                        close(ofd);
    
                        char* folder = malloc(MESSAGESIZE);
                        if (strcmp(args[j],"alto") == 0){
                        
                                strcat(folder,"./");
                                strcat(folder,argv[2]);
                                strcat(folder,"/aurrasd-gain-double");
                                 
                                
                        }else if (strcmp(args[j],"rapido") == 0){
                        
                                strcat(folder,"./");
                                strcat(folder,argv[2]);
                                strcat(folder,"/aurrasd-tempo-double");
    
                        }else if (strcmp(args[j],"lento") == 0){
                        
                                strcat(folder,"./");
                                strcat(folder,argv[2]);
                                strcat(folder,"/aurrasd-tempo-half");
    
                        }else if (strcmp(args[j],"eco") == 0){
                        
                                strcat(folder,"./");
                                strcat(folder,argv[2]);
                                strcat(folder,"/aurrasd-echo");
    
                        }else if (strcmp(args[j],"baixo") == 0){
                        
                            strcat(folder,"./");
                            strcat(folder,argv[2]);
                            strcat(folder,"/aurrasd-gain-half");
    
                        }
                        execl(folder,folder,NULL);
                    }else{
                        signal(SIGCHLD,chld_handler_filtro);
                    } 
                }  
                exit(0);
            }else{
                
                signal(SIGCHLD,chld_handler);
            }
            


            for(size_t j = 0; j < i; j++) free(args[j]);


        }else if(strncmp(buffer, "status",6) == 0){
            
            if(fork() == 0) {

            for (int i = 0; i < iTask; i++){
                if (!taskStatus[i]){ // se nao tiver acabado escreve   
                    char message[64];   
                    sprintf(message, "task #%d: %s\n",i+1,tasks[i]);

                    write(server_client_fifo, message, strlen(message));
                }
            
            }
            char message[64];
            for (int i = 0; i < nFiltros; i++)
            {
            
            sprintf(message, "filter %s: %d/%d\n",filtroNome[i],users[i],filtroMaxValue[i]);
            write(server_client_fifo, message, strlen(message));
            }
            
            exit(0);
            }
            
            
        }else {
            char buf[32];
            strcpy(buf, "\nComando inválido.\n\n");
            write(server_client_fifo, buf, strlen(buf));  
        }
        
        }
        free(buffer);
      
        close(server_client_fifo);
        close(client_server_fifo);
        
    
    }
    return 0;
}