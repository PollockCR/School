/**
 * @file   Process_Control_Block.h
 * @author 831835
 * @date   March 2015
 * @brief  Process control block header for Operating System simulator
 */

// definition
   #ifndef __PCB_H_
   #define __PCB_H_

// header files
   #include <sys/types.h> // used for process creation
   #include <fstream> // used for file input
   #include <iostream> // used for I/O
   #include <string> // used for string operations
   #include <cstring> // used for c string operations
   #include <sstream> // used for string streams 
   #include <ctime> // used to keep track of time
   #include <map> // used for cylce times
   #include <list> // used for process actions

   using namespace std;

// global constants

// global variables

// class specification
class Action
{
   public:
      Action();
      Action( char type, string descriptor, int cycle );
      char actionType;
      string actionDescriptor;
      int actionCycle;
   private:      
};

class PCB
{
   friend class Action;
   public:
      PCB( int id = 1 );
      int processID;
      string state;
      list<Action> actions;
   private:
};



#endif