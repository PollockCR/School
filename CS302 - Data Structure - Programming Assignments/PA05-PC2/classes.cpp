/**
 * @file classes.cpp
 * @author CatherinePollock
 * @date 9/30/14
 * 
 * This is the file that implements classes.h and
 * the classes within that file.
*/

#include "classes.h"
#include <iostream>
using namespace std;

/**
 * Default constructor for car class. 
 * 
 * Default constructor for car class.
 * Sets data members to zero and false.
*/
Car::Car()
{ 
   /// initialize variables
   length = 0;
   isHorz = false;
   rowNum = 0;
   colNum = 0;
}

/**
 * Default destructor for car class. 
 * 
 * No memory was allocated dynamically.
*/
Car::~Car(){}

/**
 * Default constructor for board class. 
 * 
 * Default constructor for board class. Sets all values
 * of the array to -1 to show emptyness. 
*/
Board::Board()
{
   /// initialize variables
   int row, col;
   minMoves = 10;
   
   /// set all values of board to -1
   for( row = 0; row < 6; row++ )
   {
      for( col = 0; col < 6; col++ )
      {
         boardArr[ row ][ col ] = -1;
      }
   }
}

/**
 * Parameterized constructor for board class. 
 * 
 * Sets a board up with values given in car list
 * with a given number of cars. 
 *
 * @param carList a list of cars
 * @param newNumCars an int of how many cars in list 
*/
Board::Board( Car carList[], int newNumCars )
{
   /// initialize variables
   int carCount, row, col;
   numCars = newNumCars;
   minMoves = 10;

   /// set all values of board to -1
   for( row = 0; row < 6; row++ )
   {
      for( col = 0; col < 6; col++ )
      {
         boardArr[ row ][ col ] = -1;
      }
   }
   
   /// loop through each car
   for( carCount = 0; carCount < numCars; carCount++ )
   { 
      /// save car info to list of cars in board class
      boardCars[ carCount ].length = carList[ carCount ].length;
      boardCars[ carCount ].isHorz = carList[ carCount ].isHorz;
      boardCars[ carCount ].rowNum = carList[ carCount ].rowNum;
      boardCars[ carCount ].colNum = carList[ carCount ].colNum;
   
      /// for car length 2 and horizontal, place on board
      if ( carList[ carCount ].length == 2 && carList[ carCount ].isHorz )
      {
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum+1 ] = carCount;
      } 
      
      /// for car length 3 and horizontal, place on board
      if ( carList[ carCount ].length == 3 && carList[ carCount ].isHorz )
      {
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum+1 ] = carCount;
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum+2 ] = carCount;
      }
      
      /// for car length 2 and vertical, place on board
      if ( carList[ carCount ].length == 2 && !carList[ carCount ].isHorz )
      {
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum+1 ][ carList[ carCount ].colNum ] = carCount;
      }
      
      /// for car length 3 and vertical, place on board
      if ( carList[ carCount ].length == 3 && !carList[ carCount ].isHorz )
      {
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum+1 ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum+2 ][ carList[ carCount ].colNum ] = carCount;
      }
   }
   
}

/**
 * Overloaded assignment operator for board class. 
 * 
 * This was not needed but I wrote it anyways. It works. Sets
 * one board equal to another 
 *
 * @param source a Board to be copied from
 * @return Board new board with source's values
*/
Board Board::operator=( Board source )
{
   /// initialize variables
   int row, col, carCount;
   numCars = source.numCars;
   minMoves = source.minMoves;
   
   /// loop through cars
   for( carCount = 0; carCount < numCars; carCount++ )
   {
      /// save car info to board's car list
      boardCars[ carCount ].length = source.boardCars[ carCount ].length;
      boardCars[ carCount ].isHorz = source.boardCars[ carCount ].isHorz;
      boardCars[ carCount ].rowNum = source.boardCars[ carCount ].rowNum;
      boardCars[ carCount ].colNum = source.boardCars[ carCount ].colNum;
      
   }
   
   /// set all values of this board array to equal source's board array
   for( row = 0; row < 6; row++ )
   {
      for( col = 0; col < 6; col++ )
      {
         boardArr[ row ][ col ] = source.boardArr[ row ][ col ];
      }
   }
}

