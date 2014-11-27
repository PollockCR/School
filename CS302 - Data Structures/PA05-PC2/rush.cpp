/**
 * @file rush.cpp
 * @author CatherinePollock
 * @date 9/30/14
 * 
 * This is the main driver file for the game Rush Hour.
 * It saves given car information into a list of cars and
 * a board array. It attempts to solve for the minimum
 * moves required and prints results. 
*/

/// header files
#include <iostream>
#include "classes.h"
using namespace std;

/// global constants
const int MAX_CARS = 10;
const int BOARD_SIZE = 6;

/// function prototypes
int saveCars( Car carArr[] );
bool solve( int movesSoFar );

/// main driver
int main()
{
   /// initalize variables 
   int numCars;
   Car carList[ MAX_CARS ];
   Board secondBoard;
   
   /// save cars into car list
   numCars = saveCars( carList );
   
   /// put cars onto board
   Board firstBoard( carList, numCars );
   
   /// solve for minimum moves
   firstBoard.solveIt( 0 );
   
   /// output results
   cout << "Scenario 1 requires " << firstBoard.minMoves << " moves" << endl;

   /// return success
   return 0;
}

/// function implementation

/**
 * saves cars to array
 * 
 * Gets the number of cars to be saved for first scenario.
 * Saves the length, orientation, row and column numbers for each car.
 * Repeats this until numCars is given 0 to end reading in of data.
 * Returns the number of seperate scenarios saved.
 *
 * @return scenarioNum int value of number of scenarios saved
 * @param array of cars
*/
int saveCars( Car carList[] )
{
   /// initialize variables
   int numCars, carIndex, temp;
   char tempOrientation;
   
   /// get number of cars to be saved 
   cin >> numCars;
   
   /// loop through and save car info
   for( carIndex = 0; carIndex < numCars; carIndex++ )
   {
      /// save car length
      cin >> temp;
      carList[ carIndex ].length = temp;
      
      /// save car orientation
      cin >> tempOrientation;
     
         /// skip white space characters
         while( tempOrientation == ' ' || tempOrientation == '\n' )
         {
            cin >> tempOrientation;
         }
       
         /// save orientation correctly based on given letter
         if( tempOrientation == 'H')
         {
            carList[ carIndex ].isHorz = true;
         }
         else
         {
           carList[ carIndex ].isHorz = false;
         }
      
      /// save car row number
      cin >> temp;
      carList[ carIndex ].rowNum = temp;
      
      /// save car column number
      cin >> temp;
      carList[ carIndex ].colNum = temp;
   }
      
   /// take in zero as end flag
   cin >> temp;
   
   /// return number of scenarios saved
   return numCars;
}











