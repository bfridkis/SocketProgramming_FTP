/*********************************************
** Program name: listener.h
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: Header file for listener.c
*********************************************/

#ifndef listener_h
#define listener_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>

#include "acceptConnection.h"

connection listener(char* portNumber, int backLog, int print);

#endif /* listener_h */