/**
 * @file rushSTL.cpp
 * @author Catherine Pollock
 * @date 11/29/14
 * 
 * This is the main driver file for the game Rush Hour.
 * It saves given car information into boards, which 
 * are saved into a queue and map with checked boards.
 * It attempts to solve for the minimum moves required
 * and prints results. This is done with a breath first
 * search, that checks across each level before continuing search.
*/

/// header files
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <queue>
#include "rushClasses.h"
using namespace std;

/// global constants
const int MAX_CARS = 10;
const int BOARD_SIZE = 6;

/// function prototypes
int solve( const int );
string boardToString( const Board& );
Board stringToBoard( const string );

/// main driver
int main()
{
   /// initialization
   int numCars;
   int numMoves;
   int scenarioNum = 1;
   
   /// read in number of cars in first scenario
   cin >> numCars;
   
   /// continue until no more scenarios
   while( numCars > 0 )
   {
      /// solve the board
      numMoves = solve( numCars );
      
      /// print results
      cout << "Scenario " << scenarioNum << " requires "
           << numMoves << " moves" << endl;
           
      /// increment scenario number
      scenarioNum++;
      
      /// read in number of cars for next scenario
      cin >> numCars;
   }
}


/// function implementation

/**
 * solve
 * 
 * Attempts to solve the board using a breath first search.
 * Creates a queue of boards and adds boards as moves are made.
 * Reads in data for initial board. Makes moves on board and
 * saves boards. Checks if board is solved and returns
 * numMoves for board that is solved soonest.
 * 
 * @param numCars int number of cars in board
 * @return number of moves required
*/
int solve( const int numCars )
{
   /// initialization
   int numMoves = 0;
   int i;
   queue<string> boardQueue;
   map<string,int> checkedBoards;
   Board board;
   string boardS;
   
   /// save data to board
   board.saveData( numCars );
   
   /// save board as a string
   boardS = boardToString( board );
   
   /// check if board is solved
   if( board.isSolved() )
   {
      return numMoves;
   }
   
   /// adds board to queue
   boardQueue.push( boardS );
   
   /// adds board to map of checked boards 
   checkedBoards.insert( pair<string,int>( boardS, numMoves ) );
   
   /// continue until queue is empty
   while( !boardQueue.empty() )
   {
      /// take board off queue
      boardS = boardQueue.front();
      boardQueue.pop();
      board = stringToBoard( boardS );
      
      /// finds board on checked board and saves numMoves
      numMoves = checkedBoards.find( boardS ) -> second;
      
      /// check if board is solved
      if( board.isSolved() )
      {
         return numMoves;
      }
   
      /// iterate through cars on board
      for( i = 0; i < numCars; i++ )
      {
         /// move car forward
         if( board.forward( i ) )
         {
            /// update board string
            boardS = boardToString( board );
            
            /// check map of boards
            if( checkedBoards.count( boardS ) == 0 )
            {
               /// put board on queue
               boardQueue.push( boardS );
               
               /// put board on checked boards map
               checkedBoards.insert( pair<string,int>( boardS, numMoves + 1 ) );
            }
            
            /// move car backward
            board.backward( i );
            
            /// update board string
            boardS = boardToString( board );
         }
         
         /// move car backward
         if( board.backward( i ) )
         {
            /// update board string
            boardS = boardToString( board );
                     
            /// check map of boards
            if( checkedBoards.count( boardS ) == 0 )
            {
               /// put board on queue
               boardQueue.push( boardS );
               
               /// put board on checked boards map
               checkedBoards.insert( pair<string,int>( boardS, numMoves + 1 ) );
            }
            
            /// move car backward
            board.forward( i );  
            
            /// update board string
            boardS = boardToString( board );    
         }
      }
   }
   
   /// return -1 if not solved
   return -1;
}

/**
 * boardToString
 * 
 * Converts a board to a string containing car data.
 * 
 * @param board Board to get string data from
 * @return string containing board data
*/
string boardToString( const Board& board )
{
   /// initialize
   int i;
   stringstream ss;
   string returnString;
   
   /// concatenate stringstream with each car's data
   for( i = 0; i < board.numCars; i++ )
   {
      ss << board.boardCars[i].length
         << board.boardCars[i].row  << board.boardCars[i].col
         << board.boardCars[i].orientation;
   }
   
   /// convert from string stream to string
   returnString = ss.str();

   /// return string with car data for board
   return returnString;
}

/**
 * stringToBoard
 * 
 * Converts a string to a board containing car data.
 * 
 * @param boardString string to get board data from
 * @return board with car data
*/
Board stringToBoard( const string boardString )
{
   /// initialize
   Board board;
   board.numCars = boardString.length() / 4;
   int i;
   
   for( i = 0; i < board.numCars; i++ )
   {
      board.boardCars[ i ].length = boardString[ ( i * 4 ) ] - '0'; 
      board.boardCars[ i ].row = boardString[ ( i * 4 ) + 1 ] - '0'; 
      board.boardCars[ i ].col = boardString[ ( i * 4 ) + 2 ] - '0'; 
      board.boardCars[ i ].orientation = boardString[ ( i * 4 ) + 3 ]; 
   }
   
   /// return Board with data from string
   return board;
}


























