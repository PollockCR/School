/**
 * @file BSTree.cpp
 * @author CatherinePollock
 * @date 10/16/14
 * 
 * This is the implementation file for the BSTree.h file. 
*/

#include <stdexcept>
#include <iostream>
#include "BSTree.h"

using namespace std;

//--------------------------------------------------------------------

/**
 * default constructor
 *
 * Creates an empty binary search tree.
*/
template < typename DataType, typename KeyType >
BSTree<DataType,KeyType>::BSTree()
{
   /// set root to null
   root = NULL;
}

/**
 * copy constructor
 *
 * Initializes the binary search tree to be equivalent to the other
 * BSTree object parameter.
 * @param other reference to a BST to be copied from
*/
template < typename DataType, typename KeyType >
BSTree<DataType,KeyType>::BSTree( const BSTree<DataType,KeyType>& other )
{
   /// set root to null
   root = NULL;
   
   /// use copy helper to set values
   copyHelper( root, other.root );
}

/**
 * assignment operator
 *
 * Sets the BS tree to be equivalent to the other BSTree 
 * parameter and returns a reference to this object.
 * @param other reference to a BS tree to be copied from
 * @return BSTree& reference to this BS tree
*/
template < typename DataType, typename KeyType >
BSTree<DataType,KeyType>& BSTree<DataType,KeyType>::operator=( const BSTree<DataType,KeyType>& other )
{
   /// if not same expression trees
   if( this != & other )
   {
      /// clear values
      clear();
      
      /// copy values using copy helper
      copyHelper( root, other.root );
   }
   
   /// return this expression tree, dereferenced
   return *this;
}

/**
 * destructor
 *
 * Dellocates (frees) the memory used to store the binary search tree. 
*/
template < typename DataType, typename KeyType >
BSTree<DataType,KeyType>::~BSTree()
{
   /// clear values
   clear();
}

/**
 * insert
 *
 * Calls insertHelper to insert a new data item into BST. Inserts
 * new data item into the BST. If a data item with the sane key as newDataItem
 * already exists in the tree, then updates that data item with newDataItem.
 * @param newDataItem reference to the data to be inserted 
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>::insert( const DataType& newDataItem )
{
   insertHelper( root, newDataItem );
}

/**
 * retrieve
 *
 * Calls retrieveHelper to find the data item passed. Searches the BST for the 
 * data item with key searchKey. If this data item is found, then copies the
 * data item to searchDataItem and returns true. Otherwise, returns false and 
 * searchDataItem undefined. 
 * @param searchKey a reference to the key searching for
 * @param searchDataItem a reference to the data value to find
 * @return bool if value was found
*/
template < typename DataType, typename KeyType >
bool BSTree<DataType,KeyType>::retrieve( const KeyType& searchKey, DataType& searchDataItem ) const
{
   return retrieveHelper( root, searchKey, searchDataItem );
}

/**
 * remove
 *
 * Calls removeHelper to delete the key passed. Deletes the data item with key
 * deleteKey from the binary search tree. If the data item is found, then 
 * deletes it from the tree and returns true. Otherwise, returns false.
 * @param deleteKey a reference to the key to delete
 * @return bool true if data was found and removed, false otherwise
*/
template < typename DataType, typename KeyType >
bool BSTree<DataType,KeyType>::remove( const KeyType& deleteKey )
{
   return removeHelper( root, deleteKey );
}

/**
 * writeKeys
 *
 * Outputs the keys of the data items in the BST. The keys are output 
 * in ascending order on one line, seperated by spaces.
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>::writeKeys() const
{
   writeKeysHelper( root );
   cout << endl;
}

/**
 * clear
 *
 * Removes all data items in the binary search tree
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>::clear()
{
   clearHelper( root );
   root = NULL;
}

/**
 * isEmpty
 *
 * Returns true is the BST is empty. Otherwise, returns false.
 * @return bool if tree is empty or not
*/
template < typename DataType, typename KeyType >
bool BSTree<DataType,KeyType>::isEmpty() const
{
   /// return true if root is null, false otherwise
   return( root == NULL );  
}

