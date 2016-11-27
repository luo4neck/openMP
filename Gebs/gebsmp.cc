/* 
function GE and GEMP(with openmp code inside) doing gaussian elimation job, both of them call function BACK to do back substituation and print the result..
*/
#include<string.h>
#include<iostream>
#include<fstream>
#include<sys/time.h>
#include<stdlib.h>
#include<omp.h>
#include<unistd.h>

using namespace std;

void print(double **mat, int size)// just print the matrix out..
{
    for(int i=0; i<size; ++i)
    {
        for(int j=0; j<size; ++j)
        {
			cout<<mat[i][j]<<", "; 
		}
	    cout<<endl<<endl;
    }
}


void BACK(double **mat, double* Y, int size)// back subsititution part.. 
{
// difficult to implement openmp code into this part, both side of for loop are iteration..
	double *X=new double[size];
	cout<<"Result is :"<<endl;
    for(int i=size-1; i>=0; --i)
    {
        if(i == size -1) X[i] = Y[i]/mat[i][i];
        else
		{
            double sum=0;
            for (int j=size-1; j>i; --j)
            {
				sum = sum + mat[i][j] * X[j]; 
			}
            X[i] = (Y[i] - sum) / mat[i][i];
		}
    }

	for(int i=0; i<size; ++i) cout<<X[i]<<", ";
	delete X;
	cout<<endl<<endl;
}


void GE(double **mat, double *Y, int size) // this function doing the openmp gaussian elimation part..
{
	double m;
    for(int j=0; j<size-1; ++j)
    {
        for(int i=j+1; i<size; ++i)
        {
        	m = mat[i][j]/mat[j][j];
            for(int k=j; k<size; ++k)
            {
            	mat[i][k] = mat[i][k] - m * mat[j][k];
            }
        	Y[i] = Y[i] - m * Y[j];
        }
    }
	BACK(mat, Y, size);
}


void GEMP(double **mat, double *Y, int size)// this function doing the serial gaussian elimation part..
{
	double m;
    for(int j=0; j<size-1; ++j)
    {
        for(int i=j+1; i<size; ++i)
        {
    	    m = mat[i][j]/mat[j][j];
        	int k;
        	#pragma omp parallel for private(k)
        	for(k=j; k<size; ++k) // using openmp here because all the mat[i][k] will be calculated seperatedly..
            {
            	mat[i][k] = mat[i][k] - m * mat[j][k];
            }
        	Y[i] = Y[i] - m * Y[j];
        }
    }
	BACK(mat, Y, size);
}


int main(int argc, char *argv[])
{
	int ch, size=0;
	opterr = 0;

	while((ch = getopt(argc, argv, "n:")) != -1 )
	{
    	switch(ch)
        case 'n': size = atoi(argv[2]); break;
	}

	if(argc != 3)
	{
    	cout<<"Wrong input! Please input as $ ./gebs -n 100"<<endl;
    	exit(0);
	}
	else
	{
 		int i=0;
    	while(argv[2][i])
        {		 
        	if( isdigit(argv[2][i]) == 0 ) // check if the number after n is a digit number..
        	{cout<<"Wrong input! Please input as $ ./gebs -n 100"<<endl, exit(0); }
    		i++;
    	}
	}

	/* ======= input checking part finished, memery allocating begin ======= */

	double **mat=new double*[size], *Y=new double[size]; // mat is a nxn matrix and mat * X = Y;
	double **mattmp=new double*[size], *Ytmp=new double[size]; // mat is a nxn matrix and mat * X = Y;
	for(int i=0; i<size; ++i)
	{
    	Y[i] = rand()%100; // give random value to Y
	    while(Y[i]==0) // give a new value if just given a 0
    	{ Y[i] = rand()%100; }
    
	    mat[i] = new double[size];
    	mattmp[i] = new double[size];
	    for(int j=0; j<size; ++j)
    	{
        	mat[i][j] = rand()%100; // give random value to mat..
        	while(mat[i][j] == 0) // give a new value if just given a 0
        	{  mat[i][j] = rand()%100; } 
    	}
	}
	cout<<"Solving the Matrix equation mat * X = Y with known mat and Y."<<endl;

/*
cout<<"Y is:"<<endl;
for(int i=0; i<size; ++i) cout<<Y[i]<<", ";
cout<<endl<<endl;

cout<<"Matrix is :"<<endl;
print(mat, size);
*/

/* === memery allocating part finished, calculating part with time recording begin == */

	struct timeval tm1, tm2;

	memcpy(mattmp, mat, sizeof(double *)*size);// copy mat to a new matrix to calculate.. because the value in the matrix will be changed..
	memcpy(Ytmp, Y, sizeof(double*));// same with the above line..

	gettimeofday(&tm1, NULL);
	GE(mattmp, Ytmp, size);// gaussian elimation without openmp running here..
	gettimeofday(&tm2, NULL);

	cout<<"GE Used: "<<(tm2.tv_sec - tm1.tv_sec)*1000000 + tm2.tv_usec - tm1.tv_usec<<" us."<<endl;
	memcpy(mattmp, mat, sizeof(double *)*size);// copy mat to a new matrix to calculate.. because the value in the matrix will be changed..
	memcpy(Ytmp, Y, sizeof(double*));// same with the above line..

	gettimeofday(&tm1, NULL);
	GEMP(mattmp, Ytmp, size);// gaussian elimation with openmp running here.. 
	gettimeofday(&tm2, NULL);

	cout<<"GEMP Used: "<<(tm2.tv_sec - tm1.tv_sec)*1000000 + tm2.tv_usec - tm1.tv_usec<<" us."<<endl;
	delete[] Y;
	delete[] Ytmp;
	for(int i=0; i<size; ++i) 
	{
		delete mat[i];
//delete mattmp[i];
	}
	delete[] mat;
	delete[] mattmp;
	return 0;
}
