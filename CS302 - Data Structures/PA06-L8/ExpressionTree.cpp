/**
 * @file ExpressionTree.cpp
 * @author CatherinePollock
 * @date 10/5/14
 * 
 * This is the implementation file for the ExpressionTree.h file. 
*/

#include <stdexcept>
#include <iostream>
#include "ExpressionTree.h"

using namespace std;

//--------------------------------------------------------------------
   
/**
 * constructor
 *
 * Creates an empty expression tree.
*/
template <typename DataType>
ExprTree<DataType>::ExprTree ()
{
   /// set root to null
   root = NULL;
}

/**
 * copy constructor
 *
 * Initializes the expression tree to be equivalent to the other
 * ExprTree object parameter.
 * @param source reference to an expression tree to be copied from
*/
template <typename DataType>
ExprTree<DataType>::ExprTree(const ExprTree& source)
{
   /// set root to null
   root = NULL;
   
   /// use copy helper to set values
   copyHelper( root, source.root );
}

/**
 * assignment operator
 *
 * Sets the expression tree to be equivalent to the other ExprTree 
 * parameter and returns a reference to this object.
 * @param source reference to an expression tree to be copied from
 * @return ExprTree& reference to this expression tree
*/
template <typename DataType>
ExprTree<DataType>& ExprTree<DataType>::operator=(const ExprTree& source)
{
   /// if not same expression trees
   if( this != & source )
   {
      /// clear values
      clear();
      
      /// copy values using copy helper
      copyHelper( root, source.root );
   }
   /// return this expression tree, dereferenced
   return *this;
}

/**
 * destructor
 *
 * Dellocates (frees) the memory used to store the expression tree. 
*/
template <typename DataType>
ExprTree<DataType>::~ExprTree ()
{
   /// clear values
   clear();
}

/**
 * build
 *
 * Reads an arithmetic expression in prefix form from the keyboard
 * and builds the corresponding expression tree.
*/
template <typename DataType>
void ExprTree<DataType>::build ()
{
   /// use build helper to build expression tree
   buildHelper( root );
}

/**
 * expression
 *
 * Outputs the expression corresponding to the value of the tree
 * in fully parenthesized infix form.
*/
template <typename DataType>
void ExprTree<DataType>::expression () const
{
   /// if tree is not empty
   if( root != NULL )
   {
      /// use expression helper to output expression
      expressionHelper( root );
   }
}

/**
 * evaluate
 *
 * Returns the value of the corresponding arithmatic expression.
 * @pre The expression tree cannot be empty
 * @exception logic_error Throws exception if expression tree is empty.
*/
template <typename DataType>
DataType ExprTree<DataType>::evaluate() const throw (logic_error)
{
   /// initialization
   DataType answer;
   
   /// if tree is empty
   if( root == NULL )
   {
      /// throw logic error if empty tree
      throw logic_error( "Empty tree" );
   }
   
   /// if tree is not empty
   else
   {
      /// use evaluate helper to get answer
      answer = evaluateHelper( root );
      
      /// return answer
      return answer;
   }
}

/**
 * clear
 *
 * Removes all of the data items in the expression tree.
*/
template <typename DataType>
void ExprTree<DataType>::clear()
{
   /// if tree is not empty
   if( root != NULL )
   {
      /// use clearHelper to delete all values
      clearHelper( root );
   }
}

/**
 * commute
 *
 * Commutes the operands for every arithmatic operator in the
 * expression tree. 
*/
template <typename DataType>
void ExprTree<DataType>::commute()
{
   /// if tree is not empty
   if( root != NULL )
   {
      /// use commuteHelper to swap values
      commuteHelper( root );
   }
}

/**
 * isEquivalent
 *
 * Compares the expression tree to another expression tree for equivalence.
 * If the two trees are equivalent, then returns true. Otherwise, 
 * returns false.
*/
template <typename DataType>
bool ExprTree<DataType>::isEquivalent(const ExprTree& source) const
{
   // return true if both trees empty
   if( root == NULL && source.root == NULL )
   {
      return true;
   }

   /// return false is only one trees empty
   else if( root == NULL || source.root == NULL )
   {
      return false;
   }
   
   /// otherwise, return bool returned by equivHelper
   else
   {
      return equivHelper( root, source.root );
   }
}

