#include "Commands.h"

void WelcomeMessage(){ 
	//On utilise la fonction write qui remplace la fonction printf
	write(STDOUT_FILENO,"Welcome to ENSEA Tiny Shell.\n",strlen("Welcome to ENSEA Tiny Shell.\n")); 
	write(STDOUT_FILENO,"Pour afficher la date d'aujourd'hui, taper 'date' :\n",strlen("Pour afficher la date d'aujourd'hui, taper 'date' :\n"));
	write(STDOUT_FILENO,"Pour quitter, taper 'exit' :\n",strlen("Pour quitter, taper 'exit' :\n")); 
	write(STDOUT_FILENO,"enseash % ", strlen("enseash % "));
}

void TabCommand(char commandetapee[30], const char* tab[10]){ //Crée la liste d'arguments pour execvp
	char* p = strtok(commandetapee," "); //Coupe la chaîne de caractère pour chaque espace
	int i = 0;
	while (p != NULL) { //Cette boucle remplit tab avec les éléments de p
		tab[i] = p;
		p = strtok(NULL, " ");
		i += 1;
	}
	tab[i] = NULL; //On n'oublie pas NULL en dernier argument de tab pour qu'execvp fonctionne correctement
}
	
void Exit(char commandetapee[30], int r){
	if (strncmp(commandetapee, "exit\n",5)==0){ // On compare ce qui a été écrit avec "exit(+entrée)"
		write(STDOUT_FILENO,"Bye bye..\n", strlen("Bye bye..\n"));
		exit(EXIT_SUCCESS);                     // On quitte si l'utilisateur a écrit exit et pressé la touche Entrée
	}
	if (r == 0){                                //On compare ce qui a été écrit avec l'appui de "CTRL+D"
		write(STDOUT_FILENO,"\nBye bye..\n", strlen("\nBye bye..\n"));
		exit(EXIT_SUCCESS);                     // On quitte si l'utilisateur a pressé "CTRL+D"
	}
}

void ExitOrSignal(int status, char clock[30]){ //Affiche exit s'il n'y a pas eu d'interruption et sign (pour signal) s'il y en a eu
	char stat[30];
	sprintf(stat, "%d",WEXITSTATUS(status)); //Enregistre la vealeur de sortie du fils
		
	char sig[30];
	sprintf(sig, "%d",WTERMSIG(status)); //Enregistre le numéro du signal qui a causé l'arrêt du fils
	
	if (WIFEXITED(status)){//Si le fils a terminé normalement, on écrit la valeur d'exit et le temps écoulé
		write(STDOUT_FILENO,"enseash [exit:", strlen("enseash [exit:"));
		write(STDOUT_FILENO,stat, strlen(stat));
		write(STDOUT_FILENO,"|", strlen("|"));
		write(STDOUT_FILENO,clock, strlen(clock));
		write(STDOUT_FILENO,"ms] %", strlen("ms] %"));
	}
	else if (WIFSIGNALED(status)){//Si le fils a terminé prématurément, on écrit la valeur du signal et le temps écoulé
		write(STDOUT_FILENO,"enseash [sign:", strlen("enseash [sign:"));
		write(STDOUT_FILENO,sig, strlen(sig));
		write(STDOUT_FILENO,"|", strlen("|"));
		write(STDOUT_FILENO,clock, strlen(clock));
		write(STDOUT_FILENO,"ms] %", strlen("ms] %"));
	}
}

void Time(char clock[30]){
	struct timespec time2 = {0,0};
	clockid_t clk_id = CLOCK_MONOTONIC;

	clock_gettime(clk_id,&time2);
		
	sprintf(clock, "%ld",time2.tv_nsec/1000000);//On divise par 1M pour que le résultat soit en ms
}
		
//Question 7 : Nous avons compris comment fonctionnait stdin avec < et stdout avec > mais nous n'avons pas encore bien compris ce que la fonction doit faire
