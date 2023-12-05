//INFINITO

//Author
// Carolina Minyu De Jesús and Lidia Villarreal

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
struct f{
	int forkH1, forkH2, forkN2, forkH3, forkN3, forkH4,forkP,forkDestino;
}f;

int vueltas;

void terminarHijos (int signum){

    int statusH2,statusH1,statusH3,statusH4;

    if (kill(f.forkH1, SIGTERM)==-1) perror("Error al matar H1"); 
    if (kill(f.forkH2, SIGTERM)==-1) perror("Error al matar H2");
    if (kill(f.forkH3, SIGTERM)==-1) perror("Error al matar H3");
    if (kill(f.forkH4, SIGTERM)==-1) perror("Error al matar H4");

    if(waitpid(f.forkH1,&statusH1,0)==-1)
        perror("Error.\n");
    else {
        printf("Proceso H1 ha terminado.\n");
    }

    if(waitpid(f.forkH2,&statusH2,0)==-1)
        perror("Error.\n");
    else {
        printf("Proceso H2 ha terminado.\n");
    }

    if(waitpid(f.forkH3,&statusH3,0)==-1)
        perror("Error.\n");
    else {
        printf("Proceso H3 ha terminado.\n");
    }

    if(waitpid(f.forkH4,&statusH4,0)==-1)
        perror("Error.\n");
    else {
        printf("Proceso H4 ha terminado.\n");
    }
    printf ("La senial ha dado %d vueltas", vueltas);
    exit(0);
}

void terminarN2 (int signum){
    int statusN2;
    if (kill(f.forkN2, SIGTERM)==-1) perror("Error al matar N2");
	if(waitpid(f.forkN2,&statusN2,0)==-1)
        perror("Error.\n");
    else {
        printf("Proceso N2 ha terminado.\n");
    }
    exit(0);
}

void terminarN3 (int signum){
    int statusN3;
    if (kill(f.forkN3, SIGTERM)==-1) perror("Error al matar N3");

    if(waitpid(f.forkN3,&statusN3,0)==-1)
        perror("Error.\n");
    else {
        printf("Proceso N3 ha terminado.\n");
    }
    exit(0);
}

void manejadoraSIGUSR1(int s) {
    if (kill(f.forkDestino, SIGUSR1)==-1) 
		perror("Error\n");   	
}


void manejadoraPadre(int s) {
    //si recibe SIGUSR1 el padre envia SIGUSR2 
    if(s==SIGUSR1){
    	if (kill(f.forkH2, SIGUSR2)==-1) 
			perror("Error\n");
	}
	//si recibe SIGUSR2 el padre envia SIGUSR1
    if(s==SIGUSR2){
    	if (kill(f.forkH3, SIGUSR1)==-1) 
			perror("Error\n");
	//incrementar las vueltas
	vueltas++;
	}
}

void manejadoraSIGUSR2(int s) {
    if (kill(f.forkDestino, SIGUSR2)==-1) 
		perror("Error \n");     
}