/**
 * Default destructor for car class. 
 * 
 * No memory was allocated dynamically.
*/
Board::~Board(){}

/**
 * Print board function. 
 * 
 * For testing purposes only. Prints values in board. 
 * @return void
*/
void Board::printBoard()
{
   /// initialize variables
   int row, col;
   cout << endl;
   
   /// loop through array and print each value
   for( row = 0; row < 6; row++ )
   {
      for( col = 0; col < 6; col++ )
      {
         if( boardArr[ row ][ col ] == -1)
         {
            cout << 'x';
         }
         else 
         {
            cout << boardArr[ row ][ col ];
         }
      }
      cout << endl;
   }
}

/**
 * Function to solve board. 
 * 
 * Begin's by checking if car 0 is at right edge, 
 * if it is, it sets minimum moves if nescessary. Next,
 * it checks amount of moves so far to ensure under max
 * value. Otherwise, it loops through cars and tries to move 
 * them forward or backward, recursively.
 * @param movesSoFar int that counts moves
 * @return bool which indicates solving completion
*/
bool Board::solveIt( int movesSoFar )
{
   /// initialize variables
   int carIndex;
   
   /// check if done yet
   if( amIDoneYet() )
   {
      /// save number of moves
      if( movesSoFar < minMoves )
      {
         minMoves = movesSoFar;
      }
      return true;
   }
   
   /// checks for too many moves tried
   if( movesSoFar > 10 )
   {
      return false;
   }
   
   /// loops through cars
   for( carIndex = 0; carIndex < numCars; carIndex++ )
   {
      /// if is able to move car forward, call with new moves so far
      if( forward( carIndex ) )
      {
         movesSoFar++;
         solveIt( movesSoFar );
      }
      
      /// if is able to move car backward, call with new moves so far
      if( backward( carIndex ) )
      {
         movesSoFar++;
         solveIt( movesSoFar );
      }
   }
}

/**
 * Function to move car forward. 
 * 
 * Finds appropriate type of car / orientation of car and attempts to move.
 * Fails if data is in the way or is on edge.
 * @param carIndex int with given carIndex value
 * @return bool which indicates successful move forward
*/
bool Board::forward( int carIndex )
{
   /// for length of 2 and horizontal
   if( boardCars[ carIndex ].isHorz && boardCars[ carIndex ].length == 2 )
   {
      /// return false if at edge
      if( boardCars[ carIndex ].colNum == 4)
      {
         return false;
      }
      
      /// return false if car in the way
      if( boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum+2 ] != -1 )
      {
         return false;
      }
      
      /// move car
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum+2 ] = carIndex;
         (boardCars[ carIndex ].colNum)++;
         return true;
      }
   }
   
   /// for length of 3 and horizontal
   if( boardCars[ carIndex ].isHorz && boardCars[ carIndex ].length == 3 )
   {
      /// return false if at edge
      if( boardCars[ carIndex ].colNum == 3)
      {
         return false;
      }
            
      /// return false if car in the way
      if( boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum+3 ] != -1 )
      {
         return false;
      }
            
      /// move car
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum+3 ] = carIndex;
         (boardCars[ carIndex ].colNum)++;
         return true;
      }
   }
   
   /// for length of 2 and vertical
   if( !(boardCars[ carIndex ].isHorz) && boardCars[ carIndex ].length == 2 )
   {
      /// return false if at edge
      if( boardCars[ carIndex ].rowNum == 4)
      {
         return false;
      }
            
      /// return false if car in the way
      if( boardArr[ boardCars[ carIndex ].rowNum+2 ][ boardCars[ carIndex ].colNum ] != -1 )
      {
         return false;
      }
            
      /// move car
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum+2 ][ boardCars[ carIndex ].colNum ] = carIndex;
         (boardCars[ carIndex ].rowNum)++;
         return true;
      }
   }
   
   /// for length of 3 and vertical
   if( !(boardCars[ carIndex ].isHorz) && boardCars[ carIndex ].length == 3 )
   {
      /// return false if at edge
      if( boardCars[ carIndex ].rowNum == 3)
      {
         return false;
      }
            
      /// return false if car in the way
      if( boardArr[ boardCars[ carIndex ].rowNum+3 ][ boardCars[ carIndex ].colNum ] != -1 )
      {
         return false;
      }
            
      /// move car
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum+3 ][ boardCars[ carIndex ].colNum ] = carIndex;
         (boardCars[ carIndex ].rowNum)++;
         return true;
      }
   }   
}

