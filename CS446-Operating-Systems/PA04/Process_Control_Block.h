/**
 * @file   Process_Control_Block.h
 * @author 831835
 * @date   April 2015
 * @brief  Process control block header for Operating System simulator
 */

// definition
   #ifndef __PCB_H_
   #define __PCB_H_

// header files
   #include <fstream> // used for file input
   #include <iostream> // used for I/O
   #include <string> // used for string operations
   #include <cstring> // used for c string operations
   #include <sstream> // used for string streams
   #include <ctime> // used to keep track of time
   #include <map> // used for cylce times
   #include <list> // used to store processes
   #include <queue> // used to store process actions
   #include <algorithm> // used for finding min
   #include <pthread.h> // used for threads

   using namespace std;

// global constants

// global variables

// class specification
class Action
{
   public:
      Action();
      Action( char type, string descriptor, float cycle );
      char actionType;
      string actionDescriptor;
      float actionCycle;
      int pcbID;
   private:      
};

class PCB
{
   friend class Action;
   public:
      PCB( int id = 0 );
      int processID;
      float estimatedRuntime;
      string state;
      queue<Action> actions;
   private:
};

#endif
