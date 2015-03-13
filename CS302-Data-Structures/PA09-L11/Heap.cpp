/**
 * @file Heap.cpp
 * @author CatherinePollock
 * @date 11/5/14
 * 
 * This is the implementation file for the Heap.h file. 
*/

#include <stdexcept>
#include <iostream>
#include <cmath>
#include "Heap.h"

using namespace std;

//--------------------------------------------------------------------

/**
 * default constructor
 *
 * Creates an empty hash table of size maxNumber with data type DataType.
 * @param maxNumber int of table size
*/
template <typename DataType, typename KeyType, typename Comparator>
Heap< DataType, KeyType, Comparator>::Heap ( int maxNumber )
{
   /// initialize variables/data
   size = 0;
   maxSize = maxNumber;
   dataItems = new DataType[ maxSize ];
}

/**
 * copy constructor
 *
 * Initializes the heap to be equivalent to the other
 * heap parameter.
 * @param other reference to a heap to be copied from
*/
template <typename DataType, typename KeyType, typename Comparator>
Heap< DataType, KeyType, Comparator>::Heap ( const Heap& other )
{
   /// call operator =
   *this = other;
}

/**
 * assignment operator
 *
 * Sets the heap to be equivalent to the other heap 
 * parameter and returns a reference to this object.
 * @param other Heap reference to a heap to be copied from
 * @return Heap& reference to this heap
*/
template <typename DataType, typename KeyType, typename Comparator>
Heap< DataType, KeyType, Comparator>& Heap< DataType, KeyType, Comparator>::operator=
  ( const Heap& other )
{
   /// if the address of other is not equal to this
   if( this != &other )
   {
      /// clear this
      clear();
      
      /// initialize variables/data
      int i;
      size = other.size;
      maxSize = other.maxSize;
      dataItems = new DataType[ maxSize ];  
      
      /// loop through each value and copy data from other
      for( i = 0; i < size; i++ )
      {
         dataItems[ i ] = other.dataItems[ i ];
      }
   }
   /// return dereferenced this
   return *this; 
}

/**
 * destructor
 *
 * Dellocates (frees) the memory used to store the heap. 
*/
template <typename DataType, typename KeyType, typename Comparator>
Heap< DataType, KeyType, Comparator>::~Heap ()
{
   /// calls clear to delete data in heap
   clear();
}

/**
 * insert
 *
 * Inserts newDataItem into the heap. Inserts this data item as the
 * bottom right most data item in the heap and moves it upward until
 * the properties that define a heap are restored. 
 * @param newDataItem reference to the data to be inserted 
 * @pre heap is not full
 * @exception logic_error if heap is full
*/
template <typename DataType, typename KeyType, typename Comparator>
void Heap< DataType, KeyType, Comparator>::insert
  ( const DataType &newDataItem ) throw ( logic_error )
{
   /// initialize variables
   int curIndex = size;
   DataType temp;
   
   /// if the heap is full, throw exception
   if( isFull() )
   {
      throw logic_error( "Insert when heap is full" );
   }
   
   /// set the last item's data to passed data
   dataItems[ curIndex ] = newDataItem;
   
   /// incriment size of heap
   size++;
   
   /// until the children are no longer larger than parent
   while( comparator( dataItems[ curIndex ].getPriority(), dataItems[ (curIndex - 1)/2 ].getPriority()) )
   {
      /// set temp's data
      temp = dataItems[ (curIndex - 1)/2 ];
      
      /// set parent to current child's data
      dataItems[ (curIndex - 1)/2 ] = dataItems[ curIndex ];
      
      /// set child to parent's (temp's) data
      dataItems[ curIndex ] = temp;
      
      /// set a new index (parent) and repeat
      curIndex = (curIndex - 1)/2;
   }
}

