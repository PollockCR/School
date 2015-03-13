/**
 * @file PriorityQueue.cpp
 * @author CatherinePollock
 * @date 11/5/14
 * 
 * This is the implementation file for the PriorityQueue.h file. 
*/

#include <stdexcept>
#include <iostream>
#include "PriorityQueue.h"

using namespace std;

//--------------------------------------------------------------------

/**
 * default constructor
 *
 * Creates an empty priority queue. Allocates enough memory for a queue
 * containing maxNumber data items.
 * @param maxNumber int of the max queue size
*/
template < typename DataType, typename KeyType, typename Comparator >
PriorityQueue< DataType, KeyType, Comparator >::PriorityQueue ( int maxNumber )
: Heap< DataType, KeyType, Comparator>::Heap( maxNumber )
{ }

/**
 * enqueue
 *
 * Inserts newDataItem into the priority queue.
 * @param newDataItem (const DataType&) of new data to queue
 * @pre queue is not full
 * @exception logic_error thrown if queue is full
*/
template < typename DataType, typename KeyType, typename Comparator >
void PriorityQueue< DataType, KeyType, Comparator >::enqueue ( const DataType& newDataItem )
{
   Heap< DataType, KeyType, Comparator>::insert( newDataItem );
}

/**
 * dequeue
 *
 * Removes the highest priority (front) data item from the priority queue and
 * returns it. 
 * @return DataType with data removed
 * @pre queue is not empty
 * @exception logic_error thrown if queue is empty
*/
template < typename DataType, typename KeyType, typename Comparator >
DataType PriorityQueue< DataType, KeyType, Comparator >::dequeue ()
{
   return Heap< DataType, KeyType, Comparator>::remove();
}
