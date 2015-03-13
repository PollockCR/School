/**
 * @file   Process_Control_Block.cpp
 * @author 831835
 * @date   March 2015
 * @brief  Process control block for Operating System simulator
 */

// header files
   #include "Process_Control_Block.h"
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

// class implementation
PCB::PCB( int id )
{
   processID = id;
   state = "Enter";
}

Action::Action() {}

Action::Action( char type, string descriptor, int cycle )
{
   actionType = type;
   actionDescriptor = descriptor;
   actionCycle = cycle;
}

