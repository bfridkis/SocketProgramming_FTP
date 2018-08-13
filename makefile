CFLAGS = -g -Wall -Wfatal-errors -Wno-unused-variable -D_FILE_OFFSET_BITS=64 -std=gnu99
#CFLAGS = -g -Wall -Wno-unused-variable -std=gnu99
#CFLAGS = -g -w -std=gnu99
VPATH = cutest

ftserver : ftserver.o get_in_addr.o bufferClear.o getServerPort.o listener.o acceptConnection.o getCurrentDirectory.o fsize.o initiate.o
	gcc $(CFLAGS) -o $@ $^
	
ftserverMT : ftserverMT.o get_in_addr.o bufferClear.o getServerPort.o listenerMT.o acceptConnectionMT.o getCurrentDirectory.o fsize.o initiate.o sigchld_handler.o
	gcc $(CFLAGS) -o $@ $^	

ftserver.o : ftserver.c
	gcc $(CFLAGS) -c -o $@ $<	

get_in_addr.o : get_in_addr.c
	gcc $(CFLAGS) -c -o $@ $<
	
bufferClear.o : bufferClear.c
	gcc $(CFLAGS) -c -o $@ $<
	
getServerPort.o : getServerPort.c
	gcc $(CFLAGS) -c -o $@ $<
	
listener.o : listener.c
	gcc $(CFLAGS) -c -o $@ $<

acceptConnection.o : acceptConnection.c
	gcc $(CFLAGS) -c -o $@ $<
	
initate.o : initiate.c
	gcc $(CFLAGS) -c -o $@ $<
	
getCurrentDirectory.o : getCurrentDirectory.c
	gcc $(CFLAGS) -c -o $@ $<
	
fsize.o : fsize.c
	gcc $(CFLAGS) -c -o $@ $<
	
ftserverEC.o : ftserver.c
	gcc $(CFLAGS) -c -o $@ $<

sigchld_handler.o : sigchld_handler.c
	gcc $(CFLAGS) -c -o $@ $<	
	
memcheckProg : chatclient
	valgrind --tool=memcheck --leak-check=yes chatclient

all : ftserver ftserverMT
	
run :
	./ftserver
	
clean :
	-rm *.o
	-rm ftserver
	-rm ftserverMT
	
#References
# 1. https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
