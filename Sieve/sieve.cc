#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<cmath>

using namespace std;

void erato(int n, bool check)// check=1 means print all, check=0 means just check prime or not
{
int i,j, end;
bool *N = (bool*) malloc (sizeof(bool) * (n+1));

if(check) end = n;
else end = sqrt(n) + 1;

for(i=0; i<=n; i=i+2)
{
N[i+1] = 1;
N[i] = 0;
}

printf("2 is prime number.\n");

for(i=3; i<=end; i=i+2)
{
    if(N[i]) 
    {
        if(check) cout<<i<<" is prime number."<<endl<<i+1<<" is not."<<endl;

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

free(N);
}

int main(int argc, char *argv[])
{
int ch;
bool check=0;
opterr = 0;

while((ch = getopt(argc, argv, "n:")) != -1 )
    {
    switch(ch)
        case 'n':
        check = 1;
        break;
    }

if(argc < 2 || argc > 3)
{
cout<<"Wrong input! Please input as:\n $ ./sievemp 100\n$ sievemp -n 100"<<endl;
exit(0);
}
else if(argc == 3 && atoi(argv[2]) > 2)
{
erato(atoi(argv[2]), check);
}
else if(argc == 2 && atoi(argv[1]) > 2)
{
erato(atoi(argv[1]), check);
}
else 
{
cout<<"Wrong input! Please input as:\n $ ./sievemp 100\n$ sievemp -n 100"<<endl;
exit(0);
}

return 0;
}