int main(int argc, char *argv[])
{
   
    struct sigaction sa;
    sigset_t conj;
    int flag=0;
    
    f.forkP=getpid();

	 
    if(-1 == sigfillset(&conj) ) return -1; 
    if(-1 == sigprocmask(SIG_BLOCK, &conj, NULL) ) return -1;
    
    switch (f.forkH1 = fork())
    {
    case -1: //ERROR
        perror("prueba\n");
        return -1;
    case 0: //HIJO 1
        printf("Soy H1, PID %d, padre %d\n", getpid(), getppid());
        f.forkDestino=f.forkP;
        sigdelset (&conj,SIGUSR2); //Quitamos SIGUSR2 de conj
		sigdelset (&conj,SIGTERM);
        sa.sa_handler = manejadoraSIGUSR2;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags=0;
        if(sigaction(SIGUSR2, &sa, NULL)==-1) return -1;

        while (flag == 0)
            sigsuspend (&conj);    
        break;

    default://Padre
        printf ("Soy el padre PID %d\n", f.forkP);
        switch (f.forkH4 = fork())
        {
        case -1:
            perror("prueba\n");
            return -1;
        case 0: //H4
            printf("Soy H4, PID %d, padre %d \n", getpid(), getppid ());
            f.forkDestino=f.forkP;
            sigdelset (&conj,SIGUSR1); //Quitamos SIGUSR1 de conj 
            sigdelset (&conj,SIGTERM);
            f.forkDestino=f.forkP;
            sa.sa_handler = manejadoraSIGUSR1;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags=0;
            if(sigaction(SIGUSR1, &sa, NULL)==-1) return -1; 
    
            while (flag == 0)
            	sigsuspend (&conj);
            
            break;

        default: //Padre
            switch (f.forkH2 = fork())
            {
            case -1:
                perror("prueba\n");
                return -1;
            case 0: //H2            
	            sigdelset (&conj,SIGUSR2); //Quitamos SIGUSR2 de conj 
		        sigdelset (&conj,SIGTERM);
				sa.sa_handler = manejadoraSIGUSR2;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags=0;
                if(sigaction(SIGUSR2, &sa, NULL)==-1) return -1;
                   
                switch (f.forkN2 = fork())
                {
                case -1:
                    perror("prueba\n");
                    break;
                case 0:
                    printf("Soy N2, PID %d, padre %d \n", getpid(), getppid ());
                    f.forkDestino=f.forkH1;
                    while (flag == 0)
		            	sigsuspend (&conj);
                    break;
                } //fin switch N2
                printf("Soy H2, PID %d, padre %d \n", getpid(), getppid ());
                f.forkDestino=f.forkN2;
                sa.sa_handler = terminarN2;
                if(sigaction(SIGTERM, &sa, NULL)==-1) return -1;
                while (flag == 0)
		            sigsuspend (&conj);
            	break;

            default:  
                switch (f.forkH3 = fork())  //crear H3
                {
                case -1:
                    perror("prueba\n");
                    return -1;
                case 0: //H3                    
		            sigdelset (&conj,SIGUSR1); //Quitamos SIGUSR1 de conj 
    				sigdelset (&conj,SIGTERM);
					sa.sa_handler = manejadoraSIGUSR1;
		            sigemptyset(&sa.sa_mask);
		            sa.sa_flags=0;
		            if(sigaction(SIGUSR1, &sa, NULL)==-1) return -1; 
                  
                    switch (f.forkN3 = fork())
                    {
                    case -1:
                        perror("prueba\n");
                        return -1;
                    case 0: //N3
                        printf("Soy N3, PID %d, padre %d\n", getpid(), getppid());
                        f.forkDestino=f.forkH4;
                        while (flag == 0)
		            		sigsuspend (&conj);
                        break;
                    } //fin switch N3
                	printf("Soy H3, PID %d, padre %d \n", getpid(), getppid ());
                	f.forkDestino=f.forkN3;
                	sa.sa_handler = terminarN3;
                	if(sigaction(SIGTERM, &sa, NULL)==-1) return -1;
					while (flag == 0)
		            	sigsuspend (&conj); 
	                break; 

                default: //Padre		
                    sigdelset (&conj,SIGUSR1); //Elimina señal1 del conjunto
                    sa.sa_handler=manejadoraPadre;
                    sigfillset(&sa.sa_mask); //Se llena el conjunto con todas las señales
                    sa.sa_flags=0;
                    if(sigaction(SIGUSR1, &sa, NULL)==-1) return -1; 
                    
                    sigdelset (&conj,SIGUSR2);
                    sa.sa_handler=manejadoraPadre;
                    sigfillset(&sa.sa_mask);
                    sa.sa_flags=0;
                    if(sigaction(SIGUSR2, &sa, NULL)==-1) return -1; 
                  
                  	if(sigprocmask(SIG_UNBLOCK,&conj,NULL)==-1); //Esta función desbloquea las señales especificadas en el conjunto conj para el proceso actual. En este caso, SIG_UNBLOCK se utiliza para indicar que se deben desbloquear las señales especificadas.
                    sigfillset(&sa.sa_mask);
                    sa.sa_handler=terminarHijos;
                    sa.sa_flags=0; 
					if (sigaction(SIGALRM, &sa,NULL)==-1) return -1;
                    alarm (25);

					//se inicia el envio de seniales
					if(kill(f.forkH3, SIGUSR1)==-1)
						perror("Error");

                    while (flag == 0){
                    	sigsuspend (&conj);
                    	vueltas ++;
					}
                    //se hace todo durante 25s y cuando pasan, se salta a terminarHijos

                }                   //fin switch H3
            }  
                                   //fin switch H2
		}  
                               //fin switch H4
	} 
                                //fin switch
                                
    return 0;
}









