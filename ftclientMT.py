##**************************************************************************
## Program name: ftclientMT.py
## Class name: CS372
## Author: Ben Fridkis
## Date: 8/7/2018
## Description: Client-side FTP program to demonstrate socket programming.
##				Allows user to request the contents of a remote directory
##				or request a file for transfer.
##**************************************************************************

from time import sleep
from math import floor
from socket import *
from inputParameters import getInputParameters
from os import listdir, getcwd, remove, rename
from os.path import isfile, join

#Function to populate a dict with necessary connection specifications
#--See inputParameters.py for additional details
inputParameters = getInputParameters()

#This line was taken from reference 13 (see below)
#--getcwd stores the working directory in cwd
cwd = getcwd()

#The following line is taken from reference 14 (see below)
#This populates the directoryFiles list with current directory file names
directoryFiles = [f for f in listdir(cwd) if isfile(join(cwd, f))]

#If the user requests a file with the same name as that in the current directory,
#prompt user to either overwrite existing file or append a version # to requested file
#once transferred
try:
	if inputParameters["fileName"] in directoryFiles:
		print('\n"' + inputParameters["fileName"] + '"' + " already exists in this directory.\n")
		print("-- Enter -o to overwrite, or press Enter to append")
		print("-- a unique version number to file name when saving.")
		overWrite = input();
except Exception:
	pass

#If port number specified is 4 digits, pad with one '*' for processing in server	
if(inputParameters["dataPort"] <= 9999):
	dataPortMessage = str(inputParameters["dataPort"]) + "*"
else:
	dataPortMessage = str(inputParameters["dataPort"])

#Establish connection socket (control connection)	
clientConnectionSocket = socket(AF_INET, SOCK_STREAM)

#Attempt connection to specified server on specified port
clientConnectionSocket.connect((inputParameters["serverName"], inputParameters["serverPort"]))

#Send command string to server
clientConnectionSocket.send((inputParameters["fileName"] + dataPortMessage + 
	inputParameters["command"][1]).encode())
	
#Receive acknowledgement from server on control connection
connectionResponse = clientConnectionSocket.recv(2)
#print(connectionResponse.decode("utf-8", "ignore") + "\n")

#If command was accepted, establish a data connection socket with server on specified data port number
if connectionResponse.decode("utf-8", "ignore") == ("CA"):
	clientResponseSocket = socket(AF_INET, SOCK_STREAM)
	clientResponseSocket.connect((inputParameters["serverName"], inputParameters["dataPort"]))
	#if -l command entered, send directory structure
	if inputParameters["command"] == "-l" or inputParameters["command"] == "-L":
		dataTransferResponse_Directory = clientResponseSocket.recv(8192)
		print("\nReceiving directory structure from " + inputParameters["serverName"] + ":" 
				+ str(inputParameters["dataPort"]) + "\n", 
				dataTransferResponse_Directory.decode("utf-8", "ignore"))
	else:
		#check if file with same name as requested file exists in the current directory
		#--If so, append a version number to the file before opening. 
		#--(Note that even if the user indicats an overwrite for the existing file,
		#-- a file with a unique name is created initally. This is to maintain functionality
		#-- when this client program is running in the same directory as its corresponding server.
		#-- In this case, once the transfer is complete, the pre-existing file
		#-- (which was read from by the server) will be deleted and the newly created
		#-- file will be renamed back to the original specification.
		try:
			overWrite
			dotIndex = inputParameters["fileName"].rfind(".")
			i = 1
			newOrTempFN = (inputParameters["fileName"][:dotIndex] +
							"_" + str(i) + inputParameters["fileName"][dotIndex:])
			while newOrTempFN in directoryFiles:
				i += 1
				newOrTempFN = (inputParameters["fileName"][:dotIndex] +
								"_" + str(i) + inputParameters["fileName"][dotIndex:])
			f = open(newOrTempFN, "wb")
		except:
			f = open(inputParameters["fileName"], "wb")
		
		#Receive incoming file size message from server
		incomingFileSize = clientResponseSocket.recv(32).decode("utf-8", "ignore")
		#Determine the number of 512-byte-sized blocks to be read based on file size
		numFullChunks = floor(int(incomingFileSize)/512)
		remainder = int(incomingFileSize) - (numFullChunks * 512)
		#Send acknowledgment for file size message to server on data connection
		clientResponseSocket.send("File Size Received".encode())
		
		print('\nReceiving "' + inputParameters["fileName"] + '" from ' +
				inputParameters["serverName"] + ":" + str(inputParameters["dataPort"]) + "\n")
		
		#Receive data blocks, write to file, and acknowledge each block
		while numFullChunks > 0:
			dataTransferResponse_FileContents = clientResponseSocket.recv(512)
			f.write(dataTransferResponse_FileContents)
			clientResponseSocket.send(("Chunk Received").encode())
			numFullChunks -= 1	
		dataTransferResponse_FileContents = clientResponseSocket.recv(int(remainder))
		f.write(dataTransferResponse_FileContents)
		clientResponseSocket.send(("Chunk Received").encode())
		#Send final acknowledgment to server
		clientResponseSocket.send(("File Received").encode())
		print("File Transfer Complete.\n")
		f.close()
		try:
			overWrite
			if overWrite == "-o":
				remove(inputParameters["fileName"])
				rename(cwd + "/" + newOrTempFN, cwd + "/" + inputParameters["fileName"])
		except:
			pass
	
	#Close data connection
	clientResponseSocket.close()

