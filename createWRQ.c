#include "gettftp.h"
#include "createWRQ.h"

//To create the writing request, we need to respect the header of the TFTP package on RFC1350
//Here, in the right order, we might put the OP code of 01 at first, then we put the name of the 
//file, one zero, the byte mode and one 0 to indicate the end of the word

char* createWRQ(char* file, char* WRQ){
    WRQ[0]=0;
	WRQ[1]=2;
	strcpy(WRQ+2,file);
	WRQ[2+strlen(file)]=0;
	strcpy(WRQ+3+strlen(file),"octet");
	WRQ[3+strlen(file)+strlen("octet")]=0;
    return WRQ;
}
