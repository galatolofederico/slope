CC=g++
CFLAGS=-g -I. -std=c++0x -O2

all: vm

vm: coreobjects.o operations.o slist.o util.o vm.o gc.o
	$(CC) coreobjects.o operations.o slist.o util.o vm.o gc.o -o vm $(CFLAGS)

gc.o: gc.cpp
	$(CC) -c gc.cpp -o gc.o $(CFLAGS)

coreobjects.o: coreobjects.cpp
	$(CC) -c coreobjects.cpp -o coreobjects.o $(CFLAGS)

operations.o: operations.cpp
	$(CC) -c operations.cpp -o operations.o $(CFLAGS)

util.o: util.cpp
	$(CC) -c util.cpp -o util.o $(CFLAGS)

vm.o: vm.cpp
	$(CC) -c vm.cpp -o vm.o $(CFLAGS)

slist.o: slist.cpp
	$(CC) -c slist.cpp -o slist.o $(CFLAGS)

clean:
	rm *.o vm

install:
	rm -rf /etc/slope
	mkdir /etc/slope
	cp RPN.py /etc/slope
	cp vm /usr/local/bin/slope-vm
	cp compiler.py /usr/local/bin/slope-compiler
	chmod +x /usr/local/bin/slope-compiler
	cp slope /usr/local/bin/slope
	chmod +x /usr/local/bin/slope
