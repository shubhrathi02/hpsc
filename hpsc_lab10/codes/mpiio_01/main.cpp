//
// MPIIO Demonstration Code, Adopted from Multiple References:
//
// References: [1] https://hpc-forge.cineca.it/files/CoursesDev/public/2017/Parallel_IO_and_management_of_large_scientific_data/Roma/MPI-IO_2017.pdf
//             [2] https://cvw.cac.cornell.edu/parallelio/fileviews
//             [3] http://www.math.nsysu.edu.tw/~lam/mpi/PDF/chap6_slidesMY3.pdf
//             [4] https://stackoverflow.com/questions/33537451/writing-distributed-arrays-using-mpi-io-and-cartesian-topology
//             [5] https://cvw.cac.cornell.edu/ParallelIO/

#include<mpi.h>  
#include <cstdlib>

// ==
// ||
// ||  Memory Allocation
// ||
// ==

float  * * Array2D_float(int nRows,int nCols)
{
  float *myArray;
  myArray = new float [ nRows * nCols ];
    
  // Create a pointer that points to the beginning of each new row

  float * * myArray_ptr;
  myArray_ptr = new float * [nRows];

  int count = 0;

  for ( int row = 0 ; row < nRows ; ++ row )
    {
      myArray_ptr[row] = &myArray[ count*nCols ];
      ++count;
    }

  // Return that pointer
  
  return myArray_ptr;
}




//  -
//  |
//  |   O V E R V I E W   O F   T H E
//  |
//  |   G R I D   A N D   P A R A L L E L   S T R U C T U R E
//  |
//  |
//  |   The Grid on this PE:
//  |   ---------------------------------------------------------------------
//  |
//  |   In this example, ( ) are ghost nodes and we are not interested in
//  |   writing their values to a file.  We will create an MPI SubArray
//  |   that represents the real nodes, i.e., those with [ ].
//  |
//  |  (0,4)---(1,4)---(2,4)---(3,4)----(4,4) <--- nRealx+2 , nRealy+2
//  |    |       |       |       |        |  
//  |    |       |       |       |        |  
//  |    |       |       |       |        |  
//  |  (0,3)---[1,3]---[2,3]---[3,3]----(4,3) 
//  |    |       |       |       |        |  
//  |    |       |       |       |        |  
//  |    |       |       |       |        |  
//  |  (0,2)---[1,2]---[2,2]---[3,2]----(4,2)
//  |    |       |       |       |        |  
//  |    |       |       |       |        |  
//  |    |       |       |       |        |  
//  |  (0,1)---[1,1]---[2,1]---[3,1]----(4,1)
//  |    |       |       |       |        |  
//  |    |       |       |       |        |  
//  |    |       |       |       |        |  
//  |  (0,0)---(1,0)---(2,0)---(3,0)----(4,0)
//  | 0,0                              4,0
//  |
//  |   
//  |   Global Grid:
//  |   ---------------------------------------------------------------------
//  |
//  |   In this example, the real nodes do not overlap at the PE boundary,
//  |   unlike what we did for esPIC.  So, when we write values to the file
//  |   we want to write all of the real nodes values.  The global grid is
//  |   shown below.  Note that the global grid does not exist on any one
//  |   PE.  But we want to write each PE's part of the grid to a single
//  |   file such that, when viewed by other software later, it looks as
//  |   though the global grid were, indeed, just on one PE.  In the figure
//  |   below, the global grid is shown, but some connecting lines are
//  |   missing in order to highlight the fact that different parts of the
//  |   global array existed on different PE.
//  |
//  |                                       
//  |	  [13,3]--[14,3]--[15,3]  [16,3]--[17,3]--[18,3]
//  |	     |       |       |       |       |       |  
//  |	     |       |       |       |       |       |  
//  |	     |       |       |       |       |       |  
//  |	  [13,2]--[14,2]--[15,2]  [16,2]--[17,2]--[18,2]
//  |	     |       |       |       |       |       |  
//  |	     |       |       |       |       |       |  
//  |	     |       |       |       |       |       |  
//  |	  [13,1]--[14,1]--[15,1]  [16,1]--[17,1]--[18,1]
//  |
//  |
//  |      [7,3]---[8,3]---[9,3]  [10,3]--[11,3]--[12,3]
//  |	     |       |       |       |       |       |  
//  |	     |       |       |       |       |       |  
//  |	     |       |       |       |       |       |  
//  |      [1,2]---[2,2]---[3,2]   [4,2]---[5,2]---[6,2]
//  |	     |       |       |       |       |       |  
//  |	     |       |       |       |       |       |  
//  |	     |       |       |       |       |       |  
//  |	   [1,1]---[2,1]---[3,1]   [4,1]---[5,1]---[6,1]
//  |
//  -

// ==
// ||
// ||  Main
// ||
// ==

