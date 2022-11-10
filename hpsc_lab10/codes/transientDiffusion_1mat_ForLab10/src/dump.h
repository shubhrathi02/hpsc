//  ================================================================================
//  ||                                                                            ||
//  ||  M P I I O   D u m p   W r i t e r                                         ||
//  ||                                                                            ||
//  ================================================================================

void write_mpiio_dump(mpiInfo &myMPI)
{

  printf("************* Writing dump file *****************\n");

  //rLOOP
    //printf("phi: %f", phi[r]);


  // Using the hpsc2022/codes/mpiio_01/main.cpp sample code from class lecture as a 
  // guide, write phi to a single output file from any number of PEs using the
  // MPIIO library.  Consider two approaches:
  //
  // (1) Adapt the sample code to a 1D array
  // (2) Adapt phi to a 2D array and use the sample code as a start
  //
  // Either way, note that the sample code only works for a nPEx = nPEy = 2
  // situation. 
  //
  // Be sure to check the output using sb_py3.py
  //
  // Also, be sure to run a test case where you set phi equal to test values to
  // demonstrate that it is being written correctly to the binary output file.
  //
  // 

  MPI_Datatype myPartOfGlobal;
  

  

  
  
  int idxStartThisPE  [1] = { 1  };  // Index coordinates of the sub-array inside this PE's array, A
  int AsizeThisPE     [1] = { nField + 2 };  // Size of the A array on this PE    
  int sub_AsizeThisPE [1] = { nField };  // Size of the A-sub-array on this PE

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
  MPI_Type_create_subarray(1, AsizeThisPE, sub_AsizeThisPE, idxStartThisPE, MPI_ORDER_C, MPI_DOUBLE, &myPartOfGlobal);

  MPI_Type_commit(&myPartOfGlobal);
  

  MPI_File fh;
  MPI_File_open(MPI_COMM_WORLD, "phi_dump.bin", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

  MPI_File_set_view (fh, 0, MPI_DOUBLE, myPartOfGlobal, "native", MPI_INFO_NULL);

  MPI_File_write_all(fh, &phi[1], 1, myPartOfGlobal, MPI_STATUS_IGNORE);

  MPI_File_close(&fh);

  MPI_Type_free(&myPartOfGlobal);
  //MPI_Finalize();
  return;
  
}






