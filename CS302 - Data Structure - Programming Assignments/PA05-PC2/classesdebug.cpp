/**
 * @file classes.cpp
 * @author CatherinePollock
 * @date 9/30/14
 * 
 * This is the file that implements classes.h and
 * the classes within that file.
*/

#include "classesdebug.h"
#include <iostream>
using namespace std;

Car::Car()
{
   length = 0;
   isHorz = false;
   rowNum = 0;
   colNum = 0;
}

Car::Car( const int newLength, const bool newIsHorz, const int newRowNum, const int newColNum )
{
   length = newLength;
   isHorz = newIsHorz;
   rowNum = newRowNum;
   colNum = newColNum;
}

Car::~Car(){}

Board::Board()
{
   int row, col;
   minMoves = 25;
   
   /// set all values of board to NULL
   for( row = 0; row < 6; row++ )
   {
      for( col = 0; col < 6; col++ )
      {
         boardArr[ row ][ col ] = -1;
      }
   }
}

Board::Board( Car carList[], int newNumCars )
{
   int carCount, row, col;
   numCars = newNumCars;
   minMoves = 25;

   
   /// set all values of board to NULL
   for( row = 0; row < 6; row++ )
   {
      for( col = 0; col < 6; col++ )
      {
         boardArr[ row ][ col ] = -1;
      }
   }
   
   for( carCount = 0; carCount < numCars; carCount++ )
   { 
      /// save car info 
      boardCars[ carCount ].length = carList[ carCount ].length;
      boardCars[ carCount ].isHorz = carList[ carCount ].isHorz;
      boardCars[ carCount ].rowNum = carList[ carCount ].rowNum;
      boardCars[ carCount ].colNum = carList[ carCount ].colNum;
   
      /// for car length 2 and horizontal 
      if ( carList[ carCount ].length == 2 && carList[ carCount ].isHorz )
      {
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum+1 ] = carCount;
      } 
      
      /// for car length 3 and horizontal 
      if ( carList[ carCount ].length == 3 && carList[ carCount ].isHorz )
      {
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum+1 ] = carCount;
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum+2 ] = carCount;
      }
      
      /// for car length 2 and vertical 
      if ( carList[ carCount ].length == 2 && !carList[ carCount ].isHorz )
      {
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum+1 ][ carList[ carCount ].colNum ] = carCount;
      }
      
      /// for car length 3 and vertical 
      if ( carList[ carCount ].length == 3 && !carList[ carCount ].isHorz )
      {
         boardArr[ carList[ carCount ].rowNum ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum+1 ][ carList[ carCount ].colNum ] = carCount;
         boardArr[ carList[ carCount ].rowNum+2 ][ carList[ carCount ].colNum ] = carCount;
      }
   }
   
}

Board Board::operator=( Board source )
{
   int row, col, carCount;
   numCars = source.numCars;
   minMoves = source.minMoves;
   
   for( carCount = 0; carCount < numCars; carCount++ )
   {
      /// save car info 
      boardCars[ carCount ].length = source.boardCars[ carCount ].length;
      boardCars[ carCount ].isHorz = source.boardCars[ carCount ].isHorz;
      boardCars[ carCount ].rowNum = source.boardCars[ carCount ].rowNum;
      boardCars[ carCount ].colNum = source.boardCars[ carCount ].colNum;
      
   }
   
   /// set all values of board to source's
   for( row = 0; row < 6; row++ )
   {
      for( col = 0; col < 6; col++ )
      {
         boardArr[ row ][ col ] = source.boardArr[ row ][ col ];
      }
   }
}

Board::~Board(){}

void Board::printBoard()
{
   int row, col;
   cout << endl;
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

void Board::printSolBoard()
{
   int row, col;
   cout << endl;
   for( row = 0; row < 6; row++ )
   {
      for( col = 0; col < 6; col++ )
      {
         if( solArr[ row ][ col ] == -1)
         {
            cout << 'x';
         }
         else 
         {
            cout << solArr[ row ][ col ];
         }
      }
      cout << endl;
   }
}

bool Board::solveIt( int movesSoFar )
{
   int carIndex;
   
   /// check if done yet
   if( amIDoneYet() )
   {
      /// save number of moves
      if( movesSoFar < minMoves )
      {
         minMoves = movesSoFar;
         cout << minMoves << ' ';
        
         /// set all values of board to boardArr's
         for( int row = 0; row < 6; row++ )
         {
            for( int col = 0; col < 6; col++ )
            {
               solArr[ row ][ col ] = boardArr[row][col];
            }
         }
      }
      return true;
   }
   if( movesSoFar > 25 )
   {
      return false;
   }
   for( carIndex = 0; carIndex < numCars; carIndex++ )
   {
      if( forward( carIndex ) )
      {
         movesSoFar++;
         solveIt( movesSoFar );
      }
      if( backward( carIndex ) )
      {
         movesSoFar++;
         solveIt( movesSoFar );
      }
   }
}

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
      if( boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum+2 ] != -1 )
      {
         return false;
      }
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
      if( boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum+3 ] != -1 )
      {
         return false;
      }
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
      if( boardArr[ boardCars[ carIndex ].rowNum+2 ][ boardCars[ carIndex ].colNum ] != -1 )
      {
         return false;
      }
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
      if( boardArr[ boardCars[ carIndex ].rowNum+3 ][ boardCars[ carIndex ].colNum ] != -1 )
      {
         return false;
      }
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum+3 ][ boardCars[ carIndex ].colNum ] = carIndex;
         (boardCars[ carIndex ].rowNum)++;
         return true;
      }
   }   
}

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
      if( boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum-1 ] != -1 )
      {
         return false;
      }
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
      if( boardArr[ boardCars[ carIndex ].rowNum ][ boardCars[ carIndex ].colNum-1 ] != -1 )
      {
         return false;
      }
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
      if( boardArr[ boardCars[ carIndex ].rowNum-1 ][ boardCars[ carIndex ].colNum ] != -1 )
      {
         return false;
      }
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
      if( boardArr[ boardCars[ carIndex ].rowNum-1 ][ boardCars[ carIndex ].colNum ] != -1 )
      {
         return false;
      }
      else
      {
         boardArr[ boardCars[ carIndex ].rowNum+2 ][ boardCars[ carIndex ].colNum ] = -1;
         boardArr[ boardCars[ carIndex ].rowNum-1 ][ boardCars[ carIndex ].colNum ] = carIndex;
         (boardCars[ carIndex ].rowNum)--;
         return true;
      }
   } 
}

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










