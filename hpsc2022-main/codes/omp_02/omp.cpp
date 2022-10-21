#include "omp.h"

// ==
// ||
// ||  OMP Laplacian Demo Code
// ||
// ==


int main(  int argc, char *argv[] ) 
{

  // Start the timer for the entire execution

  struct timespec t0, t1;
  StartTimer(t0);


  // Default values for user input
    
  int nCell = 190;  // nCell is for the x- and y-directions
  int numTH = 1;   // Number of threads

  // Gather user input from the command line

  for (int count =  0 ; count < argc; ++count)
    {
      if ( !strcmp(argv[count],"-nCell"    ) ) nCell     = atoi(argv[count+1]);  
      if ( !strcmp(argv[count],"-nTH"      ) ) numTH     = atoi(argv[count+1]);
    }

  // Set the number of threads for this execution

  omp_set_num_threads(numTH);

  // Set up grid and linear system information

  double dh          = 1./nCell;        // Size of each cell in each direction (dx and dy)
  int nPtsx          = nCell+1;         // Number of points in the x-direction
  int nPtsy          = nCell+1;         //   "    "    "    "   "  y-    "
  int nField         = nPtsx * nPtsy;   // Total number of points (and #rows in linear system)
  int bandwidth      = 5;               // Number of columns in the sparse-matrix format
  int max_iter       = nField*10;       // Number of allowed Jacobi iterations
  int numTHconverged = 0;               // Number of threads converged
    
  double   * phi;                       // The unknown for which we are solving A*phi = b
  
  phi     = Array1D_double(nField);     // Acquire memory for this shared variable.

  // This shared array will store either a 0 or 1 for each thread, 0 meaning not congverged
  // 1 meaning converged.
  
  int *THconverged = new int [  /* TO-DO */ ];
  

// =================================================================================== //
// =======================         BEGIN PARALLEL REGION          ==================== //
// =================================================================================== //
  
#pragma omp parallel shared(numTHconverged)
  {
    int myTH = omp_get_thread_num();  

    // ------------------------------------------------------------------------------------
    // (1) Compute this thread's bounds in the global system:
    //
    //     numPerTH = number of rows per thread (the last thread may be slightly different)
    //     Lower    = the first row in the global matrix to be handled by this thread
    //     Upper    = the last   "  "   "    "      "    "  "    "     "   "      "
    // ------------------------------------------------------------------------------------

    int numPerTH = /* TO-DO */ 
    int Lower    = /* TO-DO */ 
    int Upper    = /* TO-DO */ 

    //  (1.1) Adjust Upper for the last (highest numbered) thread to ensure all the rows of the global system are handled
    
    /* TO-DO  one line */ 

    // ------------------------------------------------------------------------------------
    // (2) Aquire memory for this thread
    //
    //     nField_TH = the number of field variables (phi) to be handled by this thread
    // ------------------------------------------------------------------------------------

    int nField_TH =  /* TO-DO */ 

    double * * Acoef;
    int    * * Jcoef;
    double   * b;
    double   * phiNew;

    //     Acoef, Jcoef, b, and phiNew are not the global matrices and arrays; here they contain only the
    //     rows being handled by this thread.
    
    Acoef    = Array2D_double( /* TO-DO */ );
    Jcoef    = Array2D_int   ( /* TO-DO */ );
    b        = Array1D_double( /* TO-DO */ );
    phiNew   = Array1D_double( /* TO-DO */ );
    
    // ------------------------------------------------------------------------------------
    // (3) Initialize the linear system and form the initial guess for phi
    // ------------------------------------------------------------------------------------

    for ( int row = 0 ; row <  /* TO-DO */ ); ++row ) 
      {
	for ( int col = 0 ; col < bandwidth ; ++col )
	  {
	    Acoef[row][col] = 0.;
	    Jcoef[row][col] = 0 ;
	  }
	b[row] = 0.;
      }
    
    for ( int row = Lower ; row <=  /* TO-DO */  ; ++row ) 
      {
	phi[row] = 0.;
      }
    
    // ------------------------------------------------------------------------------------
    // (4) Form the linear system.  Here "pt" represents "point" number in the mesh.  It is
    //     equal to the row number in the linear system, too.
    // ------------------------------------------------------------------------------------
    
    for ( int pt = /* TO-DO */ ; pt <= Upper ; ++pt ) 
      {

	// Using the same logic as for converting myPE to iPE and jPE,
	// compute the i,j logical coordinates of "pt" in the mesh:
	
	int j = int(pt/nPtsx);  
	int i = pt - j*nPtsx;   

	// Compute the row number local to this thread, relative to its Acoef/Jcoef arrays
	
	int pt_TH = /* TO-DO */

	// Populate the linear system for all interior points using that local row number

	if ( i > 0 && i < nPtsx-1 )
	  if ( j > 0 && j < nPtsy-1 )
	    {
	      Acoef[/* TO-DO */][0] = -4./dh/dh;  Jcoef[/* TO-DO */][0] = pt;      
	      Acoef[/* TO-DO */][1] =  1./dh/dh;  Jcoef[/* TO-DO */][1] = pt - 1;  
	      Acoef[/* TO-DO */][2] =  1./dh/dh;  Jcoef[/* TO-DO */][2] = pt + 1;  
	      Acoef[/* TO-DO */][3] =  1./dh/dh;  Jcoef[/* TO-DO */][3] = pt + nPtsx;
	      Acoef[/* TO-DO */][4] =  1./dh/dh;  Jcoef[/* TO-DO */][4] = pt - nPtsx;
	    }

	// Apply boundary conditions
	
	if ( i == 0       ) { Acoef[/* TO-DO */][0] = 1. ; Jcoef[/* TO-DO */][0] = pt; b[/* TO-DO */] =  1.; } 
	if ( i == nPtsx-1 ) { Acoef[/* TO-DO */][0] = 1. ; Jcoef[/* TO-DO */][0] = pt; b[/* TO-DO */] = -1.; } 
	if ( j == 0       ) { Acoef[/* TO-DO */][0] = 1. ; Jcoef[/* TO-DO */][0] = pt; b[/* TO-DO */] = -1.; } 
	if ( j == nPtsy-1 ) { Acoef[/* TO-DO */][0] = 1. ; Jcoef[/* TO-DO */][0] = pt; b[/* TO-DO */] =  1.; } 
      }

    // ------------------------------------------------------------------------------------
    // (5) Perform Jacobi iterations
    // ------------------------------------------------------------------------------------

    int iter       = 0;
    int thisThreadConverged;

    #pragma omp barrier

    // numTHconverged counts the number of threads converged.  We do not care which thread initalizes it.
    
    #pragma omp /* TO-DO */  
    {
      numTHconverged = 0;
    }

    // Iterate until all of the threads have converged or we have exceeded the number of allowed iterations
  
    while ( /* TO-DO */  && ++iter <= max_iter )  
      {
	thisThreadConverged = 1;

	for ( /*  Global matrix loop bounds for int row */ )
	  {
             int row_TH = /* TO-DO: Convert row variable to thread local row */;
	    
	      phiNew[row_TH] = b[  /* TO-DO */];
	    
	    for ( int col = 1 ; col < bandwidth ; ++col ) phiNew[ /* TO-DO */] -= Acoef[ /* TO-DO */][col] * phi[ Jcoef[ /* TO-DO */][col] ];  
	    
	    phiNew[ /* TO-DO */] /= Acoef[ /* TO-DO */][0];
	    
	    if ( fabs(phiNew[ /* TO-DO */] -  phi[row] ) > 1.e-10 ) thisThreadConverged = 0;  

	  }

	// (5.1) Record in shared array if this thread converged or not
	
	/* To-DO */[myTH] = thisThreadConverged;

	// (5.2) Count the number of threads that have converged.  We do not care which thread does the counting.

        #pragma omp /* TO-DO */
	{
	  numTHconverged = 0;
	  for ( int i = 0 ; i < numTH ; ++i ) numTHconverged += /* TO-DO */;
	  if ( numTHconverged == numTH ) printf("Jacobi converged in %d iterations.",iter);
	}

	// (5.3) Update the shared/global array phi with this thread's values
	
	for ( /*  Global matrix loop bounds for int row */ )    
	  {
	    int row_TH = /* TO-DO:  Convert row variable to thread local row */;
	    phi[row] = phiNew[ /* TO-DO */]; 
	  }
      }

  }

// =================================================================================== //
// =======================           END PARALLEL REGION          ==================== //
// =================================================================================== //
  
  if ( numTHconverged != numTH ) printf("WARNING: Jacobi did not converge.\n");
  
  EndTimer("main", t0,t1);
  
  plot("phi",phi,nPtsx,nPtsy,dh);
  return 0;
}

