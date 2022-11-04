#include<mpi.h>     // Required for MPI
#include<stdio.h>
#include "main.h"




void printArray(string arrayName, int n , int *array , int myPE)
{
  for ( int i = 0 ; i < n ; ++i ) cout << "myPE: " << myPE << " " << arrayName << "[" << i << "] = " << array[i] << endl;
}




void printVal(string valName, int val , int myPE)
{
  cout << "myPE: " << myPE << " " << valName << " = " << val << endl;
}





int main(  int argc, char *argv[] )
{
  // (1) ------------------- Initialize Parallel Region ----------------------------

  MPI_Init     (&argc         , &argv       );

  int numPE = 4;                              // Must match mpirun command
  int myPE;
  MPI_Comm_size(MPI_COMM_WORLD,  &numPE);     // Set the number of processors
  MPI_Comm_rank(MPI_COMM_WORLD, &myPE  );     // Get my PE number


  cout << "myPE: " << myPE << " Hello, world!" << endl;
  
  // (2) -------------------    Broadcast               ---------------------------

  int n = 10;
  int a[n];

  if ( myPE == 0 ) for ( int i = 0 ; i < 10; ++i ) a[i] = 333;
  
  MPI_Bcast( a , n , MPI_INT, 0, MPI_COMM_WORLD);

  printArray("BCast a",n,a,myPE);


  // (3) -------------------     Scatter                ---------------------------

  int m = 40;
  int b[m];

  if ( myPE == 0 ) for ( int i = 0 ; i < 40; ++i ) b[i] = i;
  
  MPI_Scatter( b , n , MPI_INT,
	       a , n , MPI_INT, 0 , MPI_COMM_WORLD);

  printArray("Scatter a",n,a,myPE);  // Results good on all PE

  // (4) -------------------     Gather                 ---------------------------


  for ( int i = 0 ; i < n ; ++i ) a[i] = myPE;
  
  MPI_Gather( a , n , MPI_INT,
	      b , n , MPI_INT, 0 , MPI_COMM_WORLD);

  printArray("Gather b",m,b,myPE);  // Results only valid on PE 0

  // (5) -------------------     Reduce                 ---------------------------

  int myVal = myPE;

  int myReducedVal;

  MPI_Reduce(&myVal, &myReducedVal, 1, MPI_INT, MPI_SUM, 0 , MPI_COMM_WORLD);

  printVal("SummedValue",myReducedVal,myPE);  // Result only valid on PE 0

  // Also can do MPI_MAX, MPI_MIN, MPI_PROD...

  // (6) -------------------     AllReduce              ---------------------------

  MPI_Allreduce(&myVal, &myReducedVal, 1 , MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  
  printVal("SummedValue",myReducedVal,myPE);  // Result valid on all PE

  // (7) -------------------     AllGather              ---------------------------

  int myContribution[n];
  for ( int i = 0 ; i < n ; ++i ) myContribution[i] = myPE;
  int everyonesContribution[m];

  printArray("myContribution",n,myContribution,myPE);  // Result valid on all PE
  
  MPI_Allgather( myContribution        ,  n , MPI_INT,
                 everyonesContribution ,  n , MPI_INT, MPI_COMM_WORLD );
  
  printArray("everyonesContribution",m,everyonesContribution,myPE);  // Result valid on all PE

  
  // (8) -------------------     End Parallel Region    ---------------------------

  MPI_Barrier(MPI_COMM_WORLD);  
  MPI_Finalize();

  
  return 0;



  
}



