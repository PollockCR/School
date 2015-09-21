/*
Name: HW3_CS477_Pollock_1.cpp
Author: Catherine Pollock
Problem: HW3 P1
Purpose: Implement in C/C++ a divide and conquer algorithm for finding
   a position of the largest element in an array of n numbers. Show how
   your algorithm runs on the input A = [1 4 9 3 4 9 5 6 9 3 7].
*/

// libraries
#include <iostream>
#include <fstream>
using namespace std;

// global constants

// function prototypes
void searchMax( int *arr, int beg, int end, int &indexMax ); 

// main function
int main()
{
   int index, size, indexMax = 0; // variables
   cout << "How many elements? "; // ask for number of elements
   cin >> size;
   cout << "Input elements: " << endl; 
   int arr[ size ];
   for( index = 0; index < size; index++ ) // collect elements
   {
      cin >> arr[ index ];
   }
   searchMax( arr, 0, size-1, indexMax ); // search for index of max element
   cout << "Index of max element = " << indexMax << endl; // output results
   return 0; // return sucess
}

// function implementation

// search for max element (divide and conquer)
void searchMax( int *arr, int beg, int end, int &indexMax )
{
   int mid;
   if( beg == end ) // base case - 1 element
   {
      if( arr[beg] > arr[indexMax] )
      {
         indexMax = beg;
      }
   }
   else if( beg == end - 1 ) // compare 2 elements
   {
      if( arr[beg] < arr[end] )
      {
         if( arr[end] > arr[indexMax] ) // compare with current max
         {
            indexMax = end;
         }
      }
      else
      {
         if( arr[beg] > arr[indexMax] ) // compare with current max
         {
            indexMax = beg;
         }
      }
   }
   else // continue to divide
   {
      mid = ( beg + end ) / 2; // divide
      searchMax( arr, beg, mid, indexMax ); // recursive call first half of array
      searchMax( arr, mid+1, end, indexMax ); // recursive call second half of array
   }

}