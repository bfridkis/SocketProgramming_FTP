/**********************************************
** Program name: connection.h
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: connection struct definition.
**********************************************/

#ifndef connection_h
#define connection_h

struct connection
{
	char hostName[512];
	char hostAddress[INET_ADDRSTRLEN];
	int handle;
	int sockfd;
};

#endif /* connection_h */