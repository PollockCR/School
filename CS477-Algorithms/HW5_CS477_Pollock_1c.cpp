/*
Name: HW5_CS477_Pollock_1b.cpp
Author: Catherine Pollock
Problem: HW5 P1b
Date: 11/2/15
To run:
$ g++ -Wall -std=c++0x HW5_CS477_Pollock_1b.cpp
$ ./a.out
The datafiles must be in the same directory.
*/

// libraries
#include <iostream>
#include <fstream>
#include <string> 
#include <unordered_map>
#include <vector>
#include <iterator>
#include <algorithm> 

using namespace std;

// global constants
const char* FDATA = "fData.txt";
const char* XDATA = "xData.txt";

// variable types
struct solutions
{
   int maxDrones = 0;
   vector<int> firetimes;
   unsigned int prevOpt = 0;
};

// function prototypes
bool readFile( const char *datafile, vector<int> &vals );
void solveSubproblems( unordered_map<int, solutions>  &mVals, vector<vector<int>> &m, vector<int> &fVals, vector<int> xVals );
void printTable( vector<vector<int>> &m, unsigned int size );
void printReconstruct( vector<vector<int>> &m, unsigned int size );


// main function
int main()
{
   // declare variables
   bool fileIsGood;
   vector<int> fVals; // to hold f(j) values
   vector<int> xVals; // to hold x_i values
   unordered_map<int, solutions>  mVals; // to hold optimum values

      // save base case values
      fVals.push_back(0); // f(0) = 0
      xVals.push_back(0); // x_0 = 0

   // read in from files
   fileIsGood = readFile( FDATA, fVals );

      if( !fileIsGood )
      {
         cout << "ERROR: Invalid f(i) data file." << endl;
      }

   fileIsGood = readFile( XDATA, xVals );

      if( !fileIsGood )
      {
         cout << "ERROR: Invalid x_i data file." << endl;
      }   

      if( xVals.size() != fVals.size() )
      {
         cout << "ERROR: Invalid file formats. Ensure valid data." << endl;
      }

   // solve subproblems
   vector<vector<int>> m( fVals.size(), vector<int>(fVals.size(), 0));
   solveSubproblems( mVals, m, fVals, xVals );

   // print optimum solution
   cout << endl << "The maximum number of drones nuetralized after " << mVals.size()-1 << " seconds is ";
   cout << mVals[mVals.size()-1].maxDrones << '.'<< endl << endl;

   // print out table of values
   cout << "Table displaying optimum value for second i with j seconds of charge:" << endl;
   printTable(m, fVals.size());
   cout << endl << endl;

   // print out table of values
   cout << "Table displaying where each subproblem solution came from:" << endl << "(i.e. optimality requires firing at second i, as well as during value in brackets of (i,j), recursively)" << endl;
   printReconstruct(m, fVals.size());

   return 0; // return sucess
}

// function implementation
bool readFile( const char *datafile, vector<int> &vals )
{
   // declare variables
   ifstream ifs;
   int tempVal;

   // open file
   ifs.open( datafile, ifstream::in );

      // check for invalid file
      if( !ifs.good() )
      {
         return false;
      }

   // read in values
   while( ifs.good() )
   {
      ifs >> tempVal;
      vals.push_back(tempVal);
   }

   // return success
   return true;
}

void solveSubproblems( unordered_map<int, solutions>  &mVals, vector<vector<int>> &m, vector<int> &fVals, vector<int> xVals )
{
   // declare variables
   unsigned int i, j;
   vector<int>::iterator maxPtr;

   // base case: mVals[0] = 0;
   mVals[0];

   // loop through
   for( i = 1; i < fVals.size(); i++ )
   {

      m[i][0] = 0;
      for(j = 1; j <= i; j++ )
      {
         m[i][j] = mVals[i-j].maxDrones + min( xVals[i], fVals[j] );
      }

      // find max value for second i
      maxPtr = max_element(m[i].begin(), m[i].end() );
      //cout << *maxPtr << endl;
      j = distance(m[i].begin(), maxPtr );
      // save optimum value
      mVals[i];
      mVals[i].maxDrones = *maxPtr;
      mVals[i].prevOpt = i - j; // Part B
      mVals[i].firetimes = mVals[i-j].firetimes; // Part C
      mVals[i].firetimes.push_back(i); // Part C
   }

}

// PART B
void printTable( vector<vector<int>> &m, unsigned int size )
{
   unsigned int i, j;

   // print axis labels
   cout << "            j values: " << endl << "            ";
   for( unsigned int i = 1 ; i < size; i++ )
   {
      cout << i << ' '; 
   }
   cout << endl << "            ";
   for( unsigned int i = 1 ; i < size; i++ )
   {
      cout << "__"; 
   }
   cout << endl;

   // print subproblem solutions
   for( i = size - 1 ; i > 0; i-- )
   {
      if( i == size/2 )
      {
         cout << "i values: ";
      }
      else
      {
         cout << "          ";
      }
      cout << i << " |";
      for( j = 1; j <= i ; j++ )
      {
         cout << m[i][j] << ' ';
      }
      cout << endl;
   }
}

// PART C
void printReconstruct( vector<vector<int>> &m, unsigned int size )
{
   unsigned int i, j;

   // print axis labels
   cout << "            j values: " << endl << "            ";
   for( unsigned int i = 1 ; i < size; i++ )
   {
      cout << i << "    "; 
   }
   cout << endl << "            ";
   for( unsigned int i = 1 ; i < size; i++ )
   {
      cout << "_____"; 
   }
   cout << endl;

   // print subproblem solutions
   for( i = size - 1 ; i > 0; i-- )
   {
      if( i == size/2 )
      {
         cout << "i values: ";
      }
      else
      {
         cout << "          ";
      }
      cout << i << " |";
      for( j = 1; j <= i ; j++ )
      {
         cout << m[i][j]<< "[" << i-j << "]" << ' ';
      }
      cout << endl;
   }
}