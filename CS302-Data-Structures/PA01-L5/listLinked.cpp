//--------------------------------------------------------------------
//
//  Laboratory 5                                          ListLinked.h
//
//  Class implementation for the linked implementation of the List ADT
//
//  Author: Catherine Pollock
//
//--------------------------------------------------------------------

#include <stdexcept>
#include <iostream>
#include "ListLinked.h"

using namespace std;

template <typename DataType>
// Function: List(int ignored)
List<DataType>::List(int ignored)
/* Default constructor for a list. Sets the head and cursor to null. */
{
   // Stores null for head and cursor
   head = cursor = '\0';
}

template <typename DataType>
// Function: List(const List& other)
List<DataType>::List(const List& other)
/* List copy constructor. Loops through values in other (source) from the head node until a null is found for the next value and copies them to this list (destination). */
{
   // Initalizes nodes to move through values in lists
   ListNode* tempOther;
   ListNode* tempThis;
   
   // Sets temp to head of given list
   tempOther = other.head;
   
   // Sets head and cursor to null
   head = cursor = '\0';
   
   // Checks if other list has values
   if( !(other.isEmpty()) )
   {
      // Assigns head of new list with other's head data
      tempThis = head = cursor = new ListNode( tempOther -> dataItem, '\0');
   
      // Loops through until other's next value is null
      while( tempOther -> next != '\0' )
      {
         // Assigns data value to this node from the other list
         tempThis -> next = new ListNode( tempOther -> next -> dataItem, '\0' );
         
         // Move to next node in both lists
         tempOther = tempOther -> next;
         tempThis = tempThis -> next;
         
         // Checks if other's node is the cursor
         if( tempOther == other.cursor )
         {
            // Assigns cursor to this new node
            cursor = tempThis;
         }
      }
   }
   
   // Set the temp nodes to null
   tempOther = tempThis = '\0';
}

template <typename DataType>
// Function: Overloaded assignment operator
List<DataType>& List<DataType>::operator=(const List& other)
/* First checks if the lists are the same lists, and if they are, no changes are made. If they are not, the desination list (this) is cleared and data is assigned to it from the other (source) list. */
{
   // Create temp nodes
   ListNode* tempOther;
   ListNode* tempThis;
   
   // Checks if they are the same list
   if( cursor != other.cursor )
   {
      // Clears destination list
      clear();

      // Sets starting point for source list
      tempOther = other.head;
   
      // Checks if other list has values
      if( !(other.isEmpty()) )
      {
         // Assigns head of new list with other's head data
         tempThis = head = cursor = new ListNode( tempOther -> dataItem, '\0');
         tempOther = tempOther -> next;
         tempThis = tempThis -> next;
   
         // Loops through until other's next value is null
         while( tempOther != '\0' )
         {
            // Assigns data value to this node from the other list
            tempThis = new ListNode( tempOther -> dataItem, '\0' );
            
            // Checks if other's node is the cursor
            if( tempOther == other.cursor )
            {
               // Assigns cursor to this new node
               cursor = tempThis;
            }
            
            // Move to next node in both lists
            tempOther = tempOther -> next;
            tempThis = tempThis -> next;
         }
      }
   }
   return *this;
}

template <typename DataType>
// Function: ~List()
List<DataType>::~List()
/* Goes to the beginning of the list and calls remove to delete nodes until the list is empty. */
{   
   // Goes to beginning of list
   gotoBeginning();

   // Checks if list has values
   while( !isEmpty() )
   { 
      // Deletes values 
      remove();
   }
}

template <typename DataType>
// Function: insert(const DataType& newDataItem)
bool List<DataType>::insert(const DataType& newDataItem)
/* Inserts a value at the current cursor. If the list is full, false is returned. If the list is empty, the first node is made. Otherwise, a node is created and the original cursor's next is pointed to the new node, the new node points to cursor's original next, and the cursor is moved to the new node. */
{
   // Creates a temporary node
   ListNode* temp;
   
   // Checks if list is full
   if( !isFull() )
   {
      // Checks if list is empty
      if( isEmpty() )
      {
         // Creates node at beginning
         head = cursor = new ListNode( newDataItem, '\0' );
      }
      
      // But if list has values
      else
      {
         // Assign temp the value with cursor's next value
         temp = new ListNode( newDataItem, cursor -> next );
         
         // Assign cursor's next value to the new node (temp)
         cursor -> next = temp;
         
         // Set the cursor to the new value
         cursor = temp;
         
         // Set the temporary node to null
         temp = '\0';
      }

      // returns true if the node was created
      return true;
   }
   
   // returns false if node was not created 
   return false;
}

