all: hencode hdecode

hencode: hencode.o list.o
	g++ -o hencode hencode.o list.o

hdecode: hdecode.o list.o
	g++ -o hdecode hdecode.o list.o

hencode.o: hencode.cc priqueue.h priqueue.cc list.h
	g++ -c hencode.cc

hdecode.o: hdecode.cc priqueue.h priqueue.cc list.h
	g++ -c hdecode.cc

list.o: list.cc list.h
	g++ -c list.cc