##**********************************************************************************
## Program name: clientInputParameters.py
## Class name: CS372
## Author: Ben Fridkis
## Date: 8/5/2018
## Description: Function to ascertain all input parameters from user for ftp client.
##				User will be prompted for parameters not specified on the command 
##				line at program launch. (See README.txt for additional details.)
##				Returns a dictonary with paramter values. 
##***********************************************************************************

#Essentially this is just a heirarchy of exception handlers to allow a user flexibility
#--when entering communications parameters. 
#--Required parameters:
#	 *Host Name
#	 *Host Connection Port
#	 *Command -l (or -L), and -g (or -G)
#	 	-FILENAME (Required if -g or -G command entered)
#	 *Host Data Port
#
#The above listed parameters can be entered on the command line, or if unspecified (on
#--command line at program launch) will be prompted for at program outset. They must,
#--however, be entered in the order listed above. If any are omitted from the command line,
#--all that follow must be omitted as well.

import sys

def getInputParameters():
	try:
		serverName = sys.argv[1]
	except:
		serverName = input("\nEnter server name\n-->")
	try:
		try:
			serverPort = int(sys.argv[2])
		except ValueError:
			serverPort = 0
			print("\n\nServer Port Value Invaid.")
		if not (serverPort >= 3000 and serverPort <= 65535):
			print("\nCommand-line Server Port Number Specification Invalid.")
		while not (serverPort >= 3000 and serverPort <= 65535):
			print("Server Port Number must be an integer in range of 3000 to 65,535.\n")
			try:
				serverPort = int(input("Enter Server Port Number\n--> "))
			except ValueError:
				print("\n\nServer Port Value Invaid.")
	except Exception:			
		try:
			serverPort = int(input("\nEnter Server Port Number\n--> "))
		except ValueError:
			serverPort = 0
			print("\n\nServer Port Value Invaid.")			
		while not (serverPort >= 3000 and serverPort <= 65535):
			print("Server Port Number must be an integer in range of 3000 to 65,535.\n")
			try:
				serverPort = int(input("Enter Server Port Number\n--> "))
			except ValueError:
				print("\n\nServer Port Value Invaid.")
		command = "-i"
		while command != "-g" and command != "-l" and command != "-G" and command != "-L":
			command = input("\nEnter command ('-l' for list directory or '-g' to get file): ")
		if command == "-g" or command == "-G":
			fileName = input("\nEnter file name\n-->")
		try:
			dataPort = int(input("\nEnter Data Port Number\n-->"))
		except ValueError:
			dataPort = 0
			print("\nData Port Value Invaid.")
		while not (dataPort >= 3000 and dataPort <= 65535):
			print("Data Port Number must be an integer in range of 3000 to 65,535.\n")
			try:
				dataPort = int(input("Enter Data Port Number\n--> "))
			except ValueError:
				print("\nData Port Value Invaid.")
	try:
		command
	except:
		try:
			command = sys.argv[3]
			while(command[0] != "-" or len(command) != 2):
				print("\nCommand must be one character preceded by a '-'\n")
				command = input("Enter command: ")
		except Exception:
			command = "-i"
			while command != "-g" and command != "-l" and command != "-G" and command != "-L":
				command = input("\nEnter command ('-l' for list directory or '-g' to get file): ")
			if command == "-g" or command == "-G":
				fileName = input("\nEnter file name\n-->")
			try:
				dataPort = int(input("\nEnter Data Port Number\n-->"))
			except ValueError:
				dataPort = 0
				print("\nData Port Value Invaid.")
			while not (dataPort >= 3000 and dataPort <= 65535):
				print("Data Port Number must be an integer in range of 3000 to 65,535.\n")
				try:
					dataPort = int(input("Enter Data Port Number\n--> "))
				except ValueError:
					print("\nData Port Value Invaid.")
	if command == "-g" or command == "-G":
		try:
			fileName
		except:
			try:
				fileName = sys.argv[4]
			except:
				fileName = input("\nEnter file name\n-->")
				try:
					dataPort = int(input("\nEnter Data Port Number\n-->"))
				except ValueError:
					dataPort = 0
					print("\nData Port Value Invaid.")
				while not (dataPort >= 3000 and dataPort <= 65535):
					print("Data Port Number must be an integer in range of 3000 to 65,535.\n")
					try:
						dataPort = int(input("Enter Data Port Number\n--> "))
					except ValueError:
						print("Data Port Value Invaid.")
	else:
		fileName = ""
	try:
		dataPort
	except:
		if command != "-g" and command != "-G":
			try:
				try:
					dataPort = int(sys.argv[4])
				except:
					dataPort = 0
					print("\nData Port Value Invaid.")
				if not (dataPort >= 3000 and dataPort <= 65535):
					print("\nCommand-line Data Port Number Specification Invalid.")
					while not (dataPort >= 3000 and dataPort <= 65535):
						print("Data Port Number must be an integer in range of 3000 to 65,535.\n")
						try:
							dataPort = int(input("Enter Data Port Number\n--> "))
						except ValueError:
							print("Data Port Value Invaid.")
			except Exception:
				try:
					dataPort = int(input("\nEnter Data Port Number\n-->"))
				except ValueError:
					dataPort = 0
					print("\nData Port Value Invaid.")
				while not (dataPort >= 3000 and dataPort <= 65535):
					print("Data Port Number must be an integer in range of 3000 to 65,535.\n")
					try:
						dataPort = int(input("Enter Data Port Number\n--> "))
					except ValueError:
						print("Data Port Value Invaid.")
		else:
			try:
				try:
					dataPort = int(sys.argv[5])
				except ValueError:
					dataPort = 0
					print("\nData Port Value Invaid.")
				if not (dataPort >= 3000 and dataPort <= 65535):
					print("\nCommand-line Data Port Number Specification Invalid.")
					while not (dataPort >= 3000 and dataPort <= 65535):
						print("Data Port Number must be an integer in range of 3000 to 65,535.\n")
						try:
							dataPort = int(input("Enter Data Port Number\n--> "))
						except ValueError:
							print("Data Port Value Invaid.")
			except Exception:
				try:
					dataPort = int(input("\nEnter Data Port Number\n-->"))
				except ValueError:
					dataPort = 0
					print("\nData Port Value Invaid.")
				while not (dataPort >= 3000 and dataPort <= 65535):
					print("Data Port Number must be an integer in range of 3000 to 65,535.\n")
					try:
						dataPort = int(input("Enter Data Port Number\n--> "))
					except ValueError:
						print("Data Port Value Invaid.")
	return {
		"serverName":serverName, 
		"serverPort":serverPort, 
		"command":command,
		"fileName":fileName,
		"dataPort":dataPort
		}