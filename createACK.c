#include "gettftp.h"
#include "createACK.h"

//To create the acknowledgement of receipt, we need to respect the header of the TFTP package on RFC1350
//Here, in the right order, we might put the OP code of 04 at first and then we put the block number in ACK which is
//the echo of the block number of the data packet to be acknowledged so we take buf[2] and buf[3]

char* createACK(char* buf, char* ACK){
    ACK[0]=0;
	ACK[1]=4;
	ACK[2]=buf[2];
	ACK[3]=buf[3];
	return ACK;
}
