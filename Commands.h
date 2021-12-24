#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

void WelcomeMessage();
void TabCommand(char commandetapee[30], const char* tab[10]);
void Exit(char commandetapee[30], int r);
void ExitOrSignal(int status, char clock[30]);
void Time(char clock[30]);
