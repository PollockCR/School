/**
 * @file rushClasses.h
 * @author CatherinePollock
 * @date 11/29/14
 * 
 * This is the specification file for the classes
 * implemented in rushClasses.cpp. It includes Car and Board
 * classes.
*/

#ifndef RUSHCLASSES_H
#define RUSHCLASSES_H

#include <iostream>
using namespace std;

/// car class
class Car
{
   public:
         int length;
         int row;
         int col;
         char orientation;
};

/// board class
class Board
{
   public:
         Board();
         Board( const Board& );
         Board& operator=( const Board& );
         void saveData( int );
         bool forward( int );
         bool backward( int );
         bool isSolved() const;
         bool canMove( int, int ) const;
         int getNumCars() const;
         int numCars;
         Car boardCars[ 18 ];
};

#endif
