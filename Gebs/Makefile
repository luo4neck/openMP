gebsmp gebsmp1 gebs: gebsmp.cc gebsmp1.cc gebs.cc 
	g++ -fopenmp -Wall gebsmp.cc -O2 -o gebsmp
	g++ -fopenmp -Wall gebsmp1.cc -O2 -o gebsmp1
	g++ -Wall gebs.cc -O2 -o gebs

test: gebsmp gebsmp1 gebs 
	./gebsmp -n 1000
	./gebsmp1 -n 1000
	./gebs -n 50

clean: 
	rm *.o core* gebsmp gebsmp1 gebs
