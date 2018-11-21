/*****************************************************************************
** Program name: listener.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: Function to receive port number and return socket data 
**				exchange handler in idle or "listening" mode. Also receives 
**				a  maximum queued connection count (backLog) and a pointer
**				to an int (sockfd) for storing the socket connection 
**				handler. Returns a 'connection' structure which contains
**				the data transfer handle and host address for the 
**				connection, with the help of ancillary function acceptConnection. 
**				(See acceptConnection.h and acceptConnection.c for additional
**				details.)
********************************************************************************/	

#include "listener.h"

typedef struct addrinfo addrinfo;

connection listener(char* portNumber, int backLog, int print)
{	
	//Socket file descriptor
	int sockfd;
	
	//addrinfo is an STL api struct to hold server address configuration	
	addrinfo hints, *servinfo, *p;
    
    //Used as a return value placeholder
    int rv;
	int yes = 1;

	//clears memory of hints (addrinfo struct)
    memset(&hints, 0, sizeof hints);
    
	//ai_family (addrinfo struct member) designates IP version.
	//--Here it is set to AF_INET to designate IPv4.
	hints.ai_family = AF_INET;
    
	//ai_socktype (addrinfo struct member) designates transport layer protocol.
	//--Here it is set to SCOK_STREAM to designate TCP.
	hints.ai_socktype = SOCK_STREAM;
	
	//Sets IP address automatically via getaddrinfo. (See note below.)
	hints.ai_flags = AI_PASSIVE;
	
	//The following if block uses getaddrinfo as follows: 
	//--hints is passed in pre-populated with connection configuration as 
	//--designated above, and servInfo is populated by the function in response. 
	//--Specifically, servInfo will point to a linked list of struct addrinfos, 
	//--with each containing a struct sockaddr to be used to establish connection 
	//--following the call to addrinfo. The first of these sockaddrs to provide 
	//--viable connection parameters will be used for the connection. 
	//--NULL and server port are also designated as the first 
	//--two parameters to the function. (The first parameter is NULL because 
	//--because the AI_PASSIVE flag is set. Otherwise, a hard-coded IP address 
	//--is passed to parameter 1 of getaddrinfo.) If an error occurs, it is handled 
	//--in the following for block, and if no viable connections are provided
	//--[in the linked list pointed to by servinfo], the if following that prints
	//--an error accordingly and exits execution.
	//--See reference: https://beej.us/guide/bgnet/html/multi/syscalls.html
	if((rv = getaddrinfo(NULL, portNumber, &hints, &servinfo) != 0))
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}
	
	for(p = servinfo; p != NULL; p = p->ai_next) 
	{
		//Set up socket
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) 
		{
			perror("Server: socket");
			continue;
		}
		//Set socket options
	    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
			  sizeof(int)) == -1) 
		{
			perror("setsockopt");
			exit(1);
        }
		//Bind socket to port number (provided to servInfo [and therefore p] via getaddrinfo above)
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("Server: Bind");
            continue;
        }

		break;
	}
	
	freeaddrinfo(servinfo); // all done with this structure
	
	//Print error binding message and quit, if no bind occurs
	if (p == NULL) 
	{
	   fprintf(stderr, "Server Failed to Bind.\n");
	   exit(1);
	}
	
	//print error and exit if error when attempting to listen
	//--on port-bound socket
	if (listen(sockfd, backLog) == -1) {
        perror("listen");
        exit(1);
    }
	
	if(print)
	{
		printf("\nWaiting for connection on port %s...\n", portNumber);
	}
	
	//sockfd is the socket handle for the TCP 'handshake' connection.
	//--After accepting a connection from the client, it will establish c.handle as
	//--the 'data' connection socket, via the acceptConnection function. 
	connection c = acceptConnection(sockfd, print);
		   
	return c;
}
