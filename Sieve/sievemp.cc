#include<fstream>
#include<sys/time.h>
#include<iostream>
#include<stdlib.h>
#include<cmath>
#include<omp.h>

// in the plotted graph, green stands for the function with openmp code while red line stands for the function without openmp code..
// X axis stands for the input number, Y axis stands for the time spend..

using namespace std;

void erato(int n, bool check)// check=1 means print all, check=0 means just check prime or not
{
int i,j, end;
bool *N = (bool*) malloc (sizeof(bool) * (n+1));

if(check) end = n;
else end = sqrt(n) + 1;

for(i=0; i<=n; ++i)// just odd numbers are set to 1, even numbers are set to 0..
{
N[i+1] = 1;
N[i] = 0;
i=i+1;
}
cout<<"2 is prime number."<<endl;

for(i=3; i<=end; i=i+2) //even numbers cant be prime except 2, so the outside loop controlled by i=i+2 will speed up the program..
{
    if(N[i]) 
    {
        if(check) 
        cout<<i<<" is prime number."<<endl<<i+1<<" is not."<<endl;
        
        for(j=i; j<=n; ++j)
        {
            if(j%i==0 && N[j])
            N[j] = 0;
        }
    }
    
    else if(check)
    cout<<i<<" is not."<<endl<<i+1<<" is not."<<endl;
}

if(!check)
{
    if(N[n]) cout<<n<<" is a prime number."<<endl;
    else     cout<<n<<" is not prime number."<<endl;
}
delete N;
}


void eratomp(int n, bool check)// check=1 means print all, check=0 means just check prime or not
{
int i,j, end;
bool *N = (bool*) malloc (sizeof(bool) * (n+1));

if(check) end = n;
else end = sqrt(n) + 1;

#pragma omp parallel for
for(i=0; i<=n; ++i)// just odd numbers are set to 1, even numbers are set to 0..
{
N[i+1] = 1;
N[i] = 0;
i=i+1;
}
cout<<"2 is prime number."<<endl;

for(i=3; i<=end; i=i+2) //even numbers cant be prime except 2, so the outside loop controlled by i=i+2 will speed up the program..
{
    if(N[i]) 
    {
        if(check) 
        cout<<i<<" is prime number."<<endl<<i+1<<" is not."<<endl;
        
        #pragma omp parallel for private(j)
        for(j=i; j<=n; ++j)
        {
            if(j%i==0 && N[j])
            N[j] = 0;
        }
    }
    
    else if(check)
    cout<<i<<" is not."<<endl<<i+1<<" is not."<<endl;
}

if(!check)
{
    if(N[n]) cout<<n<<" is a prime number."<<endl;
    else     cout<<n<<" is not prime number."<<endl;
}
delete N;
}


int main(int argc, char *argv[])
{
int ch;
unsigned int n;
bool check=0;
opterr = 0;

while((ch = getopt(argc, argv, "n:")) != -1 )
    {
    switch(ch)
        case 'n': check = 1; break;
    }

if(argc < 2 || argc > 3)
{
cout<<"Wrong input! Please input as:\n $ ./sievemp 100\n$ sievemp -n 100"<<endl;
exit(0);
}
else if(argc == 3 && atoi(argv[2]) > 2)
{
n = atoi(argv[2]); //erato(atoi(argv[2]), check);
}
else if(argc == 2 && atoi(argv[1]) > 2)
{
n = atoi(argv[1]); //erato(atoi(argv[1]), check);
}
else 
{
cout<<"Wrong input! Please input as:\n $ ./sievemp 100\n$ sievemp -n 100"<<endl;
exit(0);
}

/* ======= time recording area begin ============ */
struct timeval tm1, tm2, tm3;
ofstream file("plot.dat");

gettimeofday(&tm1, NULL);// time record before the non-openmp function start..

erato(n, check);

gettimeofday(&tm2, NULL);// time record after the non-openmp function finish, ..

eratomp(n, check);

gettimeofday(&tm3, NULL);// time record after the openmp function finish..

file<<n<<" "<<(tm2.tv_sec - tm1.tv_sec)*1000000 + tm2.tv_usec - tm1.tv_usec<<" "<<(tm3.tv_sec - tm2.tv_sec)*1000000 + tm3.tv_usec - tm2.tv_usec<<endl;
//write the time marker into plot.dat..

// begin the function running part with input = n/10
gettimeofday(&tm1, NULL);

erato(n/10, check);

gettimeofday(&tm2, NULL);

eratomp(n/10, check);

gettimeofday(&tm3, NULL);

file<<n/10<<" "<<(tm2.tv_sec - tm1.tv_sec)*1000000 + tm2.tv_usec - tm1.tv_usec<<" "<<(tm3.tv_sec - tm2.tv_sec)*1000000 + tm3.tv_usec - tm2.tv_usec<<endl;

// begin the function running part with input = n/100
gettimeofday(&tm1, NULL);

erato(n/100, check);

gettimeofday(&tm2, NULL);

eratomp(n/100, check);

gettimeofday(&tm3, NULL);

file<<n/100<<" "<<(tm2.tv_sec - tm1.tv_sec)*1000000 + tm2.tv_usec - tm1.tv_usec<<" "<<(tm3.tv_sec - tm2.tv_sec)*1000000 + tm3.tv_usec - tm2.tv_usec<<endl;

file.close();

/* =========== time data had been stored in plot.dat, begin plotting ========== */

FILE *gp = popen("gnuplot -persist", "w");

if(gp == NULL)
{
cout<<"Cannot plot the data!"<<endl;
exit(0);
}

fprintf(gp, "set logscale xy\n");
fprintf(gp, "plot 'plot.dat' u 1:2 w l, 'plot.dat' u 1:3 w l\n");
fprintf(gp, "pause -1\n");
fclose(gp);
return 0;
}