/**
 * showStructure
 *
 * Outputs an expression tree with its brances oriented from left
 * (root) to right (leaves). The tree output is roated counterclockwise
 * ninety degrees from its conventional orientation. If the tree is
 * empty, output "Empty tree".
*/
template <typename DataType>
void ExprTree<DataType>::showStructure () const
{
   /// if tree is empty
   if ( root == NULL )
   {
      /// print tree is empty
      cout << "Empty tree" << endl;
   }
   
   /// if tree has values
   else
   {
      /// use showHelper to print structure
      cout << endl;
      showHelper(root,1);
      cout << endl;
   }
}

/**
 * constructor
 *
 * Creates an expression tree node.
 * @param elem char that holds value to be placed in node
 * @param *leftPtr ExprTreeNode pointer to next node to the left
 * @param *rightPtr ExprTreeNode pointer to next node to the right
*/
template <typename DataType>
ExprTree<DataType>::ExprTreeNode::ExprTreeNode ( char elem, ExprTreeNode *leftPtr, ExprTreeNode *rightPtr )
{
   /// set data members of ExprTreeNode
   dataItem = elem;
   left = leftPtr;
   right = rightPtr;
}

/**
 * buildHelper
 * 
 * Recursive helper for the build() function. Creates ptr node and 
 * subtree whose root node is pointed to by ptr. 
 * @param ptr ExprTreeNode* to current node
*/
template <typename DataType>
void ExprTree<DataType>::buildHelper( ExprTreeNode* &ptr )
{
   /// initialize variables
   char val;
   
   /// read in a value
   cin >> val;
   
   /// if its not an endline
   if( val != '\n' )
   {
      /// build a new node
      ptr = new ExprTreeNode( val, NULL, NULL ); 

      /// if its not a digit
      if( !isdigit( val ) )
      {
         /// build left and right values
         buildHelper( ptr -> left );
         buildHelper( ptr -> right );
      }
   }
   
   /// finish building expression tree
   return;
}

/**
 * evaluateHelper
 * 
 * Recursive helper for the evaluate() function. Calculates the value of
 * subtree whose root node is pointed to by ptr.
 * @param ptr ExprTreeNode* to current node
 * @return DataType answer of arithmatic equation
*/
template <typename DataType>
DataType ExprTree<DataType>::evaluateHelper( ExprTreeNode* ptr ) const
{
   /// initialize
   DataType leftInt, rightInt;
   DataType answer;

   /// if data is a digit
   if( isdigit( ptr -> dataItem ) )
   {
      /// return numberical value of answer
      answer = (ptr -> dataItem) - '0';
      return answer;
   }
   
   /// if data is an operator
   else
   {
      /// recursively call evaluate left and right values
      leftInt = evaluateHelper( ptr -> left );
      rightInt = evaluateHelper( ptr -> right );
      
      /// for each operator, do math
      switch( ptr -> dataItem )
      {
         case '+':
            answer = leftInt + rightInt;
            break;
         case '-':
            answer = leftInt - rightInt;
            break;
         case '*':
            answer = leftInt * rightInt;
            break;
         case '/':
            /// divide by zero saves as int 
            answer = leftInt / rightInt;
            break;
      }
      /// return calculated value
      return answer;
   }
}

/**
 * expressionHelper
 * 
 * Recursive helper for the expression() function. Prints each
 * item in subtree whose root node is pointed to by ptr.
 * @param ptr ExprTreeNode* to current node
*/
template <typename DataType>
void ExprTree<DataType>::expressionHelper( ExprTreeNode* ptr ) const
{
   /// initialize
   DataType leftInt, rightInt;

   /// if data is digit
   if( isdigit( ptr -> dataItem ) )
   {
      /// print digit
      cout << ptr -> dataItem;
   }
   
   /// if data is not a digit
   else
   {      
      /// for each operator
      /// print parentheses and operator and recursive call left and right 
      switch( ptr -> dataItem )
      {
         case '+':
            cout << '(';
            expressionHelper( ptr -> left );
            cout << '+';
            expressionHelper( ptr -> right );
            cout << ')';
            break;
         case '-':
            cout << '(';
            expressionHelper( ptr -> left );
            cout << '-';
            expressionHelper( ptr -> right );
            cout << ')';
            break;
         case '*':
            cout << '(';
            expressionHelper( ptr -> left );
            cout << '*';
            expressionHelper( ptr -> right );
            cout << ')';
            break;
         case '/':
            cout << '(';
            expressionHelper( ptr -> left );
            cout << '/';
            expressionHelper( ptr -> right );
            cout << ')';
            break;
      }
   }
}

