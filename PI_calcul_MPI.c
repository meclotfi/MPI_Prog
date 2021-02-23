#include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <mpi.h>
# define PI 3.1415926535L
double f(double a) {return (double)4.0/((double)1.0+(a*a));}
# define MTAG1 1
# define MTAG2 2

int main ()
{
int n, myid, numprocs, i;
double mypi, pi, h, sum, x;
MPI_Status status;
MPI_Init(NULL, NULL);
MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
MPI_Comm_rank(MPI_COMM_WORLD,&myid);
n = 0;
for (;;) {
if (myid == 0) {
        printf (" Enter the number of intervals: (0 quits )");
        scanf ("%d",&n);
        MPI_Send(&n,1,MPI_INT,1,MTAG1,MPI_COMM_WORLD);
        } 
        else 
        {
                MPI_Recv(&n,1,MPI_INT,0,MTAG1,MPI_COMM_WORLD,&status);
}
if (n == 0) break;
else {
h = 1.0 / (double) n;
sum = 0.0;
for (i = myid + 1; i <= n; i += numprocs ) {
x = h * (( double )i - 0.5);
sum += f(x);
}
mypi = h * sum ;
if ( myid != 0) {
        MPI_Send(&mypi,1,MPI_DOUBLE,0,MTAG2,MPI_COMM_WORLD);
} else {
        pi = mypi ;
        MPI_Recv(&mypi,1,MPI_DOUBLE,1, MPI_Any_Tag,MPI_COMM_WORLD,& status);
        printf("I receievd %.16f from other",mypi);
        pi += mypi;
        printf("PI is approximately %.16f, Error is %.16f\n",pi,fabs (pi - PI ));
}
}
}
MPI_Finalize();
return 0;
}