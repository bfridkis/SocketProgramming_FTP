/************************************************************************************
** Program name: sigchld_handler.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 8/3/2018
** Description: Wrapper function to save the value of errno prior to waitpid call,
				so error value can be restored to errno after call in the event
				it is overwritten by waitpid. waitpid itself is used to reap all
				'dead' child processes. The sigaction function call (in ftserverEC.c) 
				registers this handler for the SIGCHLD signal, such that child
				processes which have 'terminated' but not yet cleared (i.e. "zombies")
				will be completely removed (from the process table). See reference #3
				for complete details.
				(Code directly from first reference listed below.)
*************************************************************************************/

#include "sigchld_handler.h"

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

/**************************************************************************************
   References 
** 1. https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleserver
** 2. https://support.sas.com/documentation/onlinedoc/sasc/doc/lr2/waitpid.htm
** 3. http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
** 4. http://man7.org/linux/man-pages/man2/sigaction.2.html
***************************************************************************************/