#Print error message if server responds with file NF (Not Found) command
elif connectionResponse.decode("utf-8", "ignore") == ("NF"):
	print("\n" + inputParameters["serverName"] + ":" + 
			str(inputParameters["serverPort"]) + " says FILE NOT FOUND (" +
			connectionResponse.decode("utf-8", "ignore") + ")\n")

#Server will send an "IV" command for invalid command. 
#Error message is printed according.			
else:
	print("\nServer Error: Invalid Command (" + connectionResponse.decode("utf-8", "ignore") +
		  ")\nEnter '-g <FILENAME>' to get filename, or '-l' to list directory."
		  "\n(Note: Substitute <FILENAME> for the name of the file desired,"
		  "\n--omitting the '<' & '>' symbols.)\n")

#Close control connection
clientConnectionSocket.close()


##REFERENCES##
#1.  "Computer Networking: A Top Down Approach". Kurose, James and Kieth Ross. 7th Edition. Pearson. 2017.  	
#2.  https://stackoverflow.com/questions/4187185/how-can-i-check-if-my-python-object-is-a-number
#3.  https://stackoverflow.com/questions/17761697/python-sys-is-not-defined
#4.  https://www.cyberciti.biz/faq/how-do-i-find-out-what-ports-are-listeningopen-on-my-linuxfreebsd-server/
#5.  https://unix.stackexchange.com/questions/73767/how-to-check-whether-firewall-opened-for-a-port-but-not-listening-on-the-port
#6.  https://superuser.com/questions/602049/how-to-use-netstat-on-a-specific-port-in-linux
#7.  https://stackoverflow.com/questions/986006/how-do-i-pass-a-variable-by-reference
#8.  https://www.cyberciti.biz/faq/unix-linux-check-if-port-is-in-use-command/
#9.  https://stackoverflow.com/questions/730764/how-to-properly-ignore-exceptions
#10. https://stackoverflow.com/questions/42339876/error-unicodedecodeerror-utf-8-codec-cant-decode-byte-0xff-in-position-0-in
#11. https://stackoverflow.com/questions/491921/unicode-utf-8-reading-and-writing-to-files-in-python
#12. https://www.pythoncentral.io/pythons-time-sleep-pause-wait-sleep-stop-your-code/
#13. https://stackoverflow.com/questions/5137497/find-current-directory-and-files-directory
#14  https://stackoverflow.com/questions/3207219/how-do-i-list-all-files-of-a-directory?page=1&tab=votes#tab-top
#15. https://stackoverflow.com/questions/4172448/is-it-possible-to-break-a-long-line-to-multiple-lines-in-python/4172465
#16. https://stackoverflow.com/questions/6996603/delete-a-file-or-folder
#17. https://www.guru99.com/python-rename-file.html
#18. https://stackoverflow.com/questions/431684/how-do-i-change-directory-cd-in-python/13197763#13197763