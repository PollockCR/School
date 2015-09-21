/*
Name: HW3_CS477_Pollock_3.cpp
Author: Catherine Pollock
Problem: HW3 P3
Purpose: Implement in C/C++ an algorithm to rearrange elements of a given array
   of n real numbers so that all its negative elements precede all its positive
   elements. Your algorithm should be both time- and space-efficient. Show the output
   of your algorithm on the input array A = [4 -3 9 8 7 -4 -2 -1 0 6 -5].
*/

// libraries
#include <iostream>
#include <fstream>
using namespace std;

// global constants
const int PIVOT = 0;

// function prototypes
void sortSign( int *arr, int smaller, int larger, int size ); 
void printArr( int *arr, int size );

// main function
int main()
{
   int index, size; // variables
   cout << "How many elements? "; // ask for number of elements
   cin >> size;
   cout << "Input elements: " << endl; 
   int arr[ size ];
   for( index = 0; index < size; index++ ) // collect elements
   {
      cin >> arr[ index ];
   }
   cout << "Original array:   ";
   printArr( arr, size ); // print original array
   sortSign( arr, -1, 0, size ); // search for index of max element
   cout << "Rearranged array: ";
   printArr( arr, size ); // print resulting array
   return 0; // return sucess
}

// function implementation

// search for max element (divide and conquer)
void sortSign( int *arr, int smaller, int larger, int size )
{
   int index = 0, temp;
   while( larger < size )
   {
      if( arr[ larger ] <= PIVOT )
      {
         smaller++;
         temp = arr[ larger ];
         arr[ larger ] = arr[ smaller ];
         arr[ smaller ] = temp;
         larger++;
      }
      else
      {
         larger++;
      }
   }
}

// print the array
void printArr( int *arr, int size )
{
   int index;
   cout << "[ ";
   for( index = 0; index < size; index++ )
   {
      cout << arr[index] << ' ';
   }
   cout << "]" << endl;
}