template <typename DataType>
// Function: remove()
bool List<DataType>::remove()
/* Fails if the list is empty. If the list only has one value, the cursor is deleted and head and cursor are set to null. If the cursor is at the end, a temp node is created and the cursor's previous value's next is set to null. The temp node (at the original cursor) is deleted and the new cursor is set to the beginning (head) value. If the cursor was at the beginning, the head is deleted and moved to the next value. Otherwise, the cursor must be in between values and is deleted. Its previous node's next is instructed to navigate to the original cursor's next value. */
{
   // Initialize temp node
   ListNode* tempCursor;
   ListNode* tempNext;

   // Checks if empty
   if( isEmpty() )
   {
      // Returns false if empty 
      return false;
   }
   
   // Checks if last value left in list
   if( cursor == head && cursor -> next == '\0' )
   {
      // Delete the last value
      delete cursor;

      // Set the head and cursor to null
      cursor = head = '\0';

      // Return success
      return true;
   }
   
   // Checks if cursor is at the end
   if( cursor -> next == '\0' )
   {
      ListNode* temp = cursor;
      gotoPrior();
      cursor -> next = '\0';
      // If at end delete the node at cursor
      delete temp;

      // And set the cursor to the head
      cursor = head;       
    
      // Return success
      return true;
   }
   
   // Checks if cursor is at beginning 
   if( cursor == head )
   { 
      // Moves cursor to next value;
      gotoNext();
      
      // Deletes the value at head
      delete head;
      
      // Moves the new head to cursor postition
      head = cursor;
      
      // Return success
      return true;
   }
   
   // For if cursor is in between other values
   
   // Assigns cursor's next value to a temp pointer
   tempNext = cursor -> next;
   
   // Goes to previous node to make a new cursor
   gotoPrior();
   
   // Get new position for cursor
   tempCursor = cursor;
   
   // Go back to original cursor to be removed
   gotoNext();
   
   // Deletes the cursor
   delete cursor;
   
   // Gives new value to cursor 
   tempCursor -> next = cursor = tempNext;
   
   // Sets temp pointers to null
   tempCursor = tempNext = '\0';
   
   // Return success
   return true;
}

template <typename DataType>
// Function: replace(const DataType& newDataItem)
bool List<DataType>::replace(const DataType& newDataItem)
/* If the list is empty, replacement fails. Otherwise, the data given in parameters (newDataItem) is assined to the cursor's data item. */
{
   // Checks if list is empty
   if( isEmpty() )
   {
      // Return false if list is empty
      return false;
   }
   
   // Swap data items
   cursor -> dataItem = newDataItem;
   
   // Return success
   return true;
}

template <typename DataType>
// Function: clear()
void List<DataType>::clear()
/* Loops through values and deletes them until the list is empty. Then, the cursor and head are set back to null. */
{
   // Set cursor to beginning
   cursor = head;
   
   // Loop through until there are no more values
   while( !isEmpty() )
   {
      // Call remove function to delete value
      remove();
   }
   
   // Set values back to null
   cursor = head = '\0';
}

template <typename DataType>
// Function: isEmpty()
bool List<DataType>::isEmpty() const
/* Returns whether or not the head of the list equals null, which signifies whether or not a node has been created for the list. */
{
   // Return whether or not list is empty
   return( head == '\0' );
}

template <typename DataType>
// Function: isFull()
bool List<DataType>::isFull() const
/* Checks if the list is full. Since our list is simple, this will always return false. */
{
   // Return false because this is a simple linked list
   return false;
}

template <typename DataType>
// Function: gotoBeginning()
bool List<DataType>::gotoBeginning()
/* Fails if the list is empty or already at beginning. Else, it assigns the current cursor to the head node. */ 
{
   // Checks if list is empty
   if( isEmpty() || cursor == head )
   {
      // If the list is empty, return false
      return false;
   }
   else 
   {
      // If the list has value(s), change the cursor to beginning
      cursor = head;
   }
   // Return that the list had value(s)
   return true;
}

template <typename DataType>
// Function: gotoEnd()
bool List<DataType>::gotoEnd()
/* Checks if the cursor is already at end or if it is empty. If it's not, the cursor is moved to next value until the next value is found to equal null. */
{
   // Checks if the list is empty
   if( isEmpty() || cursor -> next == '\0' )
   {
      // Returns false if list is empty
      return false;
   }
   // Loop until null is found
   while( cursor -> next != '\0' )
   {
      // Go to the next data value
      gotoNext();
   }
   // Return true for end of list
   return true;
}

