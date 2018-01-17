------------------------------------------------------------
INCLUDED
------------------------------------------------------------
cuckoo_hashing.cpp           all the functions used to the hashing scheme
cuckoo_hashing.h             header file for these functions
main.cpp	                   main function 	   
README		                   This file
Makefile                     Makefile to build program


--------------------------------------------
Description
--------------------------------------------
This is an implementation of Cuckoo Hashing in C++
Th program accepts a command line argument giving:
	n, the size of one of the tables (total number of slots will be 2*n) .
	file, the name of the text file that contains the list of instructs to perform with the hashing scheme

the input files are written in the following formate:
  [command] [int to use for the command]
  ie. i 23, performs an insert of 23
      d 20, performs a delete of 20
      s 45, performs a search of 45

And uses that information to simulate cuckoo hashing, 
it then either terminates with the the finished table or
terminates due to 

----------------------------------------
Command to compile
----------------------------------------
make 

------------------------------------------------------------
Command to run and test program
------------------------------------------------------------
To test the program with the following parameters :
	n = 11
  file = test.txt

You would run:
	./cuckoo_hashing 11 test.txt

Expected output:
  Terminates due to cycle detection
