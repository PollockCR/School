/**
 * @file   OS_Control_Block.cpp
 * @author 831835
 * @date   April 2015
 * @brief  Operating system control block for Operating System simulator
 */

// header files
   #include "OS_Control_Block.h"
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

// class implementation
OSCB::OSCB() {}
