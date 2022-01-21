#include "puttftp.h"
#include "recupServer.h"
#include "createWRQ.h"
#include "createACK.h"

int main(int argc, char*argv[]){
	
    struct addrinfo *result;
    char *host,*file;
    char buf[BUFSIZE];
	int r,sfd;
	uint16_t blockNumber = 0;
	int lenWRQ = 4+strlen(argv[2])+strlen("octet");
    
    if (argc==3){
		host = argv[1];
		file = argv[2];
	}
	else{
		fprintf(stderr, "Usage: %s host [service]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	result = recupServer(host);
	
	//We create a socket to communicate with the server
	sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	
	if (sfd==-1){
		printf("Error in the initialization of the socket");
		exit(EXIT_FAILURE);
	}
	
	//Now, the communication is established so we can create a writing request
    //in order to send a file to the server
	
	char WRQempty[lenWRQ];
    char* WRQ = createWRQ(file, WRQempty);
	
	//Sending of the request
	int send;
	send = sendto(sfd,WRQ,lenWRQ,0,result->ai_addr,result->ai_addrlen);
	if(send==-1){
		printf("Error in sending the request\n");
		exit(EXIT_FAILURE);
	}
	
	//Receipt of the acknowledgement of the writing request
	int reception = recvfrom(sfd,buf,BUFSIZE,0,result->ai_addr,&(result->ai_addrlen));
	if(!(buf[0]==0 && buf[1]==4 && buf[2]==0 && buf[3]==0) || reception ==-1){
		printf("BUF %d\n",buf[0]);
		printf("BUF %d\n",buf[1]);
		printf("BUF %d\n",buf[2]);
		printf("BUF %d\n",buf[3]);
		printf("Error receipt server writing request\nReception : %d\n",reception);
		
		exit(EXIT_FAILURE);
	}
	
	//Issuing the file
	int f = open(file,O_RDONLY,S_IRUSR);
	int acquite = 1;
	do {
		if(acquite){
			blockNumber = blockNumber + 1;
			r = read(f,buf+4,BUFSIZE-4);
		
			if (r == -1){
				printf("Error reading file\n");
				exit(EXIT_FAILURE);
			}	
		printf("BN %d\n", blockNumber);
		
		buf[1]=3;
		buf[2]=(uint8_t)(blockNumber/256);//2-bytes packet number
	
		buf[3]=(uint8_t)(blockNumber%256);//Cast to uint8_t required otherwise the char type of buf negatively signs the value from 128.
		
		
		printf("BN 2 %d\n", (uint8_t)buf[2]);
		printf("BN 1 %d\n", (uint8_t)buf[3]);
		}
		int sendDAT = sendto(sfd,buf,4+r,0,result->ai_addr,result->ai_addrlen);
		if(sendDAT==-1){
			printf("Error data sending\n");
			exit(EXIT_FAILURE);
		}
		else{
			printf("Number of bytes sent : %d in the packet %d\n",sendDAT,blockNumber);
		}
		
		reception = recvfrom(sfd,buf,4,0,result->ai_addr,&(result->ai_addrlen));
		if(!(buf[0]==0 && buf[1]==4 && (uint8_t)buf[2]==(uint8_t)(blockNumber/256) && (uint8_t)buf[3]==(uint8_t)(blockNumber%256))){
			printf("No acknowledgement of receipt received for package %d\n",blockNumber);
			acquite=0;
		}
		else{
			printf("Acknowledgement of receipt received for package %d\n",blockNumber);
			acquite=1;
		}
		
	}while(r == BUFSIZE-4 || acquite == 0);
		
	close(f);
	
	exit(EXIT_SUCCESS);
}