/**
 * showStructure
 * 
 * Outputs the keys in a binary search tree. The tree is output
 * rotated counterclockwise 90 degrees from its conventional
 * orientation using a "reverse" inorder traversal. This operation is
 * intended for testing and debugging purposes only.
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>::showStructure() const
{
    if ( root == 0 )
       cout << "Empty tree" << endl;
    else
    {
       cout << endl;
       showHelper(root,1);
       cout << endl;
    }
}

/**
 * getHeight
 * 
 * Returns the height of the binary search tree.
 * @return int height of BST
*/
template < typename DataType, typename KeyType >
int BSTree<DataType,KeyType>::getHeight() const
{
   /// call helper to count height
   return getHeightHelper( root );
}

/**
 * getCount
 * 
 * Returns the count of the number of data items in the binary search tree.
 * @return int count of number of data items in BST
*/
template < typename DataType, typename KeyType >
int BSTree<DataType,KeyType>::getCount() const
{
   /// call helper to count data items
   return getCountHelper( root );
}

/**
 * constructor
 * 
 * Creates a binary search tree node
 * @param nodeDataItem reference to data to save to node
 * @param leftPtr pointer to node to the left
 * @param rightPtr pointer to node to the right 
*/
template < typename DataType, typename KeyType >
BSTree<DataType,KeyType>::BSTreeNode::BSTreeNode( const DataType &nodeDataItem,
  BSTreeNode *leftPtr, BSTreeNode *rightPtr )
{
   /// initialize data members
   dataItem = nodeDataItem;
   left = leftPtr;
   right = rightPtr;
}

