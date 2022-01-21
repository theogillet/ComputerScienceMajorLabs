#include "gettftp.h"
#include "recupServer.h"
#include "createRRQ.h"
#include "createACK.h"
 
int main(int argc, char*argv[])
{
      char *host,*file,*blocksize;
      int sfd;
      struct addrinfo* result;
      int reception;
      int BUFSIZE;
      
      //We take the arguments from the request writen in the terminal if there are enough 
      //and we explain how to make the request if there isn't enough arguments writen in
      
      if (argc==4){
		 host = argv[1];
         file = argv[2];
         blocksize = argv[3];
         BUFSIZE=4+strtol(argv[3],NULL,10);
      }
      
      char buf[BUFSIZE];
      
      if (argc<4){
         fprintf(stderr, "Usage: %s host file blocksize\n", argv[0]);
         exit(EXIT_FAILURE);
      }
     
      result = recupServer(host);
      
      //We create a socket to communicate with the server
      sfd = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
      
      if (sfd==-1){
		printf("Error in the initialization of the socket\n");
		exit(EXIT_FAILURE);
	  }
     
      //Now, the communication is established so we can create a reading request
      //in order to ask the server to send us a file
      int nSend;
      char mode[] = "octet";
      char BlkSize[] = "blksize";
      int lenRRQ = 6+strlen(file)+strlen(mode)+strlen(BlkSize)+strlen(blocksize);
      char RRQempty[lenRRQ];
      char* RRQ = createRRQ(file, RRQempty,mode,BlkSize,blocksize);
      
      //And we send this request with the use of the sendto function
      nSend=sendto(sfd,RRQ,lenRRQ,0,result->ai_addr,result->ai_addrlen);
      if (nSend != -1){
		printf("number of bits sent = %d\n",nSend);
	  } else {
		  printf("Error in tosend for RRQ\n");
		  exit(EXIT_FAILURE);
	  }
	  
	  recvfrom(sfd,buf,BUFSIZE,0,result->ai_addr,&(result->ai_addrlen));
	  int i=0;
	  for (i=0;i<strlen(blocksize);i++){//The loop permits to verify that the option got accepted
		 if(!(buf[3+strlen(BlkSize)+i]=blocksize[i])){
			printf("blocksize refused\n");
			exit(EXIT_FAILURE);
		 }
	  }
      
      //Then, we create an acknowledgement of receipt to confirmation the receipt of the data
      //Because the length of the file can be larger than the buffer, we create a while loop to copy 
      //the information splitted in as many parts as we need to to reach the file size with the buffer size
      int lenACK=4;
      char ACKempty[lenACK];
      ACKempty[0] = 0;
      ACKempty[1] = 4;
      ACKempty[2] = 0;
      ACKempty[3] = 0;
	
	  sendto(sfd,ACKempty,lenACK,0,result->ai_addr,result->ai_addrlen);//the reception of the QACK is acknowledged
	
      //Reception of the file
	  int f = open(file,O_CREAT|O_RDWR|O_APPEND, S_IRUSR|S_IWUSR);
	  
	  do {
		reception = recvfrom(sfd,buf,BUFSIZE,0,result->ai_addr,&(result->ai_addrlen));
		printf("Reception of %d bytes\n", reception);
		int wVerif=write(f,buf+4,BUFSIZE-4);
		if (wVerif==-1){
			printf("Error of writing\n");
			exit(EXIT_FAILURE);
		}
		char* ACK = createACK(buf, ACKempty);
		int sendACK = sendto(sfd,ACK,lenACK,0,result->ai_addr,result->ai_addrlen);
		if(sendACK==-1){
			printf("Error in sendACK\n");
			exit(EXIT_FAILURE);
		}
		else{
			printf("Number of bytes sent : %d\n",sendACK);
		}
	  }while(reception == BUFSIZE);
	
	  if (reception==-1){
		printf("Reception error\n");
		exit(EXIT_FAILURE);
	  }
	  close(f);
	  exit(EXIT_SUCCESS);
}
