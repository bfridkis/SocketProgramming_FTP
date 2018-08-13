README.TXT - CS372 PROGRAMMING ASSIGNMENT 2 (PROJECT 2)
AUTHOR: BENJAMIN FRIDKIS

STEPS TO EXECUTE FTSERVER AND FTCLIENT - A FTP PROGRAM TO DEMONSTRATE SOCKET PROGRAMMING

1.  Copy the zip file "Project2_CS372_BFRIDKIS.zip" to a directory of your choice on any of the 
	  following machines:
		- flip1.engr.oregonstate.edu
		- flip2.engr.oregonstate.edu
		- flip3.engr.oregonstate.edu
		
	Unzip the contents.
	
	OPTIONAL: Move "ftclient.py","inputParameters.py", and ftclientMT.py to a 
			  different directory.
			  
			  If you do not do this, you will be prompted to overwrite every file
			  you attempt to transfer, as you will be reading the same file that you
			  will be saving, all from within the same directory. (You will in each
			  case be asked if you want to overwrite or append a new version to the
			  filename when saving. Note this step is only optional because
			  a temp file name is generated for writing in all cases, and only if
			  overwrite is selected will the original be deleted and the temp then
			  renamed with the original title. This also occurs, of course, if the
			  server and client are executed in different directories and the client
			  requests a file from the server with the same name as a file currently
			  existing in the client-side directory.)
	
2.  Type the command: "make all" (without quotes)
		- Note this generates all files for both the base program and extra credit program.
		  The base program alone can be compiled by simply typing "make" (without quotes)

3. 	Launch the c-based server (ftserver), specifying a port number between 3000 and 65535
	  on the command line with program launch.
		~EX. To launch using port 35000, type: "./ftserver 35000" (without quotes)

	OPTIONAL: Omit the server-port specification on the command-line. A prompt will be
			  issued by the program for server port entry in this case.

4.  Launch the python-based client (ftclient.py), specifying the following criteria on the
	  command line IN THE ORDER LISTED:
		
		i. HOST SERVER NAME
		
		ii. HOST SERVER PORT NUMBER
		
		iii. COMMAND (-l to list server directory, -g for get file)
			(These commands are case-insensitive, so -L and -G can be used alternatively.)
		iv.
		  a. If entry for #3 is -l (or -L) : PORT NUMBER FOR DATA CONNECTION
		  b. If entry for #4 is -g (or -G) : NAME OF REQUESTED FILE
				-If a file is requested with the same name as a file residing in the current
				 directory (which will always be the case if the client and server are running
				 from the same directly, as described above), a prompt will be issued for
				 an "overwrite" or "rename" (with new version number). An entry of -o is used
				 to overwrite the existing file, while any other entry (including a blank one)
				 will result in a renaming of the transferred file. (See OPTIONAL note in step 1
				 above for additional details.)
		
		v. PORT NUMBER FOR DATA CONNECTION - ONLY USE IF COMMAND -g or -G IS USED IN STEP 3
		
	Note: Because the assignment specifies the need to process an invalid command server-side,
		  the client will accept invalid commands of the proper format (which is a single '-'
		  followed by a single character) via the command prompt. If an invalid command
		  (of the proper format) is received, the program assumes the fourth command-line 
		  argument is PORT NUMBER FOR DATA CONNECTION.
		  
		  Note additionally that a command (for command-line arg #3) of an invalid format
		  type (i.e. anything besides a single '-' followed by a single character) will NOT  
		  be accepted, and if an argument with an invalid format for command is received 
		  (via command-line), the user will be prompted to re-enter the command according to 
		  the correct format. If -g or -G is specified after this command re-entry prompt 
		  is issued, the program considers command-line argument 4 NAME OF REQUESTED FILE
		  (and argument 5 PORT NUMBER FOR DATA CONNECTION, if provided). Otherwise, argument 
		  4 is considered PORT NUMBER FOR DATA CONNECTION.
		  
	OPTIONAL: Command-line arguments are optional. Prompts will be issued for any omitted items.
			  However, if any command-line arguments are omitted, ALL command-line arguments that
			  follow (according to the order specified above) must be omitted as well!
			  
			  If the "COMMAND" command-line argument is omitted, invalid formats for this parameter
			  will not be accepted by the program prompt (i.e. data validation for this parameter is 
			  enforced exclusively by the client if COMMAND is not provided in the command-line).
			  
	~EX 1. To view the working directory of ftserver on flip3.engr.oregonstate.edu:35000
		     using data transfer port 35001
			   type: "python3 ftclient.py flip3.engr.oregonstate.edu 35000 -l 35001" (without quotes)

	~EX 2. To request the file "Alice.txt" from ftserver running on flip3.engr.oregonstate.edu:35000, 
			 using data transfer port 35001
			   type: "python3 ftclient.py flip3.engr.oregonstate.edu 35000 -g Alice.txt 35001" (without quotes)				

			Note: Filenames are case-sensitive!
	
	
     ** ALL RESULTING SERVER AND CLIENT ACTIVITY SHOULD BE INDICATED BY CONSOLE OUTPUTS RESPECTIVELY **

	FINAL NOTE: An attempt to re-use the same data transfer port for multiple back-to-back 
				operations in quick succession may result in a "PU" (Port Unavailable) 
				error from the server. (This timeout period is typically only a few seconds.)
	 
!!!!! EXTRA CREDIT !!!!! - MULTI-THREADED SERVER

5.  If only the base program was compiled in step 2, type: "make ftserverMT" (without quotes)
	  Otherwise, skip this step.
	  
6.  Launch the c-based multi-threaded server (ftserverMT), specifying a port number between 
	  3000 and 65535 on the command line with program launch.
		~EX. To launch using port 35000, type: "./ftserverMT 35000" (without quotes)
	  
7.  Repeat step 4, using "ftclientMT.py" in place of "ftclient.py".

	~EX 1. To view the working directory of ftserverMT on flip3.engr.oregonstate.edu:35000
		     using data transfer port 35001
			   type: "python3 ftclientMT.py flip3.engr.oregonstate.edu 35000 -l 35001" (without quotes)

	~EX 2. To request the file "Alice.txt" from ftserverMT running on flip3.engr.oregonstate.edu:35000, 
			 using data transfer port 35001
			   type: "python3 ftclientMT.py flip3.engr.oregonstate.edu 35000 -g Alice.txt 35001" (without quotes)

	   **  Each requested data connection spawns a child process, so multiple files can  **
	          be transferred simultaneously using the same server (control) port!        **
	
       **  ftserverMT will only log data transfer messages. Unlike ftserver, it will NOT **
	   **	            log data port connection specifics to the terminal.		         **
	   
	   **    As opposed to the ftserver/ftclient.py protocol, in which ftclient.py       **
	   **  initiates the control connection and ftserver initiates the data connection,  **
	   **   the ftserverMT/ftclientMT.py protocol specifies that ftclient.py initiates   **
	   **                both the control and data connections.                          **	
	   
	   **       The single-threaded and multi-threaded versions of these programs        **
	   **    are not interchangeable! You must use ftclient.py to connect to ftserver,   **
	   **          and you must use ftclientMT.py to connect ftserverMT!                 **
	
	
**** Programs tested using various combinations of flip servers. 
     Primarily used flip2.engr.oregonstate.edu for ftclient.py,
     connected to ftserver running on flip3.engr.oregonstate.edu.

**** Used "Alice.txt", "short.txt", and "long.txt" for testing file
	 file transfers. These have been included with the assignment 
	 submission. (Files with bigger sizes were also tested but have 
	 not been included with the submission.)