/**
 * remove
 *
 * Removes the data item with the highest priority ( the root ) from
 * the heap and returns it. Replaces the root data item with the bottom
 * rightmost data item and moves this data item downward until the properties
 * that define a heap are restored. 
 * @pre heap is not empty
 * @exception logic_error if heap is empty
 * @return DataType containing data removed from root value
*/
template <typename DataType, typename KeyType, typename Comparator>
DataType Heap< DataType, KeyType, Comparator>::remove () throw ( logic_error )
{
   /// throw logic error if empty heap
   DataType tempData;
   if( isEmpty() )
   {
      throw logic_error( "Remove when heap is empty" );
      return tempData;
   }
   
   /// initialize variables
   int curIndex = 0;
   DataType returnData;
   int largestIndex = curIndex;
   
   /// decrement heap size
   size--;
   
   /// set data to be returned
   returnData = dataItems[ curIndex ];
   
   /// set root to have bottom rightmost value
   dataItems[ curIndex ] = dataItems[ size ];
   
   /// while parent index is greater than total number of data items
   while( curIndex < size )
   {
      /// if two children to check
      if( (curIndex*2) + 2 < size )
      {
         /// test if left larger and set largestIndex
         if( comparator( dataItems[ (curIndex*2) + 1 ].getPriority(), dataItems[ largestIndex ].getPriority() ) )
         {
            largestIndex = (curIndex*2) + 1;
         }
         /// test if right larger and set largestIndex
         if( comparator( dataItems[ (curIndex*2) + 2 ].getPriority(), dataItems[ largestIndex ].getPriority() ) )
         {
            largestIndex = (curIndex*2) + 2;
         }
         
         /// if a child was greater than parent
         if( largestIndex != curIndex )
         {
            /// swap data
            tempData = dataItems[ curIndex ];
            dataItems[ curIndex ] = dataItems[ largestIndex ];
            dataItems[ largestIndex ] = tempData;
            
            /// change parent index to check
            curIndex = largestIndex;
         }
         /// if parent was largest
         else 
         {
            /// return stored data
            //cout << "!!!!!!!!!!!!" << endl;
            return returnData;
         }
      }
      /// if only left child to check
      else if( (curIndex*2) + 1 < size )
      {
         /// test if left larger
         if( comparator( dataItems[ (curIndex*2) + 1 ].getPriority(), dataItems[ largestIndex ].getPriority() ) )
         {
            /// swap data
            largestIndex = (curIndex*2) + 1;
            tempData = dataItems[ curIndex ];
            dataItems[ curIndex ] = dataItems[ largestIndex ];
            dataItems[ largestIndex ] = tempData;
            
            /// change parent index to check
            curIndex = largestIndex;
         }
         /// if parent was largest
         else 
         {
            /// return stored data
            return returnData;
         }
      }
      /// if no children to check
      else
      {
         /// return stored data
         return returnData;
      }
   }
   /// return stored data
   return returnData;
}

/**
 * clear
 *
 * Removes all data items in the heap.
*/
template <typename DataType, typename KeyType, typename Comparator>
void Heap< DataType, KeyType, Comparator>::clear ()
{
   /// while heap still has data
   while( !isEmpty() )
   {
      /// remove root value
      remove();
   }
}

/**
 * isEmpty
 *
 * Returns true if the heap is empty. Otherwise, returns false.
 * @return bool if heap is empty or not
*/
template <typename DataType, typename KeyType, typename Comparator>
bool Heap< DataType, KeyType, Comparator>::isEmpty() const
{
   /// return if heap is empty
   return( size == 0 );
}

/**
 * isFull
 *
 * Returns true if the heap is full. Otherwise, returns false.
 * @return bool if heap is full or not
*/
template <typename DataType, typename KeyType, typename Comparator>
bool Heap< DataType, KeyType, Comparator>::isFull() const
{
   /// return if heap is full
   return( size == maxSize );
}

/**
 * showStructure
 *
 * Outputs the priorities of the data items in a heap in both array
 * and tree form. If the heap is empty, outputs "Empty heap". This
 * operation is intended for testing/debugging purposes only.
*/
template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: showStructure () const
{
    int j;   /// Loop counter

    cout << endl;
    if ( size == 0 )
       cout << "Empty heap" << endl;
    else
    {
       cout << "size = " << size << endl;       /// Output array form
       for ( j = 0 ; j < maxSize ; j++ )
           cout << j << "\t";
       cout << endl;
       for ( j = 0 ; j < size ; j++ )
           cout << dataItems[j].getPriority() << "\t";
       cout << endl << endl;
       showSubtree(0,0);                        /// Output tree form
    }
}

/**
 * writeLevels
 * 
 * Outputs the data items in a heap in level order, one level per line. Only outputs each data item's priority. If the heap is empty, then outputs "Empty heap".
*/
template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: writeLevels() const
{
   int i;
   /// if heap is empty, print empty heap
   if( isEmpty() )
   {
      cout << "Empty heap" << endl;
      return;
   }
   
   /// otherwise loop though each data member and print
   for( i = 0; i < size; i++ )
   {
      /// print endline if complete level has been printed
      if( i == 1 || i == 3 || i == 7 || i == 15 || i == 31 )
      {
         cout << endl;
      }
      cout << dataItems[i].getPriority() << ' ';
      
   }
   cout << endl;
   
}

/**
 * showSubtree
 *
 * Helper function for the showStructure() function. Outputs the
 * subtree (subheap) whose root is stored in dataItems[index]. Argument
 * level is the level of this dataItems within the tree.
 * @param index (int) current index at
 * @param level (int) current level at
*/
template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: showSubtree ( int index, int level ) const


{
     int j;   // Loop counter

     if ( index < size )
     {
        showSubtree(2*index+2,level+1);        /// Output right subtree
        for ( j = 0 ; j < level ; j++ )        /// Tab over to level
            cout << "\t";
        cout << " " << dataItems[index].getPriority();   /// Output dataItems's priority
        if ( 2*index+2 < size )                /// Output "connector"
           cout << "<";
        else if ( 2*index+1 < size )
           cout << "\\";
        cout << endl;
        showSubtree(2*index+1,level+1);        /// Output left subtree
    }
}
















