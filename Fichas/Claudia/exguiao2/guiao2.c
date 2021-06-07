#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
///EX 1 
/*
int main (){
    printf ("pid: %d\n" , getpid()); // obtem o seu proprio pid 
    printf("filho-pid pai:%d\n", getppid()); //getppid obtem pid do pai 
    return 0;
}

//Ex 2 
int main(){
    pid_t pid ;
    pid =fork();
    int i=0;
    if (pid ==0){
        printf("filho %d " ,i);
        printf ("pid filho :%d e o seu pai %d\n",getpid(), getppid() );
        _exit(-1);
    }
    else {
        i--;
        printf("pai %d " ,i);
        printf ("pid pai  %d , e do seu pai: %d\n",getpid(), getppid());
    }
    return 0;
}

///EX 3 -espelha pelo processo filho antes de começar uma nova iteração 

int main(){
    pid_t pid ;
    int status;

    for (int a=0; a<10;a++ ){
      pid =fork();
        if (pid ==0){
            printf ("filho :%d ", a );
            printf ("processo do filho :%d\n" , getpid());
            _exit(0);
    }
        else {
            pid_t child =wait (&status);
            printf ("process pai  %d exited , exit code :%d\n",child, WEXITSTATUS(child));
            
    }
    }
    return 0;
}

//EX4 
//Espera pelo processo filho antes de começar uma nova iteraçao -processos concorrentes 
//faz forks lg no inicio
int main(){
    pid_t pid ;
    int status;

    for (int a=0; a<10;a++ ){
      pid =fork();
        if (pid ==0){
            printf ("filho :%d ", a );
            printf ("processo do filho :%d\n" , getpid());
            //sleep(a*10);
            _exit(a);
    }
    }
    for (int s=0; s<10;s++ ){
            pid_t child =wait (&status);
            printf ("process pai  %d exited , exit code :%d\n",child, WEXITSTATUS(child));
            
    }
    return 0;
}
*/
//EX 5 -em profundidade - um processo cria um filho e o filho outro e por ai em diante
int main(){
    pid_t pid =fork();
    int status;
    for (int a=0; a<10;a++ ){
      if (pid==0){
        printf ("pid :%d e o seu pai %d\n",getpid(),getppid());
        
      }else {
          pid_t terminated_pid= wait(&status);

          if(WIFEXITED(status)){
              printf("[pai] process %d exited.exit code :%d\n" ,terminated_pid, WEXITSTATUS(terminated_pid);
          }else {
              printf("pai process %d exited.\n", terminated_pid);
          }
          _exit(0);
      }
    }
    printf ("sai do ciclo %d" ,getpid());
return 0;
}
ggggggg


       