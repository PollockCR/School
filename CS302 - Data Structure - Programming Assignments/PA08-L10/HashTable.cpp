/**
 * @file HashTable.cpp
 * @author CatherinePollock
 * @date 10/27/14
 * 
 * This is the implementation file for the HashTable.h file. 
*/

#include <stdexcept>
#include <iostream>
#include "HashTable.h"

using namespace std;

//--------------------------------------------------------------------

/**
 * default constructor
 *
 * Creates an empty hash table of size initTableSize.
*/
template < typename DataType, typename KeyType >
HashTable<DataType,KeyType>::HashTable(int initTableSize)
{
   /// set the size of the hash table
   tableSize = initTableSize;
   
   /// create a table of BSTs
   dataTable = new BSTree<DataType,KeyType>[ tableSize ];
}

/**
 * copy constructor
 *
 * Initializes the hash table to be equivalent to the other
 * hash table parameter.
 * @param other reference to a BST to be copied from
*/
template < typename DataType, typename KeyType >
HashTable<DataType,KeyType>::HashTable(const HashTable<DataType,KeyType>& other)
{
   /// set table size
   tableSize = other.tableSize;
   
   /// create table of BSTs
   dataTable = new BSTree<DataType,KeyType>[ tableSize ];
   
   /// call copyTable to copy table
   copyTable( other );
}

/**
 * assignment operator
 *
 * Sets the hash table to be equivalent to the other hash table 
 * parameter and returns a reference to this object.
 * @param other reference to a hash table to be copied from
 * @return HashTable& reference to this hash table
*/
template < typename DataType, typename KeyType >
HashTable<DataType,KeyType>& HashTable<DataType,KeyType>::operator=
  (const HashTable<DataType,KeyType>& other)
{
   /// if the tables are not the same instances
   if( this != &other )
   {
      /// clear values in this table
      clear();
      
      /// set table size
      tableSize = other.tableSize;
   
      /// create table of BSTs
      dataTable = new BSTree<DataType,KeyType>[ tableSize ];
      
      /// call copyTable to copy other's data into this table
      copyTable( other );
   }
   
   /// return reference to this table
   return *this;
}

/**
 * destructor
 *
 * Dellocates (frees) the memory used to store the hash table. 
*/
template < typename DataType, typename KeyType >
HashTable<DataType,KeyType>::~HashTable()
{
   /// calls clear to delete data in table
   clear();
}

/**
 * insert
 *
 * Inserts newDataItem into the appropriate BST. If a data item
 * with the same key as newDataItem already exists in the BST,
 * then updates that data item with newDataItem. Otherwise, it
 * inserts it into the BST.
 * @param newDataItem reference to the data to be inserted 
*/
template < typename DataType, typename KeyType >
void HashTable<DataType,KeyType>::insert(const DataType& newDataItem)
{
   /// get index of tree to insert to 
   int treeIndex = newDataItem.hash( newDataItem.getKey() )%tableSize;
   
   /// insert into appropriate data table
   dataTable[ treeIndex ].insert( newDataItem );
}

/**
 * remove
 *
 * Searches the hash table for the data item with the key deleteKey.
 * If the data item is found, then removes the data item and returns
 * true. Otherwise, returns false. 
 * @param deleteKey a reference to the key to delete
 * @return bool true if data was found and removed, false otherwise
*/
template < typename DataType, typename KeyType >
bool HashTable<DataType,KeyType>::remove(const KeyType& deleteKey)
{
   /// initialize variables
   int treeIndex;
   
   /// for each tree in table
   for( treeIndex = 0; treeIndex < tableSize;  treeIndex++ )
   {
      /// return true if deleteKey found
      if( dataTable[ treeIndex ].remove( deleteKey ) )
      {
         return true;
      }
   }

   /// return false if deleteKey not found
   return false;
}

/**
 * retrieve
 *
 * Searches the hash table for the data item with key searchKey.
 * If the data item is found, then copies the data item to returnItem
 * and returns true. Otherwise, returns false with returnItem undefined.  
 * @param searchKey a reference to the key searching for (const KeyType&)
 * @param returnItem a reference to the data value found (DataType&)
 * @return bool if value was found
*/
template < typename DataType, typename KeyType >
bool HashTable<DataType,KeyType>::retrieve(const KeyType& searchKey, DataType& returnItem) const
{
   /// initialize variables
   int treeIndex;
   
   /// for each tree in table
   for( treeIndex = 0; treeIndex < tableSize;  treeIndex++ )
   {
      /// return true if retrieveKey found
      if( dataTable[ treeIndex ].retrieve( searchKey, returnItem ) )
      {
         return true;
      }
   }

   /// return false if retrieveKey not found
   return false;
}

/**
 * clear
 *
 * Removes all data items in the hash table
*/
template < typename DataType, typename KeyType >
void HashTable<DataType,KeyType>::clear()
{
   /// initialize variables
   int treeIndex;
   
   /// for each tree in table
   for( treeIndex = 0; treeIndex < tableSize;  treeIndex++ )
   {
      /// clear values of that tree
      dataTable[ treeIndex ].clear();
   }
}

/**
 * isEmpty
 *
 * Returns true if the hash table is empty. Otherwise, returns false.
 * @return bool if hash table is empty or not
*/
template < typename DataType, typename KeyType >
bool HashTable<DataType,KeyType>::isEmpty() const
{
   /// initialize variables
   int treeIndex;
   
   /// for each tree in table
   for( treeIndex = 0; treeIndex < tableSize;  treeIndex++ )
   {
      /// if tree is not empty return false
      if( !( dataTable[ treeIndex ].isEmpty() ) )
      {
         return false;
      } 
   }   
   
   /// return true if all trees empty
   return true;
}

/**
 * showStructure
 * 
 * Outputs the trees in the hash table. Outputs "Empty hash table" if empty.
 * This operation is intended for testing and debugging purposes only.
*/
template < typename DataType, typename KeyType >
void HashTable<DataType, KeyType>::showStructure() const
{
   /// for each tree in hash table
   for (int i = 0; i < tableSize; ++i) 
   {
      /// prints which tree in hash table
      cout << i << ": ";
      
      /// uses writeKeys of BST to print each tree
      dataTable[i].writeKeys();
   }
}

/**
 * standardDeviation
 * 
 * @return double of standard deviation
*/
template < typename DataType, typename KeyType >
double HashTable<DataType,KeyType>::standardDeviation() const
{
   
}

/**
 * copyTable
 * 
 * Copies data of one table to the other 
 * @param source (const HashTable&) table to be copied from
*/
template < typename DataType, typename KeyType >
void HashTable<DataType,KeyType>::copyTable(const HashTable& source)
{
   /// initialize variables
   int treeIndex;
   
   /// for each tree in table
   for( treeIndex = 0; treeIndex < tableSize;  treeIndex++ )
   {
      dataTable[ treeIndex ] = source[ treeIndex ];
   }
}