int main(  int argc, char *argv[] )
{

  int nRealx  = 5;    // Number of real points in the x- and y-directions
  int nRealy  = 5;    //
  int myPE, numPE;    // This process' ID and the number of processes total
  int nPEx,  nPEy;    // Number of processes in the x- and y-directions, in the 2D grid of processes
  int iPE ,   jPE;    // This process' logical ID in the 2D array of processors

  // (1) Initialize MPI

  MPI_Init      ( &argc          , &argv  );
  MPI_Comm_size ( MPI_COMM_WORLD , &numPE );
  MPI_Comm_rank ( MPI_COMM_WORLD , &myPE  );

  // (2) In this example, we are not letting the user choose the number of PEs.  It is a 2x2 logical
  //     grid of PEs.  The formulas for iPE and jPE are from the previous mpiInfo.h

  nPEx = 2;
  nPEy = 2;
  
  jPE = int(myPE/nPEx);
  iPE = myPE - jPE*nPEx;

  // (3) Add ghost nodes
  
  int nTotalx = nRealx + 2;
  int nTotaly = nRealy + 2;
  
  // (4) Allocate memory for and populate the real nodes' entries in the 2D array we wish to write
  
  float **A = Array2D_float(nTotalx, nTotaly);
  for (int i = 1; i <= nRealx; i++) 
    for (int j = 1; j <= nRealy; j++)
      A[i][j] = i + 10.*j + myPE/10.;
  
  // (5) Create MPI derived data type that will be used to represent the real nodes in the grid.

  // (5.1) Set up specifications for the datatype, which we will call "myRealNodes"
  
  MPI_Datatype myRealNodes;
  int idxStartThisPE  [2] = { 1        , 1        };  // Index coordinates of the sub-array inside this PE's array, A
  int AsizeThisPE     [2] = { nTotalx  , nTotaly  };  // Size of the A array on this PE    
  int sub_AsizeThisPE [2] = { nRealx   , nRealy   };  // Size of the A-sub-array on this PE

  // (5.2) Ask MPI to create that new datatype
  //
  // ------------------------------------------------------------------------------------------------------
  //
  // Number of elements in A in each dimension                   Starting indices of the subarray inside
  //                          of the sub array                   the this PE's A array
  //                                         |                   |
  //  Number of elements in each             |                   |           Order of the A array in memory, row
  // dimension of this PE's array            |                   |           major or column major
  //                            |            |                   |           |
  //    Number of dimensions    |            |                   |           |
  //                       |    |            |                   |           |     
  //                       |    |            |                   |           |     
  MPI_Type_create_subarray(2, AsizeThisPE, sub_AsizeThisPE, idxStartThisPE, MPI_ORDER_C, MPI_FLOAT, &myRealNodes);
  //
  // ------------------------------------------------------------------------------------------------------
  
  MPI_Type_commit(&myRealNodes);
  
  // (6) Create a second derived type that will be used to describe the whole array, as it exists
  //     across PEs.  This next datatype describes this PE's relationship between its part of the
  //     array, A, and the global A array across PE.  It is used to create the "view" of the file
  //     from this PE's perspective, i.e., where in that file this PE should write its data.
  
  MPI_Datatype myPartOfGlobal;
  int idxStartInGlobal [2] = { iPE * nRealx , jPE * nRealy };  // Index cordinates of the sub-arrayinside the global array
  int AsizeGlobal      [2] = { nPEx* nRealx , nPEy* nRealy };  // Size of the global array
  
  // ------------------------------------------------------------------------------------------------------
  //
  //      Number of elements in each dimension              Starting indices of the subarray inside
  //                          of the sub array              the this PE's array
  //                                         |              |
  //  Number of elements in each             |              |                    Order of the array in memory, row
  //  dimension of this PE's array           |              |                    major or column major
  //                            |            |              |                    |
  //    Number of dimensions    |            |              |                    |
  //                       |    |            |              |                    |     
  //                       |    |            |              |                    |     
  MPI_Type_create_subarray(2, AsizeGlobal, sub_AsizeThisPE, idxStartInGlobal, MPI_ORDER_FORTRAN, MPI_FLOAT, &myPartOfGlobal);
  //
  // ------------------------------------------------------------------------------------------------------
  
  MPI_Type_commit(&myPartOfGlobal);
  
  // (7) Set the "view" of the file from this PE's perspective, i.e., where and how this PE should write data
  
  MPI_File fh;
  
  MPI_File_open(MPI_COMM_WORLD, "output.bin", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
  
  // ------------------------------------------------------------------------------------------------------
  //
  //
  //     Displacement in file to     Contains the description
  //     accommodate other data      of how this PE's "A" array
  //     written before this data    fits into the global "A".
  //                     |                 |
  //                     |                 |
  MPI_File_set_view (fh, 0, MPI_FLOAT, myPartOfGlobal, "native", MPI_INFO_NULL);
  //                           |                                     |
  //                           |                                     |
  //                       The data type of each element     Could contain optimization hints,
  //                                                         e.g., striping information
  // ------------------------------------------------------------------------------------------------------

  //  (8) Perform the collective write operation

  
  // ------------------------------------------------------------------------------------------------------
  //                            The sub_array describing this
  //        File pointer        PE's real-nodes part of A
  //                 |                   |
  //                 |                   |
  MPI_File_write_all(fh, &A[0][0], 1, myRealNodes, MPI_STATUS_IGNORE);
  //                       |
  //                       |
  //                 Memory location of original
  //                 data set for which the
  //                 sub arrays were created.
  // ------------------------------------------------------------------------------------------------------

  MPI_File_close(&fh);
  
  // (9) Cleanup 
  
  free(A[0]);
  free(A);
  
  MPI_Type_free(&myPartOfGlobal);
  MPI_Type_free(&myRealNodes);
  MPI_Finalize();
  
  return 0;

}




