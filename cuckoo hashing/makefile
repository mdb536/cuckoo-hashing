
default: cuckoo_hashing

cuckoo_hashing: main.o cuckoo_hashing.o
	g++ -g main.o cuckoo_hashing.o -o cuckoo_hashing

main.o: main.cpp cuckoo_hashing.h
	g++ -g -c main.cpp

cuckoo_hashing.o: cuckoo_hashing.cpp cuckoo_hashing.h
	g++ -g -c cuckoo_hashing.cpp