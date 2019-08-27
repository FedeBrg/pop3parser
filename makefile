all: utils messi tcputils addr die getaddr server

mayus: mayus.c
	gcc -o mayus mayus.c

utils: Utilities.c
	gcc -o Utilities.o -c Utilities.c

messi: messi.c Utilities.o
	gcc -o messi.o -c messi.c



tcputils: TCPServerUtility.c
	gcc -o TCPServerUtility.o -c TCPServerUtility.c



addr: AddressUtility.c
	gcc -o AddressUtility.o -c AddressUtility.c



die: DieWithMessage.c
	gcc -o DieWithMessage.o -c DieWithMessage.c



getaddr: GetAddrInfo.c
	gcc -o GetAddrInfo.o -c GetAddrInfo.c



server: server.c Utilities.o messi.o TCPServerUtility.o AddressUtility.o DieWithMessage.o GetAddrInfo.o
	gcc -o server server.c Utilities.o messi.o DieWithMessage.o TCPServerUtility.o AddressUtility.o


.PHONY: clean
clean:
	rm -rf *.o server mayus