#include "gettftp.h"
#include "createRRQ.h"

//To create the reading request, we need to respect the header of the TFTP package on RFC1350
//Here, in the right order, we might put the OP code of 01 at first, then we put the name of the 
//file, one zero, the byte mode and one 0 to indicate the end of the word

char* createRRQ(char* file, char* RRQ, char* mode, char* BlkSize, char* blocksize){
    RRQ[0]=0;
    RRQ[1]=1;
    strcpy(RRQ+2, file);
    RRQ[2+strlen(file)]=0;
    strcpy(RRQ+3+strlen(file),mode);
    RRQ[3+strlen(file)+strlen(mode)]=0;
    strcpy(RRQ+4+strlen(file)+strlen(mode),BlkSize);
	RRQ[4+strlen(file)+strlen(mode)+strlen(BlkSize)]=0;
	strcpy(RRQ+5+strlen(file)+strlen(mode)+strlen(BlkSize),blocksize);
	RRQ[5+strlen(file)+strlen(mode)+strlen(BlkSize)+strlen(blocksize)]=0;
    return RRQ;
}
