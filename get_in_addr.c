/**********************************************************************************************************
** Program name: get_in_addr.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: Utility function to get remote host (client) scoket address information.
				Copied directly from: https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleserver
************************************************************************************************************/

#include "get_in_addr.h"

//get sockaddr, IPv4 or IPv6:
//Returns a void pointer so either 
//--IPv4 or IPv6 socket input address
//--can be accomodated.
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
