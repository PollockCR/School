/**
 * @file   OS_Control_Block.h
 * @author 831835
 * @date   April 2015
 * @brief  Operating system control block header for Operating System simulator
 */

// definition
   #ifndef __OSCB_H_
   #define __OSCB_H_

// header files
   #include "Process_Control_Block.h"
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

// class declaration
class OSCB
{
   friend class Action;   
   public:
      OSCB();
      clock_t timer; // keeps track of time
      map<string, float> cycleData; // cycles per action
      float quantum; // holds quantum time value (number of cycles)
      char logLocation; // location to log to
      ofstream outputFile; // output file stream
      string schedulingCode; // scheduling code  
      float phase; // value of current phase
      string metaPath; // filepath of meta-data file
      string logPath; // filepath to log to 
      Action* currentAction; // current action to complete
   private:
};

#endif
