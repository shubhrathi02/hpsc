#include "omp.h"


void printVal(string task , string valName, int val )
{
  printf("%s myTH: %d %s = %d\n",task.c_str(),omp_get_thread_num(),valName.c_str(),val);
}

void printVal(string task , string valName, double val )
{
  printf("%s myTH: %d %s = %f\n",task.c_str(),omp_get_thread_num(),valName.c_str(),val);
}


// ==
// ||
// ||  M A I N
// ||
// ==


int main(  int argc, char *argv[] )
{

  struct timespec t0, t1;

  cout << "\n\n";
  cout << " --------------------------------------------------------\n";
  cout << " |                                                      |\n";
  cout << " |      O M P  D E M O  C O D E                         |\n";
  cout << " |                                                      |\n";
  cout << " --------------------------------------------------------\n";
  cout << "\n\n";




  cout << "\n\n( 1) ------- Hello world ------------------------------\n\n";
    
  int maxTH =     omp_get_max_threads();

  cout << "Max threads = " << maxTH << endl;





  cout << "\n\n( 2) - ----- Simple parallel region -------------------\n\n";
  
  omp_set_num_threads(4);
  
  #pragma omp parallel
  {
    printf("( 2) myTH: %d Hello, world!\n", omp_get_thread_num());
  }





  cout << "\n\n( 3) ------ Race condition on shared variable --------\n\n";

  int shared_var;
  omp_set_num_threads(3);

  #pragma omp parallel
  {
    shared_var = omp_get_thread_num();
  }

  cout << "( 3) shared_var = " << shared_var << endl;





  cout << "\n\n( 4) ------ No race condition on private variable ----\n\n";

  omp_set_num_threads(3);

  #pragma omp parallel
  {
    int private_var = omp_get_thread_num();
    printVal("( 4)", "private_var",private_var);
  }




  cout << "\n\n\n( 5) ****** Parallelize a loop (compare) *************\n\n";

  omp_set_num_threads(maxTH);
  



  cout << "\n( 5) ------ * Serial version * -----------------------\n\n";

  int n = maxTH * 1000000;  int *a ; a = new int [n];

  StartTimer(t0);

  for ( int i = 0 ; i < n ; ++i ) a[i] = i*i;

  EndTimer("Set Array: Serial", t0, t1);




  cout << "\n( 5 ) ------ * OMP version * --------------------------\n\n";

  StartTimer(t0);

  #pragma omp parallel for
  for ( int i = 0 ; i < n ; ++i ) a[i] = i*i;

  EndTimer("Set Array: OMP", t0, t1);




  cout << "\n\n\n( 6) ****** Reduction (compare) **********************\n\n";

  double sum;

  cout << "\n( 6) ------ * Serial Version * -----------------------\n\n";
  
  sum = 0.;

  StartTimer(t0);

  for ( int i = 0 ; i <= n ; ++i ) sum += a[i];

  cout << "serial sum = " << sum << endl;
  
  EndTimer("Scalar Sum: Serial",t0,t1);






  cout << "\n( 6 ) ------ * OMP Version * --------------------------\n\n";
  
  sum = 0.;

  StartTimer(t0);

  #pragma omp parallel for reduction(+:sum)
  for ( int i = 0 ; i <= n ; ++i )
    {
      sum += a[i];
    }

  cout << "omp sum = " << sum << endl;
  
  EndTimer("Scalar Sum: OMP",t0,t1);





  
  cout << "\n\n\n( 7) ------ private directive ------------------------\n\n";

  int m = maxTH * 10;
  double *c ; c = new double[m];

  #pragma omp parallel for 
  for ( int i = 0 ; i < m ; ++i ) c[i] = i*i;
  
  double looksShared = 0.;

  #pragma omp parallel for private(looksShared)
  for ( int i = 0 ; i < m ; ++i )
    {
      looksShared += omp_get_thread_num() + 1;
      printVal("( 7)", "looksShared",looksShared);
    }

  cout << "( 7) looksShared = " << looksShared << endl;




  
  cout << "\n\n\n( 8) ------ firstprivate directive --------------------\n\n";

  int FPvar = 100;

  omp_set_num_threads(4);
  #pragma omp parallel for firstprivate(FPvar)

  for ( int i = 0 ; i < 12 ; ++i )
    {
      FPvar += omp_get_thread_num() + 1;
      printVal("( 8)", "FPvar",FPvar);
    }

  cout << "( 8) FPvar = " << FPvar << endl;
  

  cout << "\n\n\n( 9) ------ lastprivate directive --------------------\n\n";

  int LPvar = 100;

  omp_set_num_threads(4);
  #pragma omp parallel for lastprivate(LPvar)

  for ( int i = 0 ; i < 12 ; ++i )
    {
      printVal("( 9)", "LPvar",LPvar);
      LPvar = omp_get_thread_num() + 1;
    }

  cout << "( 9) LPvar = " << LPvar << endl;
  





  cout << "\n\n\n(10) ------ Processing in specified chunks -----------\n\n";

  omp_set_num_threads(8);
  omp_set_schedule(omp_sched_static, 10);
  
  #pragma omp parallel for schedule(runtime)
    
  for(int i = 0; i < 20; i++)
    {
      printVal("(10)", "handling i ",i);
    }
 





  cout << "\n\n\n(11) ------ Processing in auto chunks ----------------\n\n";

  omp_set_num_threads(3);
  
  #pragma omp parallel for schedule(guided)
    
  for(int i = 0; i < 20; i++)
    {
      printVal("(11)", "handling i ",i);
    }
 
  return 0;
  
}






  // cout << "\n ( 5.3) ------ NUMA effects: First touch done poorly ----\n\n";

  // int *b1 ; b1 = new int [n];
  // int *c1 ; c1 = new int [n];
  // int *d1 ; d1 = new int [n];

  // timingInfo tNUMA_1("NUMA_1");

  // for ( int i = 0 ; i < n ; ++i ) { b1[i] = 0; c1[i] = 111; d1[i] = 222; }
  
  // tNUMA_1.Start(0);

  // #pragma omp parallel for
  // for ( int i = 0 ; i < n ; ++i ) b1[i] = c1[i] + d1[i];

  // tNUMA_1.Finish(0);

  // cout << "\n ( 5.4) ------ NUMA effects: First touch done well ------\n\n";

  // int *b2 ; b2 = new int [n];
  // int *c2 ; c2 = new int [n];
  // int *d2 ; d2 = new int [n];

  // timingInfo tNUMA_2("NUMA_2");

  // #pragma omp parallel for
  // for ( int i = 0 ; i < n ; ++i ) { b2[i] = 0; c2[i] = 111; d2[i] = 222; }
  
  // tNUMA_2.Start(0);

  // #pragma omp parallel for
  // for ( int i = 0 ; i < n ; ++i ) b2[i] = c2[i] + d2[i];

  // tNUMA_2.Finish(0);

