/**********************************************************************************
** Program name: getServerPort.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: Gets server port based on command line input or, if no command line
**				input is specified, via user prompt.(Server port needs to be
**				specified as argv[1].)
***********************************************************************************/

#include "getServerPort.h"

void getServerPort(char* serverPort, char* cl_arg1)
{
	//serverPort can be specified on command line when launching program, or is prompted if no
	//--command line specification is provided at launch.
	//serverPort has a max character count of 7. An addtional two bytes are reserved for '\n' and '\0'.
	//--('\n' needed when fgets is used to determine serverPort. See below.)
	
	//When fgets is used (i.e. when serverPort is not determined directly from command line):
	//--An extra byte is needed in the destination character array because fgets brings in the 
	//--newline as long as the max input (specified in parameter 2 of fgets) is not reached . This is used
	//--here for input validation in the sense that a last character that is not an '\n' after  
	//--data entry indicates the user's input exceeded the input character maximum, and is thus handled 
	//--accordingly. 
	
	char bufferDeplete[1024] = "\0";
	if(cl_arg1 != NULL)
	{
		strcpy(serverPort, cl_arg1);
	}
	else
	{
		printf("\nEnter server port number.\n--> ");
		fgets(serverPort, 7, stdin);
		bufferClear(serverPort, bufferDeplete);
		//Remove trailing '\n'
		serverPort[strlen(serverPort) - 1] = '\0';
	}
	while(serverPort == 0 || atoi(serverPort) < 3000 || atoi(serverPort) > 65535 || 
			(strlen(serverPort) != 4 && strlen(serverPort) != 5))
	{
		printf("\nEnter server port number. (Must be integer between 3000 and 65535)\n--> ");
		fgets(serverPort, 7, stdin);
		bufferClear(serverPort, bufferDeplete);
		//Remove trailing '\n'
		serverPort[strlen(serverPort) - 1] = '\0';
	}
}