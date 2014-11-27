/**
 * @file rush.cpp
 * @author CatherinePollock
 * @date 9/30/14
 * 
 * This is the main driver file for the game Rush Hour.
*/

/// header files
#include <iostream>
using namespace std;

/// global constants
const int MAX_CARS = 10;
const int MAX_SCENARIOS = 10;

/// data structures
struct Car
{
   int length;
   bool isHorz;
   int rowNum;
   int colNum;
};

/// function prototypes
int saveCars( Car carArr[][ MAX_CARS ] );

/// main driver
int main()
{
   /// initalize variables 
   int numScenarios;
   Car carArr[ MAX_SCENARIOS ][ MAX_CARS ];
   
   /// save cars
   numScenarios = saveCars( carArr );

   //
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
int saveCars( Car carArr[][ MAX_CARS ] )
{
   /// initialize variables
   int numCars, scenarioNum = 0, carIndex, temp;
   char tempOrientation;
   
   /// get scenario 
   cin >> numCars;
   
   /// loop through until no more scenarios
   while( numCars != 0 )
   {
      /// loop through and save car info
      for( carIndex = 0; carIndex < numCars; carIndex++ )
      {
         /// save car length
         cin >> temp;
         carArr[ scenarioNum ][ carIndex ].length = temp;
         
         /// save car orientation
         cin >> tempOrientation;
         while( tempOrientation == ' ' || tempOrientation == '\n' )
         {
            cin >> tempOrientation;
         } 
         if( tempOrientation == 'H')
         {
            carArr[ scenarioNum ][ carIndex ].isHorz = true;
         }
         else
         {
            carArr[ scenarioNum ][ carIndex ].isHorz = false;
         }
         
         /// save car row number
         cin >> temp;
         carArr[ scenarioNum ][ carIndex ].rowNum = temp;
         
         /// save car column number
         cin >> temp;
         carArr[ scenarioNum ][ carIndex ].colNum = temp;

      }
      
      /// count scenario as completed
      scenarioNum++;
      
      /// get next number of cars
      cin >> numCars;
   }
   /// return number of scenarios saved
   return scenarioNum;
}