/**
 * showHelper
 * 
 * Recursive helper for showStructure. 
 * Outputs the subtree whose root node is pointed to by p. 
 * Parameter level is the level of this node within the tree.
 * @param p pointer to current node
 * @param level int count of number of levels currently
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>:: showHelper( BSTreeNode *p,
  int level) const
{
   /// Loop counter
   int j;   

   if ( p != 0 )
   {
      /// Output right subtree
      showHelper(p->right,level+1);   
      
      /// Tab over to level       
      for ( j = 0 ; j < level ; j++ )    
          cout << "\t";
      cout << " " << p->dataItem.getKey();   /// Output key
      if ( ( p->left != 0 ) &&           /// Output "connector"
           ( p->right != 0 ) )
        cout << "<";
      else if ( p->right != 0 )
         cout << "/";
      else if ( p->left != 0 )
         cout << "\\";
      cout << endl;
      showHelper(p->left,level+1);          /// Output left subtree
    }
}

/**
 * insertHelper
 * 
 * Recursive helper for insert. 
 * Inserts new data item into the BST. If a data item with the sane key as newDataItem
 * already exists in the tree, then updates that data item with newDataItem. Calls itself if
 * data should go to right or left until a null is found.
 * @param ptr BSTreeNode pointer to current node
 * @param newDataItem int value to be inserted
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>::insertHelper( BSTreeNode* &ptr, const DataType& newDataItem )
{
   /// if current tree node is null
   if( ptr == NULL )
   {
      /// insert a new node with given data
      ptr = new BSTreeNode( newDataItem, NULL, NULL );
      return;
   }
   
   /// if data to be inserted is less than current tree node
   if( ptr -> dataItem.getKey() > newDataItem.getKey() )
   {
      /// call insertHelper with node to the right   
      insertHelper( ptr -> left, newDataItem );
   }
   
   /// if data to be inserted is greater than current node
   else
   {
      /// call insertHelper with node to the right
      insertHelper( ptr -> right, newDataItem );
   }
}

/**
 * removeHelper
 * 
 * Recursive helper for remove. 
 * Calls itself to delete the key passed. Deletes the data item with key
 * deleteKey from the binary search tree. If the data item is found, then 
 * deletes it from the tree and returns true. Otherwise, returns false.
 * @param ptr BSTreeNode pointer to current node
 * @param deleteKey int value to be deleted
*/
template < typename DataType, typename KeyType >
bool BSTree<DataType,KeyType>::removeHelper( BSTreeNode* &ptr, const KeyType& deleteKey )
{
   /// if ptr is null
   if( ptr == NULL )
   {
      /// value was not found
      return false;
   }   
   
   /// if value was found
   if( ptr -> dataItem.getKey() == deleteKey )
   {
      /// case 1 - no children
      if( ( ptr -> left == NULL ) && ( ptr -> right == NULL ) )
      {
         /// delete node
         delete ptr;
         
         /// set ptr to null
         ptr = NULL;
         
         /// return that data was deleted
         return true;
      }
      
      /// case 2 - 1 child 
         /// case 2l - left child
         if( ( ptr -> left != NULL ) && ( ptr -> right == NULL ) )
         {
            /// initialize temp node pointer
            BSTreeNode* temp;
            
            /// point temp to ptr
            temp = ptr;
            
            /// point ptr to its left child
            ptr = ptr -> left;
            
            /// delete temp (original ptr)
            delete temp;
            
            /// return that data was deleted
            return true;
         }
         
         /// case 2r - right child
         if( ( ptr -> left == NULL ) && ( ptr -> right != NULL ) )
         {
            /// initialize temp node pointer
            BSTreeNode* temp;
            
            /// point temp to ptr
            temp = ptr;
            
            /// change ptr to its right child
            ptr = ptr -> right;
            
            /// delete temp (original ptr)
            delete temp;
            
            /// return that data was deleted
            return true;
         }         
      
      /// case 3 - 2 children
      else
      {
         /// initialize a temp note pointer
         BSTreeNode* temp;
         
         /// set the temp pointer to ptr
         temp = ptr;
         
         /// point temp to its left child
         temp = temp -> left;
         
         /// until temp equals null
         while( temp -> right != NULL )
         {
            /// point temp to its right child
            temp = temp -> right;
         }
         
         /// set ptr's data to that of temp's
         /// ( change the value of the removed node to that of it's closest child )
         ptr -> dataItem.setKey( temp -> dataItem.getKey() );
         
         /// call removeHelper to repeat on remaining children and return result
         return removeHelper( ptr -> left, temp -> dataItem.getKey() );
      }
    
   }
      
   /// if the ptr's data is greater than the one to delete
   if( ( ptr -> dataItem.getKey() ) > deleteKey )
   {
      /// call removeHelper to test child to left
      return removeHelper( ptr -> left, deleteKey );
   }
   
   /// if the ptr's data is less than the one to delete
   if( ( ptr -> dataItem.getKey() ) < deleteKey )
   {
      /// call removeHelper to test child to right
      return removeHelper( ptr -> right, deleteKey );
   }        
}

/**
 * retrieveHelper
 * 
 * Recursive helper for retrieve. 
 * Calls itself to find the data item passed. Searches the BST for the 
 * data item with key searchKey. If this data item is found, then copies the
 * data item to searchDataItem and returns true. Otherwise, returns false and 
 * searchDataItem undefined. 
 * @param ptr BSTreeNode pointer to current node
 * @param deleteKey int value to be deleted
*/
template < typename DataType, typename KeyType >
bool BSTree<DataType,KeyType>::retrieveHelper( BSTreeNode* ptr, const KeyType& searchKey, DataType& searchDataItem ) const
{
   /// base cases
   
   /// if current node is null 
   if( ptr == NULL )
   {
      /// value was not found, return false
      return false;
   }

   /// if search data item is found
   if( searchKey == ( ptr -> dataItem.getKey() ) )
   {
      /// set search data item, return true
      searchDataItem = ptr -> dataItem;
      return true;
   }
     
   /// recursive calls
   
   /// if search item is less than pointer's
   if( searchKey < ( ptr -> dataItem.getKey() ) )
   {
      /// call self with node to the left
      retrieveHelper( ptr -> left, searchKey, searchDataItem );
   }
   
   /// if search item is greater than pointer's
   else if( searchKey > ( ptr -> dataItem.getKey() ) )
   {
      /// call self with node to the right
      retrieveHelper( ptr -> right, searchKey, searchDataItem );
   }
}

