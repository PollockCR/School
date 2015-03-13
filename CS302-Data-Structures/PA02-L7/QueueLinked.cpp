/**
 * @file QueueLinked.cpp
 * @author CatherinePollock
 * @date 9/8/14
 * 
 * This is the implementation file for the QueueLinked.h file. 
*/

#include <stdexcept>
#include <iostream>
#include "QueueLinked.h"

using namespace std;

//--------------------------------------------------------------------

/** 
 * Default constructor
 * 
 * Creates an empty queue. Sets front and back to null. 
 * @param given a max number of data items allowed in queue, which is
 * ignored (of type int)
*/
template <typename DataType>
QueueLinked<DataType>::QueueLinked(int maxNumber)
{
   // Stores null for front and back
   front = back = NULL;
}

/** 
 * Copy constructor
 * 
 * Initiates an QueueLinked object to be equivalent to the other
 * Queue object parameter. It clears all values, sets tempOther
 * to other's front, and makes a new node
 * for this queue with tempOther's data and null for next. Then, it
 * loops through until tempOther's next value is null and continues
 * to set data values, copied from other. 
 * @param const QueueLinked other (source for copying values)
*/
template <typename DataType>
QueueLinked<DataType>::QueueLinked(const QueueLinked& other)
{
   /// Stores null for front and back
   front = back = NULL;
   
   if( !other.isEmpty() )
   {
      /// Declares temp values to hold
      QueueNode* tempOther;
      
      /// sets tempOther to other's front
      tempOther = other.front;
      
      /// sets front value for new queue
      front = back = new QueueNode( tempOther->dataItem, NULL );
      
      /// loops through other's values
      while(tempOther->next != NULL)
      {
         /// makes a node after back
         back->next = new QueueNode( tempOther->next->dataItem, NULL);
         
         ///sets back to new node
         back = back->next;
         
         /// advances tempOther
         tempOther = tempOther->next;
      }
   }
}

/** 
 * Overloaded assignment operator
 * 
 * Sets the queue to be eqivalent to the other Queue object parameter
 * and returns a reference to the modified queue. If other and this are
 * the same queue, the function does not copy. Otherwise, it clears
 * all values, sets tempOther to other's front, and makes a new node
 * for this queue with tempOther's data and null for next. Then, it
 * loops through until tempOther's next value is null and continues
 * to set data values, copied from other. 
 * @param const QueueLinked other (source for copying values)
*/
template <typename DataType>
QueueLinked<DataType>& QueueLinked<DataType>::operator =(const QueueLinked& other)
{
   /// checks if they are not the same queue
   if( front != other.front )
   {
      /// Clears all values from queue
      clear();
      
      /// Declares temp values to hold
      QueueNode* tempOther;
      
      /// sets tempOther to other's front
      tempOther = other.front;
      
      /// sets front value for new queue
      front = back = new QueueNode( tempOther->dataItem, NULL );
      
      /// loops through other's values
      while(tempOther->next != NULL)
      {
         back->next = new QueueNode( tempOther->next->dataItem, NULL);
         back = back->next;
         tempOther = tempOther->next;
      }
   }
   
   /// returns this queue
   return *this;
}

/** 
 * Destructor
 * 
 * Deallocates (frees) the memory used to store the queue. Starts from the front
 * and deletes nodes until the queue is empty.
*/
template <typename DataType>
QueueLinked<DataType>::~QueueLinked()
{
   /// while queue has values, dequeue the values
   while( !isEmpty() )
   {
      dequeue();
   }
   
   /// set pointers to null
   front = back = NULL;
}

/** 
 * Enqueue (insert)
 * 
 * Inserts newDataItem at  the rear of the queue. Throws an exception
 * if queue is full. It sets temp to a new QueueNode with the data newDataItem
 * and the next value as null. If the queue is empty, front and back are set
 * to temp. Otherwise, back's next value is set to temp and back is 
 * advanced to the new node.
 * @param const DataType& newDataItem (value given to new node of data)
 * @pre Queue is not full
 * @exception If queue is full
 * @return void
*/
template <typename DataType>
void QueueLinked<DataType>::enqueue(const DataType& newDataItem) throw (logic_error)
{
   /// If queue is full, throw exception
   if( isFull() )
   {
      throw logic_error("enqueue() while queue full");
   }
   else
   {
     /// sets temp as node to be added 
     QueueNode* temp;
     temp = new QueueNode( newDataItem, NULL );
     
     /// checks if queue is empty
     if( isEmpty() )
     {
        /// sets first value to new node
        front = back = temp; 
     }
     else 
     {
        /// sets value after the last
        back->next = temp; 
        
        /// advances back
        back = back->next; 
     }
   }
}

/** 
 * Dequeue (remove)
 * 
 * Removes the least recently added (front) data item from the queue and returns it. 
 * Throws an exception if the queue is empty. Gets the data from front. If front
 * is the only value in queue, it deletes the front node and sets front and 
 * back to null. Else, a temp QueueNode is declared and set to node after front.
 * Then, front is deleted and temp is made the new front. Lastly, the data
 * is returned that was stored in the original front.
 * @pre Queue is not empty
 * @exception If queue is empty
 * @return DataType removed from end of queue
*/
template <typename DataType>
DataType QueueLinked<DataType>::dequeue() throw(logic_error)
{
   /// If queue is empty, throw exception
   if( isEmpty() )
   {
      throw logic_error("dequeue() while queue empty");
   }
   else 
   {
      /// gets value of front node
      DataType removedData = front->dataItem;
      
      /// if value is only one in queue
      if( front == back )
      {
         /// deletes front node and sets front and back to null 
         /// since it is now empty
         delete front;
         front = back = NULL;
      }
      else
      {
         /// delcares temp node to hold front's next value
         QueueNode* temp;
         /// sets temp to value after head
         temp = front->next; 
         
         /// deletes value at head
         delete front; 
         
         /// makes front original front's next (temp's) node 
         front = temp;         
      }
      
      /// return data from front
      return removedData; 
   }
}