template <typename DataType>
// Function: gotoNext()
bool List<DataType>::gotoNext()
/* Checks if the list is empty or at the end. If it's not, the cursor is moved to the next value in the list. */
{
   // Checks if list is empty or at end
   if( isEmpty() || cursor -> next == '\0' )
   {
      // If list is empty or at end, returns false
      return false;
   }
   // Moves the cursor to the next node in list
   cursor = cursor -> next;
   
   // Return true because cursor moved
   return true;
}

template <typename DataType>
// Function: gotoPrior()
bool List<DataType>::gotoPrior()
/* Checks if list is empty or at beginning. If it isn't, a temporary node was created and moves through the list until the next element is the cursor. Then, the temp value is assigned to cursor. */
{
   // Checks if list is empty or at beginning
   if( isEmpty() || head == cursor )
   {
      // Returns false if empty
      return false;
   }
   
   // Creates a temporary node pointer to head
   ListNode* temp = head;
   
   // Loops through until cursor is found
   while( temp -> next != cursor )
   {
      // Goes to next data node
      temp = temp -> next;
   }
   
   // Sets the cursor equal to the temp value
   cursor = temp;
   
   // Sets temp to null
   temp = '\0';
   
   // Returns true because cursor went to prior
   return true;
}

template <typename DataType>
// Function: getCursor()
DataType List<DataType>::getCursor() const
/* Gets the value of the current cursor and returns it */
{
   // Checks if list is empty
   if( isEmpty() )
   {
      // Returns negative value if list is empty
      return -1;
   }
   // Return the data item at cursor
   return cursor -> dataItem;
}

template <typename DataType>
// Function: showStructure
void List<DataType>::showStructure() const
/* Outputs the items in a list. If the list is empty, outputs
"Empty list". This operation is intended for testing and
debugging purposes only. */
{
    if ( isEmpty() )
    {
       cout << "Empty list" << endl;
    } 
    else
    {
	for (ListNode* temp = head; temp != 0; temp = temp->next) {
	    if (temp == cursor) {
		cout << "[";
	    }

	    // Assumes that dataItem can be printed via << because
	    // is is either primitive or operator<< is overloaded.
	    cout << temp->dataItem;	

	    if (temp == cursor) {
		cout << "]";
	    }
	    cout << " ";
	}
	cout << endl;
    }
}

template <typename DataType>
// Function: ListNode(const DataType& nodeData, ListNode* nextPtr)
List<DataType>::ListNode::ListNode(const DataType& nodeData, ListNode* nextPtr)
/* Creates an instance of a list node by assigning the data and next values */
{
   // Set the item's data value
   dataItem = nodeData;
   
   // Set the list's next address
   next = nextPtr;
}

template <typename DataType>
// Function: moveToBeginning()
bool List<DataType>::moveToBeginning ()
/* Moves the data item at cursor to the beginning of the list and moves cursor to beginning of the list as well, as long as list is not empty or already at beginning. */
{
   // Checks if list is empty or already at beginning 
   if( isEmpty() || cursor == head )
   {
      // Returns failure because list is empty
      return false;
   }
   
   // Saves cursor's data
   DataType tempData = cursor -> dataItem;
   
   // Removes data at cursor 
   remove();
   
   // Moves to beginning
   gotoBeginning();
   
   // Adds a node at beginning 
   head = new ListNode( tempData, cursor );
   
   // Makes the head the new cursor
   cursor = head;  
   
   // Returns success
   return true;
}

template <typename DataType>
// Function: insertBefore(const DataType& newDataItem)
bool List<DataType>::insertBefore(const DataType& newDataItem)
/* Inserts the data before the current cursor as a new node.  */
{
   // Checks if list is full
   if( isFull() )
   {
      return false;
   }
   // Checks if list is empty
   if( isEmpty() )
   {
      // If list is empty, new node is inserted
      insert( newDataItem );
      
      // Return success
      return true;
   }
   // Checks if cursor is at beginning
   if( cursor == head )
   {
      // If cursor is at head set new head to a new node 
      head = new ListNode( newDataItem, head );
      
      // Set cursor equal to new head
      cursor = head;
      
      // Return success
      return true;
   }
   // Otherwise, go to previous
   gotoPrior();
   
   // Insert data at new cursor
   insert( newDataItem );
   
   // Return success
   return true;
}