/**
 * clearHelper
 * 
 * Recursive helper for clear. Ends if at null pointer. Else, calls to remove
 * children and deletes itself.
 * Calls itself to remove all data items in the binary search tree 
 * @param ptr BSTreeNode pointer to current node
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>::clearHelper( BSTreeNode* &ptr )
{
   /// if pointer is null
   if( ptr == NULL) 
   {
      return;
   }
   
   /// if data has children
   else 
   {
      /// clear left and right chilren
      clearHelper( ptr -> left );
      clearHelper( ptr -> right );
      
      /// delete root
      delete ptr;
      
      /// set to null
      ptr = NULL;
   }
}

/**
 * writeKeysHelper
 * 
 * Recursive helper for writeKeys.
 * Outputs the keys of the data items in the BST. The keys are output 
 * in ascending order on one line, seperated by spaces.
 * @param ptr BSTreeNode pointer to current node
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>::writeKeysHelper( BSTreeNode* ptr ) const
{
  /// for each node that isn't empty
  if( ptr != NULL )
  {
     /// print nodes to left
     writeKeysHelper( ptr -> left );
     
     /// print this node
     cout << ( ptr -> dataItem.getKey() ) << ' ';
     
     /// print nodes to right
     writeKeysHelper( ptr -> right );
  }
}

/**
 * copyHelper
 * 
 * Sets this BS tree to be equivalent to the other BSTree 
 * parameter by calling itself to copy each node
 * @param ptr BSTreeNode pointer to current node to copy to
 * @param sourcePtr BSTreeNode pointer to source's node to copy from
*/
template < typename DataType, typename KeyType >
void BSTree<DataType,KeyType>::copyHelper( BSTreeNode* &ptr, BSTreeNode* sourcePtr )
{
   /// if node empty, end 
   if( sourcePtr == NULL )
   {
      return;
   }
   
   /// copy value in source node
   ptr = new BSTreeNode( sourcePtr -> dataItem, NULL, NULL ); 
   
   /// copy left and right values
   copyHelper( ptr -> left, sourcePtr -> left );
   copyHelper( ptr -> right, sourcePtr -> right ); 
}

/**
 * getHeightHelper
 * 
 * Returns the height of the BST
 * @param ptr BSTreeNode pointer to current node to copy to
 * @return int height of BST
*/
template < typename DataType, typename KeyType >
int BSTree<DataType,KeyType>::getHeightHelper( BSTreeNode* ptr ) const
{
   /// base case - end of branch  
   if( ptr == NULL )
   {
      return 0;
   }
   
   /// if left branch has greater height than right
   if( getHeightHelper( ptr -> left ) > getHeightHelper( ptr -> right ) )
   {
      /// return 1 (for this node) plus the height of left branch
      return 1 + getHeightHelper( ptr -> left );
   }
   /// otherwise
   else
   {
      /// return 1 (for this node) plus the height of right branch
      return 1 + getHeightHelper( ptr -> right );   
   }

}

/**
 * getCountHelper
 * 
 * Returns the count of number of data items in the BST
 * @param ptr BSTreeNode pointer to current node to copy to
 * @return int count of items in BST
*/
template < typename DataType, typename KeyType >
int BSTree<DataType,KeyType>::getCountHelper( BSTreeNode* ptr ) const
{
   /// base case - end of branch
   if( ptr == NULL )
   {
      return 0;
   }
   else
   {
      /// recursive call - add 1 (this item) plus counts of left and right branches
      return 1 + getCountHelper( ptr -> right ) + getCountHelper( ptr -> left );
   }
}