/** 
 * Clears the queue
 * 
 * Clears the queue of all values. While the queue has values, dequeue 
 * them. Then set front and back to null. 
 * @return void
*/
template <typename DataType>
void QueueLinked<DataType>::clear()
{
   /// while queue has values, dequeue the values
   while( !isEmpty() )
   {
      dequeue();
   }
   
   /// set pointers to null
   front = back = NULL;
}

/** 
 * Checks if queue is empty
 * 
 * Checks if the queue is empty. If it is, return true.
 * If it has values in it, return false.
 * @return bool (that states if queue is empty)
*/
template <typename DataType>
bool QueueLinked<DataType>::isEmpty() const
{
   /// Returns if the front equals null 
   return( front == NULL );
}

/** 
 * Checks if queue is full
 * 
 * Checks if the queue is full. If it is (which will not occur), true
 * is returned. If it is not full, false is returned.
 * @return bool (that states if queue is full)
*/
template <typename DataType>
bool QueueLinked<DataType>::isFull() const
{
   /// Returns false because queue cannot be full
   return false;
}

/** 
 * Puts data item at front of queue
 * 
 * Checks if the queue is full. If it is, throw an exception. If it is not,
 * check if queue is empty. If it is enqueue like normal. Otherwise,
 * set temp to front, and make the new front a node with newDataItem and
 * temp as next. 
 * @param const DataType& newDataItem (data value to put at front of queue)
 * @pre queue is not full
 * @exception if queue is full
 * @return void
*/
template <typename DataType>
void QueueLinked<DataType>::putFront(const DataType& newDataItem) throw (logic_error)
{
   /// throws exception if queue is full
   if( isFull() )
   {
      throw logic_error("putFront() while queue full");
   }
   
   /// checks if queue is empty
   else if( isEmpty() )
   {
      // enqueues item
      enqueue( newDataItem );
   }
   /// otherwise, set temp to front and make a new front with data and temp as next
   else 
   {
      QueueNode* temp;
      temp = front;
      front = new QueueNode( newDataItem, temp );
   }
}

/** 
 * Gets the data item from the end of the queue
 * 
 * Checks if the queue is empty. If it is, throw an exception. Else, save
 * the data at rear. If it is the only value in queue, clear the queue.
 * Otherwise, loop through from front until one before back. Delete 
 * back, set the temp to back and set back's next to null.
 * @pre queue is not empty
 * @exception if queue is empty
 * @return DataType (data value from end)
*/
template <typename DataType>
DataType QueueLinked<DataType>::getRear() throw (logic_error)
{
   /// If empty, throw exception
   if( isEmpty() )
   {
      throw logic_error("getRear() while queue empty");
   }

   else 
   {
      /// set return value
      DataType returnedData = back->dataItem;
      /// If only one value, delete and set to null
      if( front == back )
         {
            clear();
         }
      /// Otherwise, advance temp to node before back, delete back and set 
      /// back to temp
      else 
      {
         QueueNode* temp;
         temp = front;
         while( temp->next != back )
         {
            temp = temp->next;
         }
         delete back;
         back = temp;
         back->next = NULL;
      }
      /// return back's data
      return returnedData;
   }
}

/** 
 * Gets number of values in queue
 * 
 * Sets a counter to zero and a QueueNode temp to front. Advances
 * temp and incriments counter until NULL is reached.
 * @pre queue is not empty
 * @exception if queue is empty
 * @return DataType (data value from end)
*/
template <typename DataType>
int QueueLinked<DataType>::getLength() const
{
   int count = 0;
   QueueNode* temp;
   temp = front;
   while( temp != NULL )
   {
      count++;
      temp = temp->next;
   }
   return count;
}

/** 
 * Outputs the data items in queue
 * 
 * Outputs the elements in a queue. If the queue is empty,
 * outputs "Empty queue". This operation is intended for
 * testing and debugging purposes only.
 * @return void
*/
template <typename DataType>
void QueueLinked<DataType>:: showStructure () const
{
   /// Iterates through the queue
	QueueNode* p;

    if ( isEmpty() )
	cout << "Empty queue" << endl;
    else
    {
	cout << "Front\t";
	for ( p = front ; p != NULL ; p = p->next )
	{
	    if( p == front )
	    {
		cout << '[' << p->dataItem << "] ";
	    }
	    else
	    {
		cout << p->dataItem << " ";
	    }
	}
	cout << "\trear" << endl;
    }
}

/** 
 * Creates a new data item in queue
 * 
 * Creates a new data item in the queue by setting the data value
 * and the next data item's address.
 * @param const DataType& nodeData (data value to be given to node)
 * @param QueueNode* nextPtr (address of next data item)
*/
template <typename DataType>
QueueLinked<DataType>::QueueNode::QueueNode(const DataType& nodeData, QueueNode* nextPtr)
{
   /// Set the item's data value
   dataItem = nodeData;
   
   /// Set the queue's next address
   next = nextPtr;
}





