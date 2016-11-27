sieve sievemp: sieve.cc sievemp.cc
	g++ -Wall sieve.cc -o sieve
	g++ -fopenmp -Wall sievemp.cc -o sievemp

test: sieve sievemp 
	./sieve -n 100
	./sievemp -n 1000
#./sieve 10000
#./sievemp 10000

clean: 
	rm *.o core* sieve sievemp
