a) 	
	make sure to compile this in linux environment
		g++ -std=c++11 myNode.cpp -o myNode.out
		./myNode.out input_file1.txt access_trace1.txt
	it will create exe file to run the exe file use ./myNode inputfile1 accesstrace1
b)	Maximum Number of blocks:-
		The maximum number of blocks that can be supported using myNode is 11 blocks for single direct pointer, 100 blocks for single
		indirect block and 10000 blocks for double indirect block, so total block number is 11+100+10000 = 10111 blocks
	Naming Convention:-
		I stored the names in array for direct blocks while I used some mathematical computations to name the indirect blocks
		The naming convention i used to named the files is zero.txt for block 0 one.txt for block 1 two.txt for block 2 and so on..
`		for single indirect blocks I used naming conventions as twelve_0.txt for block 12 twelve_1.txt for block 13 and so on.
		for double indirect blokcs I used naming conventions as thirteen_0_0.txt for block 112, thirteen_0_1.txt for block 113 and so on