/**
 * Function to move car backward. 
 * 
 * Finds appropriate type of car / orientation of car and attempts to move.
 * Fails if data is in the way or is on edge.
 * @param carIndex int with given carIndex value
 * @return bool which indicates successful move backward
*/
bool Board::backward( int carIndex )
{
   /// for length of 2 and horizontal
   if( boardCars[ carIndex ].isHorz && boardCars[ carIndex ].length == 2 )
   {
      /// return false if at edge
      if( boardCars[ carIndex ].colNum == 0)
      {
         return false;
      }
            
      /// return false if car in the way
      if( boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum-1 ] != -1 )
      {
         return false;
      }
            
      /// move car
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum+1 ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum-1 ] = carIndex;
         (boardCars[ carIndex ].colNum)--;
         return true;
      }
   }
   
   /// for length of 3 and horizontal
   if( boardCars[ carIndex ].isHorz && boardCars[ carIndex ].length == 3 )
   {
      /// return false if at edge
      if( boardCars[ carIndex ].colNum == 0)
      {
         return false;
      }
            
      /// return false if car in the way
      if( boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum-1 ] != -1 )
      {
         return false;
      }
            
      /// move car
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum+2 ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum-1 ] = carIndex;
         (boardCars[ carIndex ].colNum)--;
         return true;
      }
   }
   
   /// for length of 2 and vertical
   if( !(boardCars[ carIndex ].isHorz) && boardCars[ carIndex ].length == 2 )
   {
      /// return false if at edge
      if( boardCars[ carIndex ].rowNum == 0)
      {
         return false;
      }
            
      /// return false if car in the way
      if( boardArr[ boardCars[ carIndex ].rowNum-1 ][ boardCars[ carIndex ].colNum ] != -1 )
      {
         return false;
      }
            
      /// move car
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum+1 ][ boardCars[ carIndex ].colNum ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum-1 ][ boardCars[ carIndex ].colNum ] = carIndex;
         (boardCars[ carIndex ].rowNum)--;
         return true;
      }
   }
   
   /// for length of 3 and vertical
   if( !(boardCars[ carIndex ].isHorz) && boardCars[ carIndex ].length == 3 )
   {
      /// return false if at edge
      if( boardCars[ carIndex ].rowNum == 0)
      {
         return false;
      }
            
      /// return false if car in the way
      if( boardArr[ boardCars[ carIndex ].rowNum-1 ][ boardCars[ carIndex ].colNum ] != -1 )
      {
         return false;
      }
            
      /// move car
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum+2 ][ boardCars[ carIndex ].colNum ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum-1 ][ boardCars[ carIndex ].colNum ] = carIndex;
         (boardCars[ carIndex ].rowNum)--;
         return true;
      }
   } 
}

/**
 * Function to check for completion. 
 * 
 * Loops through each row and returns indication of
 * value 0 at right edge (red car made it out).
 * @return bool which indicates red car's status
*/
bool Board::amIDoneYet()
{
   int row;
   for( row = 0; row < 6; row++ )
   {
      if( boardArr[ row ][ 5 ] == 0 )
      {
         return true;
      }
   }
   return false;
}










