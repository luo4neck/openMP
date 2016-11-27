#include<iostream>
#include<stdlib.h>
#include<cmath>
#include<unistd.h>

using namespace std;

void print(double **mat, int size)
{
    for(int i=0; i<size; ++i)
    {
        for(int j=0; j<size; ++j)
        { cout<<mat[i][j]<<", "; }
    	cout<<endl<<endl;
    }
}

void GE(double **mat, double *Y, int size)
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
	cout<<"After gaussian elimation the Matrix is:"<<endl;
	print(mat, size);
}

void BACK(double **mat, double* X, double* Y, int size)
{
    for(int i=size-1; i>=0; --i)
    {
        if(i == size -1) X[i] = Y[i]/mat[i][i];
        else
		{
            double sum=0;
            for (int j=size-1; j>i; --j)
            { sum = sum + mat[i][j] * X[j]; }
           	X[i] = (Y[i] - sum) / mat[i][i];
        }
    }
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

	double **mat=new double*[size], *X=new double[size], *Y=new double[size]; // mat is a nxn matrix and mat * X = Y;

	for(int i=0; i<size; ++i)
	{
		X[i] = 0; 
        Y[i] = rand()%100;
        mat[i] = new double[size];
        for(int j=0; j<size; ++j)
        { mat[i][j] = rand()%100; }
	}
	cout<<"Solving the Matrix equation mat * X = Y with known mat and Y."<<endl<<"Y is:"<<endl;

	for(int i=0; i<size; ++i) cout<<Y[i]<<", ";
	cout<<endl<<endl;

	cout<<"Matrix is :"<<endl;
	print(mat, size);

	GE(mat, Y, size);
	BACK(mat, X, Y, size);

	cout<<"Result is :"<<endl;
	for(int i=0; i<size; ++i) cout<<X[i]<<", ";
	cout<<endl<<endl;

	delete Y;
	delete X;
	for(int i=0; i<size; ++i) delete mat[i];
	delete mat;
	return 0;
}