/**
 * clearHelper
 * 
 * Recursive helper for the clear() function. Deltes each
 * item in subtree (bottom to top) whose root node is
 * pointed to by ptr. Then deletes ptr (root).
 * @param ptr ExprTreeNode* to current node
*/
template <typename DataType>
void ExprTree<DataType>::clearHelper( ExprTreeNode* &ptr )
{
   /// if data is a digit
   if( isdigit( ptr -> dataItem ) )
   {
      /// delete node
      delete ptr;
      
      /// set to null
      ptr = NULL;
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
 * copyHelper
 * 
 * Recursive helper for the copy constructor function. Calls itself
 * with values of source until all digits placed.
 * @param ptr ExprTreeNode* to current node
 * @param sourcePtr ExprTreeNode* to source's current node
*/
template <typename DataType>
void ExprTree<DataType>::copyHelper( ExprTreeNode* &ptr, ExprTreeNode* sourcePtr )
{
   /// copy value in source node
   ptr = new ExprTreeNode( sourcePtr -> dataItem, NULL, NULL ); 

   /// if its not a digit
   if( !isdigit( sourcePtr -> dataItem ) )
   {
      /// copy left and right values
      copyHelper( ptr -> left, sourcePtr -> left );
      copyHelper( ptr -> right, sourcePtr -> right );
   }
}

/**
 * showHelper
 * 
 * Recursive helper for the showStructure() function. Outputs the
 * subtree whose root node is pointed to by p. Parameter level is the
 * level of this node within the expression tree.
 * @param p ExprTreeNode* to current node
 * @param level int current level of tree
*/
template <typename DataType>
void ExprTree<DataType>::showHelper( ExprTreeNode* p, int level ) const
{
     int j;   /// Loop counter

     if ( p != 0 )
     {
        showHelper(p->right,level+1);        /// Output right subtree
        for ( j = 0 ; j < level ; j++ )   /// Tab over to level
            cout << "\t";
        cout << " " << p->dataItem;        /// Output dataItem
        if ( ( p->left != 0 ) &&          /// Output "connector"
             ( p->right != 0 ) )
           cout << "<";
        else if ( p->right != 0 )
           cout << "/";
        else if ( p->left != 0 )
           cout << "\\";
        cout << endl;
        showHelper(p->left,level+1);         /// Output left subtree
     }
}

/**
 * commuteHelper
 * 
 * Recursive helper for the commute() function. Calls
 * itself to swap each value of subtree until all values swapped.
 * @param p ExprTreeNode* to current node
*/
template <typename DataType>
void ExprTree<DataType>::commuteHelper( ExprTreeNode* ptr )
{
   /// if digit, return
   if( isdigit( ptr -> dataItem ) )
   {
      return;
   }
   
   /// if not digit, swap values and recurse with children
   else
   {
      ExprTreeNode* temp = ptr -> left;
      ptr -> left = ptr -> right;
      ptr -> right = temp;
      
      commuteHelper( ptr -> right );
      commuteHelper( ptr -> left );
   }
}

/**
 * equivHelper
 * 
 * Recursive helper for the isEquivalent() function. Checks if expression tree 
 * is equivalent to expression tree in parameters, including commutative property.
 * @param ptr ExprTreeNode* to current node
 * @param sourcePtr ExprTreeNode* to source's current node
 * @return bool flag noting equivalency in this specific call
*/
template <typename DataType>
bool ExprTree<DataType>::equivHelper( ExprTreeNode* ptr, ExprTreeNode* sourcePtr ) const
{
   /// set flag to false
   bool flag = false;
   
   /// check if equal data
   if( ptr -> dataItem == sourcePtr -> dataItem )
   {
      /// if either is digit
      if( isdigit( ptr -> dataItem ) || isdigit( sourcePtr -> dataItem ) )
      {
         /// return if values the same
         flag = ( ptr -> dataItem == sourcePtr -> dataItem );
      }
      /// if not a digit
      else
      {
         /// call to check children's values and flag if equal
         if( equivHelper( ptr -> left, sourcePtr -> left ) &&
            equivHelper( ptr -> right, sourcePtr -> right ) )
         {
            flag = true;
         }
         
         /// if operator commutable, check children's values commuted
         else if( ptr -> dataItem == '*' || ptr -> dataItem == '+' )
         {
            if( equivHelper( ptr -> left, sourcePtr -> right ) &&
            equivHelper( ptr -> right, sourcePtr -> left ) )
            {
               flag = true;
            }
         }
         
         /// if not same children, flag false
         else 
         {
            flag = false;
         }
      }
   }
   /// return flag value for this call
   return flag;
}











