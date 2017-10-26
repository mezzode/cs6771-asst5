all: sortTester

sortTester : sortTester.o BucketSort.o
	g++ -std=c++14 -Wall -Werror -g -pthread -o sortTester sortTester.o BucketSort.o

sortTester.o: sortTester.cpp BucketSort.h
	g++ -std=c++14 -Wall -Werror -g -pthread -c sortTester.cpp

BucketSort.o : BucketSort.h BucketSort.cpp
	g++ -std=c++14 -Wall -Werror -g -pthread -c BucketSort.cpp

clean:
	rm *.o sortTester