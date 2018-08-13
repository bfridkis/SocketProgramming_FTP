/*******************************************
** Program name: sigchld_handler.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: Header to sigchld_handler.
********************************************/

#ifndef sigchld_handler_h
#define sigchld_handler_h

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

void sigchld_handler(int s);

#endif /* sigchld_handler_h */