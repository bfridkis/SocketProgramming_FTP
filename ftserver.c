/***************************************************************************
** Program name: ftserver.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/8/2018
** Description: Server-side chat program to demonstrate socket programming.
**				Serves directory contents and files.
****************************************************************************/

#include "getServerPort.h"
#include "getCurrentDirectory.h"
#include "listener.h"
#include "initiate.h"
#include "fsize.h"

void sigchld_handler(int s);
void getCurrentDirectory(char* directory);

typedef struct addrinfo addrinfo;
//typedef struct sockaddr_storage sockaddr_storage;

#define BACKLOG 10     // Number of pending connections queue will hold

int main(int argc, char *argv[])
{
	//Load server port number from command line (or, if unspecified, via user prompt)
	char serverPort[7];
	getServerPort(serverPort, argv[1]);
	
	//Establish a control connection communication handle
	//(See listener.c and acceptConnection.c for additional details.)
	connection cc = listener(serverPort, BACKLOG, 1);
	
	//Declare 2-digit response code string
	char responseCode[2];
	
	do
	{
		//Receive command string from client. This will include the filename requested (if applicable),
		//--followed by the data port specification and command type (valid types are -l and -g).
		char buf[256] = "\0";
		recv(cc.handle, buf, 256, 0);
		//printf("\nClient Request String: %s\n", buf);	
		
		//If command is invalid, notify client on control connection
		if(buf[strlen(buf) - 1] != 'g' && buf[strlen(buf) - 1] != 'l'
			&& buf[strlen(buf) - 1] != 'G' && buf[strlen(buf) - 1] != 'L')
		{
			printf("\n'-%c' command invalid.\nSending error code IV to %s on port %s\n\n",
					buf[strlen(buf) - 1], cc.hostName, serverPort);
			
			strcpy(responseCode, "IV");
			send(cc.handle, responseCode, strlen(responseCode), 0);	
		}
		else
		{
			//Parse the data port specification from the client command string
			char dataTransferPort[6];
			char command = buf[strlen(buf) - 1];
			for(int i = 0; i < 5; i++)
			{
				dataTransferPort[i] = buf[strlen(buf) - 6 + i];
			}
			if(dataTransferPort[4] == '*')
			{
				dataTransferPort[4] = '\0';
			}
			else
			{
				dataTransferPort[5] = '\0';	
			}
			
			//If file is requested...
			if(command == 'g' || command == 'G')
			{
				//Parse file name requested from client command string, store in fileName
				char *fileName = NULL;
				if(strlen(buf) > 6)
				{
					fileName = malloc(sizeof(int) * (strlen(buf) - 6));
					for(int i = 0; i < strlen(buf) - 6; i++)
					{
						fileName[i] = buf[i];
					}
					fileName[strlen(buf) - 6] = '\0';
				}
				
				printf("File \"%s\" requested on port %s\n", fileName, dataTransferPort);
				
				//Attempt to open requested file
				FILE *fp = fopen(fileName, "rb");
				//If successful (i.e. file exists and no errors occur)...
				if(fp)
				{
					//Send acknowledgment to client on control connection 
					//--(acknowledging request for directory contents)
					strcpy(responseCode, "CA");
					send(cc.handle, responseCode, strlen(responseCode), 0);
					
					//Initiate data connection for file transfer on client-specified data port
					int sock_dc_fd[2];
					initiate(dataTransferPort, &cc, sock_dc_fd, 1);
					
					if(sock_dc_fd[1] == 0)
					{
						//Send port available (PA) message to client on control connection
						strcpy(responseCode, "PA");
						send(cc.handle, responseCode, strlen(responseCode), 0);
					
						printf("Sending \"%s\" to %s:%s\n", fileName, cc.hostName, dataTransferPort);
						
						//Determine size of requested file (see fsize.c for additional details)
						//--Convert the file size to a string representation in fileSizeString
						int fileSize = fsize(fileName);
						int fileLengthStringSize = snprintf(NULL, 0, "%d", fileSize);
						char* fileSizeString = malloc(fileLengthStringSize + 1);
						snprintf(fileSizeString, fileLengthStringSize + 1, "%d", fileSize);
						
						//Send file size to client on data connection, in preparation for transfer
						int bytes_sent = send(sock_dc_fd[0], fileSizeString, strlen(fileSizeString), 0);
						
						//Receive acknowledgment from client
						char confirmationBuffer[18];
						recv(sock_dc_fd[0], confirmationBuffer, 18, 0);
						
						//Send file in blocks of 512 bytes until file transfer is complete
						char fileChunk[512] = "i";
						int i = 0, j = 0;
						while(!feof(fp))
						{
							for(i = 1; i <= 512 && !feof(fp); i++)
							{
								fread(&fileChunk[i - 1], 1, 1, fp);
							}
							if(i > 0)
							{
								int bytes_sent = send(sock_dc_fd[0], fileChunk, --i, 0);
								recv(sock_dc_fd[0], confirmationBuffer, 14, 0);
							}
							//j is set back to one so that when the outer while loop
							//--exits, the following if statement can make sure data
							//--was sent before waiting for a final response from the
							//--client (i.e. bypasses this final confirmation expectation
							//--if the file selected was blank to begin with.)
							j = 1;
						}
						//If data was transferred (i.e. file was not blank to begin with),
						//--receive final confirmation from client
						if(j > 0)
						{
							recv(sock_dc_fd[0], confirmationBuffer, 13, 0);
						}
						//Close data connection and free memory
						close(sock_dc_fd[0]);
						free(fileName);
						free(fileSizeString);
					}
					//A non-zero value in sock_dc_fd[1] indicates a failure to bind
					//--to the specified data port
					else
					{
						//Send port unavailable command (PU)
						strcpy(responseCode, "PU");
						send(cc.handle, responseCode, strlen(responseCode), 0);
					}
				}
				//If requested file is not found, send error message 
				//--to client (on control connection) accordingly.
				else
				{	
					printf("File \"%s\" not found.\nSending error code NF to %s on port %s\n", 
							fileName, cc.hostName, serverPort);
					strcpy(responseCode, "NF");
					int bytes_sent = send(cc.handle, responseCode, strlen(responseCode), 0);
					if(fileName != NULL)
					{
						free(fileName);
					}
				}
			}
			else
			{
				//Send acknowledgment to client on control connection 
				//--(acknowledging request for directory contents)
				strcpy(responseCode, "CA");
				send(cc.handle, responseCode, strlen(responseCode), 0); 
				
				//Initiate data connection for file transfer on client-specified data port
				int sock_dc_fd[2];
				initiate(dataTransferPort, &cc, sock_dc_fd, 1);
				
				if(sock_dc_fd[1] == 0)
				{
					//Send port available (PA) message to client on control connection
					strcpy(responseCode, "PA");
					send(cc.handle, responseCode, strlen(responseCode), 0);
				
					printf("List directory requested on port %s\n", dataTransferPort);
					
					//Get directory contents and send to client on data connection
					char directory[8192] = "\n";
					getCurrentDirectory(directory);
					printf("Sending directory contents to %s:%s\n", 
							cc.hostName, dataTransferPort);
					int bytes_sent = send(sock_dc_fd[0], directory, strlen(directory), 0);
					
					//Close data connection
					close(sock_dc_fd[0]);
				}
				//A non-zero value in sock_dc_fd[1] indicates a failure to bind
				//--to the specified data port
				else
				{
					//Send port unavailable command (PU)
					strcpy(responseCode, "PU");
					send(cc.handle, responseCode, strlen(responseCode), 0);
				}
					
			}
		}
		
		printf("Connection Ended.\n"); 
		printf("\nWaiting for connection on port %s...\n", serverPort);
		
		//Close the control connection
		close(cc.handle);
		//Listen again on control connection port
		cc = acceptConnection(cc.sockfd, 1);
		
    }while(1);
	
    return 0;
    
}

