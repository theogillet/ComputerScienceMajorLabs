#include "gettftp.h"
#include "recupServer.h"

struct addrinfo* recupServer(char* host){
	
    struct addrinfo hints;
    char sbuf[BUFTAILLE], hbuf[BUFTAILLE];
    int s;
	struct addrinfo* result;
	
	memset(&hints, 0, sizeof(hints));
	
	//The following data allows us to orient the results we want to have on the server
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;
	hints.ai_protocol = IPPROTO_UDP;
    
    //To use the getaddrinfo() to obtain the server's address we need:
    //-the address (the host)
    //-the port (which is 1069 for the server given in the tp) 
    //-the hints 
    //-the result which is the address of the first record answered by the return call
	s = getaddrinfo(host, "1069", &hints, &result);
	
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
    getnameinfo(result->ai_addr, result->ai_addrlen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
    printf("IP : %s\nport : %s \nIPVx : %d\n", hbuf, sbuf, result->ai_family);
     
    return result;
}
