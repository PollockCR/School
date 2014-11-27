/**
 * @file pumpkin.cpp
 * @author CatherinePollock
 * @date 9/16/14
 * 
 * This is the file that drives reading in, examining, and
 * outputting information about a pumpkin patch.
*/
#include <stdexcept>
#include <iostream>

using namespace std;

const int MAX_SIZE = 40;

//--------------------------------------------------------

/// Function Prototypes
int findP( char garden[][ MAX_SIZE ], int row, int col, int rows, int cols);

/** Main Function
 * 
 * This program counts patches of pumpkins in a garden. First, the program gets a size for rows and columns from the user. Next, the user must enter in these values for their garden. Once they have all been entered, the program uses the function findP to search for patches. The size is given back to main after each patch is found, and then the sizes of the patches for each garden is sorted using a bubble sort. Once they have been sorted, the results are printed to screen. 
 */
int main()
{
   /// variable decleration
   char tempChar;
   int rowNums, colNums = 0;
   int rowIndex, colIndex;
   int gardenCount = 0, size = 0;
   
   // array decleration
   int sizeHolder[ MAX_SIZE ][ 301 ];
   int patchCount[ MAX_SIZE ];
   int sortHolder[ MAX_SIZE ][ 301 ];
   
   // continue repeating following until 0's are given
   do
   {
      /// determine size of garden
      cin >> rowNums;
      cin >> colNums;

      // determine if garden size is too big and reset to appropriate size
      if( rowNums > MAX_SIZE )
      {
         rowNums = MAX_SIZE;
      }
      if( colNums > MAX_SIZE )
      {
         colNums = MAX_SIZE;
      }

      /// array decleration
      char gardenArr[ MAX_SIZE ][ MAX_SIZE ];
      
      /// reset variables
      rowIndex = 0;
      
      /// loop through rows to fill array
      while( rowIndex < rowNums )
      {
         /// loop through columns
         colIndex = 0;
         while( colIndex < colNums )
         {
            /// enter the char into array unless it is a new-line or space
            cin >> tempChar;
            if( tempChar != '\n' && tempChar != ' ' )
            {
               gardenArr[ rowIndex ][ colIndex ] = tempChar;
               colIndex++;
            }
         }
         rowIndex++;
      }
      
      /// reset counter and index values
      gardenCount++;
      patchCount[ gardenCount ] = 0;
      rowIndex = 0;

      
      /// if array has rows and columns loop through
      if( rowNums != 0 && colNums != 0 )
      {         
         while( rowIndex < rowNums )
         {
            colIndex = 0;
            while( colIndex < colNums )
            { 
               /// find all instances of p and save to variable size
               size = findP( gardenArr, rowIndex, colIndex, rowNums, colNums );

               if( size > 0 )
               {
                  sizeHolder[ gardenCount ][ patchCount[ gardenCount ] ] = size;
                  patchCount[ gardenCount ] +=1;
               }
               colIndex++;
            }
            rowIndex++;
         }        
      }
      
      
   } while ( rowNums != 0 && colNums != 0 );

   /// loop through each garden   
   for( int gardenIndex = 1; gardenIndex < gardenCount; gardenIndex++ )
   {
      /// print garden information
      cout << "Garden # " << gardenIndex << ": " << patchCount[ gardenIndex ] << " patches, sizes: "; 
      
      /// sort sizes for each gardem with bubble sort
      for( int i = 0; i < patchCount[ gardenIndex ]; i++ )
      {
         for( int j = 0; j < patchCount[ gardenIndex ] - 1; j++ )
         {
            if( sizeHolder[ gardenIndex ][ j ] > sizeHolder[ gardenIndex ][ j + 1] )
            {
               tempChar = sizeHolder[ gardenIndex ][ j ];
               sizeHolder[ gardenIndex ][ j ] = sizeHolder[ gardenIndex ][ j + 1 ];
               sizeHolder[ gardenIndex ][ j + 1 ] = tempChar;
            }
         }
      }
      
      // print out sorted size values
      for( int i = 0; i < patchCount[ gardenIndex ]; i++ )
      {
       cout << sizeHolder[ gardenIndex ][ i ] << ' ';
      }
      
      // add spacing after printed values
      cout << endl;
   }
   // return success to OS
   return 0;
}

/// Function Implementation
/**
 * findP function
 * 
 * Determines if 'p' is at given location. If it is, the elements around are
 * checked for a 'p' value. If no 'p' is found, the function returns 1, for 
 * one found instance. If one 'p' is found, the function recursively calls
 * itself with the new location of a 'p'. If more than one 'p' is found around
 * the original 'p', the function calls itself with all values surrounding it.
 * With each call of the function and each finding of a 'p', 1 is added to the 
 * total found 'p's, and the value is changed to an uppercase P to flag that 
 * it has been counted. 
 * @param char garden array, used to search through
 * @param int row is given to indicate current row to be checked
 * @param int col is given to indicate current column to be checked
 * @param int rows to indicate max amount of rows
 * @param int cols to indicate max amount of columns
 * @return int size of current pumpkin patch
 */
int findP( char garden[][ MAX_SIZE ], int row, int col, int rows, int cols )
{ 
   /// declare variables and arrays
   int numPs = 0;
   int newRow = row, newCol = col;
   int rowAbove = row, rowBelow = row, colLeft = col, colRight = col;
   
   // get the value at the given location
   char temp = garden[ row ][ col ];
   
   /// check if value at location is 'p'
   if( temp == 'p' )
   {
      /// change the value of 'p' to 'P' to flag that it has been counted.
      garden[ row ][ col ] = 'P';
      
      /// check above for another 'p'
      if( row != 0 )
      {
         if( garden[ row-1 ][ col ] == 'p' )
         {
            numPs++;
            newRow = rowAbove = row-1;
         }
      } 
            
      /// check below for another 'p'
      if( row != rows-1 )
      {
         if( garden[ row+1 ][ col ] == 'p' ) 
         {
            numPs++;
            newRow = rowBelow = row+1;
         }
      } 
            
      /// check to left for another 'p'
      if( col != 0 )
      {
         if( garden[ row ][ col-1 ] == 'p' ) 
         {
            numPs++;
            newCol = colLeft = col-1;
         }
      }            
            
      /// check to right for another 'p'
      if( col != cols-1 )
      {
         if( garden[ row ][ col+1 ] == 'p' ) 
         {
            numPs++;
            newCol = colRight = col+1;
         }
      }
      
      // if no p's were found around p, return size of 1
      if( numPs == 0 )
      {
         return 1;
      }
      /// if one 'p' was found, call this function with new location
      else if( numPs == 1 )
      {
         return 1 + findP( garden, newRow, newCol, rows, cols );
      }
      /// if multiple 'p's were found, call function with all surrounding locations
      else
      {
         return 1 + findP( garden, rowAbove, col, rows, cols ) + findP( garden, rowBelow, col, rows, cols ) + findP( garden, row, colLeft, rows, cols ) + findP( garden, row, colRight, rows, cols );
      }
   }
   
   // return zero if no value of 'p' was found in this location.
   return 0;
}
