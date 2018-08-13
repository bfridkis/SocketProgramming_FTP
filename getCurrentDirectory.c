/********************************************************************************************
** Program name: getCurrentDirectory.c
** Class name: CS372
** Author: Ben Fridkis
** Date: 7/19/2018
** Description: Program to print current directory and sub-directories.
**				CODE TAKEN FROM (with only minor modifcations made thereto): 
				https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/
*********************************************************************************************/	

#include "getCurrentDirectory.h"

void getCurrentDirectory(char* directory)
{	
	struct dirent *de;  		//Pointer for directory entry
	struct stat stat_info;		//Structure to hold stat info about directory items

	// opendir() returns a pointer of DIR type. 
	DIR* dr;
	dr = opendir(".");

	if (dr == NULL)  //opendir returns NULL if couldn't open directory
	{
		printf("Could not open current directory" );
	}

	// Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
	// for readdir()
	while ((de = readdir(dr)) != NULL)
	{
		//printf("\n*%s*", de->d_name);
		if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
		{
			stat(de->d_name, &stat_info);
			strcat(directory, de->d_name);
			if(S_ISDIR(stat_info.st_mode))
			{
				strcat(directory, " (DIR) ");
			}
			strcat(directory, "\n");
		}
	}
	directory[strlen(directory)] = '\0';
	
	closedir(dr);    
}

/********************************************************************************
   Additional Reference
** https://stackoverflow.com/questions/7478492/problem-reading-directories-in-c  
*********************************************************************************/