/****************************************************************************************************************************
   References 
** 1.  https://beej.us/guide/bgnet/html/multi/syscalls.html
** 2.  https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleserver
** 3.  "C Programming Language". Kernighan, Brian and Dennis Ritchie. 2nd Edition. Pearson Education, Inc. 1988. 
** 4.  https://www.tutorialspoint.com/cprogramming/c_input_output.htm 
** 5.  https://stackoverflow.com/questions/16010622/reasoning-behind-c-sockets-sockaddr-and-sockaddr-storage/34236231
** 6.  https://www.geeksforgeeks.org/fork-system-call/
** 7.  https://www.geeksforgeeks.org/explicitly-assigning-port-number-client-socket/
** 8.  https://stackoverflow.com/questions/11583562/how-to-kill-a-process-running-on-particular-port-in-linux
** 9.  https://stackoverflow.com/questions/18609397/whats-the-difference-between-sockaddr-sockaddr-in-and-sockaddr-in6
** 10. https://stackoverflow.com/questions/8004237/how-do-i-properly-compare-strings
** 11. https://stackoverflow.com/questions/308695/how-do-i-concatenate-const-literal-strings-in-c
** 12. http://www.cplusplus.com/reference/cstdio/scanf/
** 13. https://stackoverflow.com/questions/2700646/dereferencing-pointer-to-incomplete-type
** 14. https://stackoverflow.com/questions/12024703/why-cant-getaddrinfo-be-found-when-compiling-with-gcc-and-std-c99
** 15. https://stackoverflow.com/questions/17665793/directly-assigning-values-to-c-pointers
** 16. https://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c
** 17. https://stackoverflow.com/questions/797318/how-to-split-a-string-literal-across-multiple-lines-in-c-objective-c
** 18. https://stackoverflow.com/questions/3220277/what-do-the-makefile-symbols-and-mean
** 19. https://stackoverflow.com/questions/41407318/warning-passing-argument-2-of-fopen-makes-pointer-from-integer-without-a-cast
** 20. https://stackoverflow.com/questions/190229/where-is-the-itoa-function-in-linux
** 21. http://www.cplusplus.com/reference/cstdio/snprintf/
** 22. https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
** 23. http://www.cplusplus.com/reference/cstdio/fread/
** 24. https://stackoverflow.com/questions/6093224/reading-a-binary-file-1-byte-at-a-time
*******************************************************************************************************************************/