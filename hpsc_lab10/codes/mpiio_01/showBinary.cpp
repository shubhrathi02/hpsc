
// From
//
// http://www.cppforschool.com/tutorial/files2.html
//

#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>

using namespace std;

// Write binary file

void writeChars()
{
    ofstream outFile;
    outFile.open("char.dat", ios::binary);

    char OutStr[100] = "Greetings from a binary file";

    outFile.write( OutStr, 100);
    
    outFile.close();
}

// Read from a binary file

void readChars()
{
    ifstream inFile;
    inFile.open("char.dat", ios::binary);

    char InpStr[100];
    
    while(inFile.read(InpStr, 100))
    {
      cout << "InpStr = " << InpStr << endl;
    }        
    
    inFile.close();
}

// Write binary file - float

void writeFloat()
{
    ofstream outFile;
    outFile.open("float.dat", ios::binary);

    float Out = 3.1415;

    outFile.write( (const char *) & Out , sizeof(float) );
    
    outFile.close();
}

// Read from a binary file - float

void readFloat()
{
    ifstream inFile;
    inFile.open("float.dat", ios::binary);

    float Inp;
    
    while(inFile.read( ( char *) & Inp, sizeof(float) ) )
    {
      cout << "Inp (float) = " << Inp << endl;
    }        
    
    inFile.close();
}


// Write binary file - float array

void writeFloatArray()
{
    ofstream outFile;
    outFile.open("floatArray.dat", ios::binary);

    float Out;

    for ( int i = 1 ; i <= 10 ; ++i )
      {
	Out = i * 100.;
	outFile.write( (const char *) & Out , sizeof(float) );
      }
    
    outFile.close();
}

// Read from a binary file - float array

void readFloatArray()
{
    ifstream inFile;
    inFile.open("floatArray.dat", ios::binary);

    float Inp;
    
    while(inFile.read( ( char *) & Inp, sizeof(float) ) )
    {
      cout << "Inp (float) = " << Inp << endl;
    }        
    
    inFile.close();
}




int main()
{
  writeChars()      ;  readChars();
  writeFloat()      ;  readFloat();
  writeFloatArray() ;  readFloatArray();
  return 0;
}

