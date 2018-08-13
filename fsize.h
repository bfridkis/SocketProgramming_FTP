/*********************************************
** Program name: fsize.h
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: Header file for fsize.c
*********************************************/

#ifndef fsize_h
#define fsize_h

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

off_t fsize(const char *filename);

#endif /* fsize_h */