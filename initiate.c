/*****************************************************************************
** Program name: listener.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/10/2018
** Description: Function to receive port number and control connection address
**				and name information (via a 'connection' struct), and load a
**				a socket file descriptor for data connection on said port 
**				number (into arr[0]). If an error occurs, arr[1] is set to 1 to
**				indicate binding failure. 
********************************************************************************/	

#include "initiate.h"

typedef struct addrinfo addrinfo;
typedef struct connection connection;

void *initiate(char* portNumber, connection* c, int *sockfd, int print)
{	
	//Set error flag to 0 initially.
	sockfd[1] = 0;
	
	//addrinfo is an STL api struct to hold server address configuration	
	addrinfo hints, *servinfo, *p;
    
    //Used as a return value placeholder
    int rv;

	//clears memory of hints (addrinfo struct)
    memset(&hints, 0, sizeof hints);
    
	//ai_family (addrinfo struct member) designates IP version.
	//--Here it is set to AF_INET to designate IPv4.
	hints.ai_family = AF_INET;
    
	//ai_socktype (addrinfo struct member) designates transport layer protocol.
	//--Here it is set to SCOK_STREAM to designate TCP.
	hints.ai_socktype = SOCK_STREAM;
	
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
	if((rv = getaddrinfo(c->hostAddress, portNumber, &hints, &servinfo) != 0))
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}
	
	for(p = servinfo; p != NULL; p = p->ai_next) 
	{
		//Set up socket
		if ((sockfd[0] = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) 
		{
			perror("Socket Error");
			continue;
		}
		
		//Connect to socket on port number (provided to servInfo [and therefore p] via getaddrinfo above)
		//--i.e. Establish data connection for data transfer on client-specified data port
		if (connect(sockfd[0], p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd[0]);
            perror("Port Bind Error/Port In Use");
            continue;
        }

		break;
	}
	
	if (p == NULL) {
        fprintf(stderr, "Failed to Connect on Port %s\n", portNumber);
        sockfd[1] = 1;
    }
	
	freeaddrinfo(servinfo); // all done with this structure
	
	if(sockfd[1] == 0 && print == 1)
	{
		printf("Connecting to %s:%s\n", c->hostName, portNumber);
	}
		   
	return sockfd;
}