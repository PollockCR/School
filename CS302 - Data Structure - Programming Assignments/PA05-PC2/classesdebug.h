/**
 * @file classes.h
 * @author CatherinePollock
 * @date 9/30/14
 * 
 * This is the specification file for the classes
 * implemented in classes.cpp.
*/

#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
using namespace std;

class Car
{
   public:
         Car();
         Car( const int newLength, const bool newIsHorz, const int newRowNum, const int newColNum );
         ~Car();
         int length;
         bool isHorz;
         int rowNum;
         int colNum;
};

class Board
{
   public:
         Board();
         Board( Car [], int newNumCars );
         Board operator=( Board source );
         ~Board();
         void printBoard();void printSolBoard();
         bool solveIt( int movesSoFar );
         bool forward( int carIndex );
         bool backward( int carIndex );
         bool amIDoneYet();
         int minMoves;
         int numCars;
         int boardArr[ 6 ][ 6 ];
         Car boardCars[ 10 ];
         int solArr[6][6];
};
#endif
