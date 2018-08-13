/*****************************************************
** Program name: getServerPort.h
** Class name: CS372
** Author: Ben Fridkis
** Date: 7/19/2018
** Description: Header file for sendCurrentDirectory.c
******************************************************/

#ifndef getCurrentDirectory_h
#define getCurrentDirectory_h

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

/* #include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h> */

void getCurrentDirectory(char* directory);

#endif /* getCurrentDirectory_h */