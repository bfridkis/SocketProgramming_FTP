/**********************************************************************************
** Program name: acceptConnection.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: Function to wait on an established ("handshake") socket for a 
**				connection request, and to return a new ("data transfer") socket 
**				based on the established connection. The while loop continues to 
**				wait until a connection is established successfully. 
**				(Basically a wrapper function for accept()). Returns a 'connection' 
**				struct that contains the handle integer for the connection and the 
**				host name. (See acceptConnection.h for connection struct defintion.)
************************************************************************************/	

#include "acceptConnection.h"

connection acceptConnection(int sockfd, int print)
{
	connection c;
	
	//data type to hold address size info for the client after accepting the connection
	socklen_t sin_size;
	//sockaddr_storage is an STL api struct to hold incoming (client) connection information
	//--printed to screen after successful connection
	struct sockaddr_storage client_addr;
	while(1)
	{
		sin_size = sizeof client_addr;
		c.handle = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if (c.handle == -1) 
		{
			perror("accept");
			continue;
		}
		else
		{
			break;
		}			
	}
	
	char destinationHostName[256], destinationHostService[256];
	inet_ntop(AF_INET,
		get_in_addr((struct sockaddr *)&client_addr),
		c.hostAddress, sizeof c.hostAddress);
	
	//See https://beej.us/guide/bgnet/html/multi/getnameinfoman.html
	getnameinfo((struct sockaddr *)&client_addr, sizeof client_addr, 
				 destinationHostName, sizeof destinationHostName, 
				 destinationHostService, sizeof destinationHostService, 0);		
	
	strcpy(c.hostName, destinationHostName);
	
	if(print)
	{	
		printf("\nGot connection from %s (%s)\n", 
				destinationHostName, c.hostAddress);
	}
	
	return c;
}

/*************************************************************************
   References 
** https://beej.us/guide/bgnet/html/multi/syscalls.html
** https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleserver
**************************************************************************/