#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
#include "Commands.h"

int main(int argc, char* argv[]){
	char commandetapee[30];
	pid_t pid;
	int r;
	int status;
	const char* tab[10];
	char clock[30];
	
	WelcomeMessage();
	
	while(1){                                       
		r = read(STDIN_FILENO, commandetapee, 30);//Permet de récupérer ce que l'utilisateur écrit dans le prompt
		TabCommand(commandetapee,tab);
		Exit(commandetapee,r);
        pid = fork();//On crée un fils afin d'utiliser la fonction execvp sans que cela interrompe la machine virtuelle
		if (pid !=0){//Code père
			if (pid == -1){//On vérifie si le fork a produit une erreur
				int fork_err = errno;
				write(STDOUT_FILENO,"Fork impossible\n", strlen("Fork impossible\n"));
				if (fork_err == EAGAIN){
					write(STDOUT_FILENO,"(Too much processes)\n", strlen("(Too much processes)\n"));
				} 
			}
			else {//Sinon, on attend que le fils termine
				wait(&status);
			}
		} else if (pid==0){//Code fils
			commandetapee[r-1]='\0';                              
			int ex = execvp(tab[0],tab);
			if (ex<0){	
				write(STDOUT_FILENO,"Commande non trouvée\n", strlen("Commande non trouvée\n"));
			}
			exit(EXIT_FAILURE);
		}
		Time(clock);
		ExitOrSignal(status, clock);
	}
}
