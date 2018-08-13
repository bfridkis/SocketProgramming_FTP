/**********************************************************************************
** Program name: bufferClear.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 7/19/2018
** Description: Function to manually clear the input buffer up to (and including)
**				the next newline character.
***********************************************************************************/

#include "bufferClear.h"

void bufferClear(char *bufferCheckString, char *bufferDeplete)
{
	if(bufferCheckString[strlen(bufferCheckString) - 1] != '\n')
	{
		while(bufferDeplete[strlen(bufferDeplete) - 1] != '\n')
		{
			fgets(bufferDeplete, 1024, stdin);
		}
		bufferDeplete[0] = '\0';
	}
}