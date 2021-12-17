#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <unistd.h>
#include <sys/types.h>
#include "time.h"
#include <sys/wait.h>


int main(int argc, char* argv[]){
	char commandetapee[30];
	pid_t pid;
	int ex;
	int r;
	int status;
	write(STDOUT_FILENO,"Welcome to ENSEA Tiny Shell.\n",strlen("Welcome to ENSEA Tiny Shell.\n")); //On utilise la fonction write qui remplace la fonction printf
	write(STDOUT_FILENO,"Pour afficher la date d'aujourd'hui, taper 'date' :\n",strlen("Pour afficher la date d'aujourd'hui, taper 'date' :\n"));
	write(STDOUT_FILENO,"Pour quitter, taper 'exit' :\n",strlen("Pour quitter, taper 'exit' :\n")); // Cette foncion prend 3 arguments : le fichier dans lequel on veut écrire, ce que l'on veut écrire et sa taille
	write(STDOUT_FILENO,"enseash % ", strlen("enseash % "));
	while(1){                                       // Boucle While qui fonctionne en permanence
		r = read(STDIN_FILENO, commandetapee, 30);  // Permet de remplacer fscanf et de lire ce que l'utilisateur écrit dans la fenêtre de commande
		if (strncmp(commandetapee, "exit\n",5)==0){ // Compare ce qui a été écrit avec "exit(+entrée)"
			write(STDOUT_FILENO,"Bye bye..\n", strlen("Bye bye..\n"));
			exit(EXIT_SUCCESS);                     // Quitte si l'utilisateur a écrit exit et pressé la touche Entrée
		}
		if (r == NULL){
			write(STDOUT_FILENO,"Bye bye..\n", strlen("Bye bye..\n"));
			exit(EXIT_SUCCESS);
		}
        pid = fork();
		if (pid !=0){
			wait(&status);
		} else if (pid==0){  
			commandetapee[r-1]='\0';                              
			ex = execlp(commandetapee, commandetapee, (char *) NULL);
			if (ex<0){	
				write(STDOUT_FILENO,"Commande non trouvée\n", strlen("Commande non trouvée\n"));
			}
			write(STDOUT_FILENO,"enseash % ", strlen("enseash % "));
			exit(EXIT_FAILURE);
		}
	}
}
