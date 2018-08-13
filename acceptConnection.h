/**************************************************
** Program name: acceptConnection.h
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: Header file for acceptConnection.c
***************************************************/

#ifndef acceptConnection_h
#define acceptConnection_h

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "get_in_addr.h"
#include "connection.h"

typedef struct connection connection;

connection acceptConnection(int sockfd, int print);

#endif /* acceptConnection_h */