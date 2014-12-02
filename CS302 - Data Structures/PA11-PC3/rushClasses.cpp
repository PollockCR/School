/**
 * @file rushClasses.cpp
 * @author Catherine Pollock
 * @date 11/29/14
 * 
 * This is the file that implements classes.h and
 * the classes within that file.
*/

/// header files
#include <iostream>
#include "rushClasses.h"
using namespace std;

/**
 * Default constructor for board class. 
 * 
 * Default constructor for board class. Sets the data members.
*/
Board::Board()
{
   /// initialize number of cars to zero
   numCars = 0;
}

/**
 * Copy constructor for board class. 
 * 
 * Sets a board up with values given in car list
 * with a given number of cars. Initializes boardCars
 * with other's boardCars values.
 *
 * @param other source of boardCars to copy from
*/
Board::Board( const Board& other )
{
   *this = other;
}

/**
 * Overloaded assignment operator
 * 
 * Sets a board up with values given in car list
 * with a given number of cars. Set boardCars
 * with other's boardCars values.
 *
 * @param other source of boardCars to copy from
 * @return Board for chain of assignments
*/
Board& Board::operator=( const Board& other )
{
   /// initialization
   int i;
   
   /// if they are not the same board
   if( this != &other )
   {
      /// set number of cars
      numCars = other.numCars;
      
      /// loop through to set data values
      for( i = 0; i < numCars; i++ )
      {
         boardCars[ i ] = other.boardCars[ i ];
      }  
   }
   
   /// return reference to this board 
   return *this;
}

/**
 * saveData
 * 
 * Reads in and saves data into boardCars until no more cars to
 * get data for.
 * 
 * @param cars int of how many cars
*/
void Board::saveData( int cars )
{
   /// initialize
   int i;
   numCars = cars;
   
   /// loop through until no more cars
   for( i = 0; i < numCars; i ++ )
   {
      cin >> boardCars[ i ].length >> boardCars[ i ].orientation
          >> boardCars[ i ].row >> boardCars[ i ].col;
   }
}

/**
 * forward
 * 
 * Attempts to move car forward. Checks that move is within dimensions
 * of board and that the space to move to is empty. Changes car's row or
 * column number if move was sucessful.
 * 
 * @param carIndex int of car's index in boardCars
 * @return bool if move was successful
*/
bool Board::forward( int carIndex )
{
   /// for horizontal car
   if( boardCars[ carIndex ].orientation == 'H' )
   {
      /// check that car is not at right edge of board and that it can move right
      if( boardCars[ carIndex ].col + boardCars[ carIndex ].length < 6
          && canMove( boardCars[ carIndex ].row, boardCars[ carIndex ].col
                      + boardCars[ carIndex ].length ) )
      {
         /// update car's column value and return success
         boardCars[ carIndex ].col++;
         return true;
      }
   }
   
   /// for vertical car
   else
   {
      /// check that car is not at bottom edge of board and that it can move down
      if( boardCars[ carIndex ].row + boardCars[ carIndex ].length < 6
          && canMove( boardCars[ carIndex ].row + boardCars[ carIndex ].length,
                      boardCars[ carIndex ].col ) )
      {
         /// update car's column value and return success
         boardCars[ carIndex ].row++;
         return true;
      }   
   }
   
   /// return that move was not successful
   return false;
}


/**
 * backward
 * 
 * Attempts to move car backward. Checks that move is within dimensions
 * of board and that the space to move to is empty. Changes car's row or
 * column number if move was sucessful.
 * 
 * @param carIndex int of car's index in boardCars
 * @return bool if move was successful
*/
bool Board::backward( int carIndex )
{
   /// for horizontal car
   if( boardCars[ carIndex ].orientation == 'H' )
   {
      /// check that car is not at left edge of board and that it can move left
      if( boardCars[ carIndex ].col > 0
          && canMove( boardCars[ carIndex ].row, boardCars[ carIndex ].col - 1 ) )
      {
         /// update car's column value and return success
         boardCars[ carIndex ].col--;
         return true;
      }
   }
   
   /// for vertical car
   else
   {
      /// check that car is not at top edge of board and that it can move up
      if( boardCars[ carIndex ].row > 0
          && canMove( boardCars[ carIndex ].row - 1, boardCars[ carIndex ].col ) )
      {
         /// update car's column value and return success
         boardCars[ carIndex ].row--;
         return true;
      }   
   }
   
   /// return that move was not successful
   return false;
}

/**
 * isSolved
 * 
 * Checks if car of index 0 (red car) is at the edge of the board.
 * 
 * @return bool if car is at right edge and board is solved.
*/
bool Board::isSolved() const
{
   if( numCars > 0 )
   {
      return( boardCars[ 0 ].col + boardCars[ 0 ].length == 6 );
   }
   return true;
}


/**
 * canMove
 * 
 * Attempts to move a car to specified row and column space.
 * Does this by iterating through each car and checking it's row
 * and col values to see if space can be moved to.
 * Returns whether move is possible ot not.
 * 
 * @param row int to check 
 * @param col int to check 
 * @return bool if move is possible
*/
bool Board::canMove( int row, int col ) const
{
   /// intitialize
   int i;
   int j;
   
   /// iterate through all cars
   for( i = 0; i < numCars; i++ )
   {
      /// check entire length of car
      for( j = 0; j < boardCars[ i ].length; j++ )
      {
         /// if car is horizontal
         if( boardCars[ i ].orientation == 'H' )
         {
            if( row == boardCars[ i ].row && col == boardCars[ i ].col + j )
            {
               return false;
            }
         }
         /// if car is vertical
         else 
         {
            if( row == boardCars[ i ].row + j && col == boardCars[ i ].col )
            {
               return false;
            }
         }
      }
   }
   
   /// return that move is possible
   return true;
}

/**
 * getNumCars
 * 
 * Returns number of cars in board.
 * 
 * @return int number of cars in board
*/
int Board::getNumCars() const
{
   return numCars;
}













