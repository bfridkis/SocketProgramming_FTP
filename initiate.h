/*********************************************
** Program name: initiate.h
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/10/2018
** Description: Header file for initiate.c
*********************************************/

#ifndef initiate_h
#define initiate_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>

#include "get_in_addr.h"
#include "connection.h"

void *initiate(char* portNumber, struct connection* c, int *sockfd, int print);

#endif /* initiate_h */