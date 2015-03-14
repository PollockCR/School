/**
 * @file   OS_Phase_1.cpp
 * @author 831835
 * @date   March 2015
 * @brief  One-program operating system simulator
 *
 * Operating system simulator with three states (Enter/Start, 
 * Running, Exit). It will accept meta-data for one program, 
 * run it, and end the simulation. Each unique I/O operation 
 * is conducted with its own unique thread. 
 */

// header files
   #include "Process_Control_Block.h" 
   #include <fstream> // used for file input
   #include <iostream> // used for I/O
   #include <string> // used for string operations
   #include <cstring> // used for c string operations
   #include <sstream> // used for string streams
   #include <ctime> // used to keep track of time
   #include <map> // used for cylce times
   #include <list> // used for process actions
   #include <pthread.h> // used for threads

   using namespace std;

// global constants
   const int STD_STR_LEN = 45;
   const int STD_LINE_LEN = 256;
   const int SPACE_BUF = 1;
   const int OP_COUNT = 5;
   const int PRECISION = 6;
   const float CYCLE_TO_MS = 0.001;

// global variables
   clock_t timer; // keeps track of time
   map<string, float> cycleData; // cycles per action
   char logLocation; // location to log to
   ofstream outputFile; // output file stream

// function prototypes
   bool saveConfig( char* configPath, float &phase, char* metaPath, char* logPath );
   int saveMetaData( char* metaPath, PCB& process );
   void threadActions( PCB& process );
   void *runner( void *actVoidPtr ); 
   void printAction( stringstream& actionOutput ); 

// main program
int main( int argc, char* argv[] )
{
   // initialize variables
   bool saveSucess;
   float phase = 0.0;
   char metaPath[ STD_LINE_LEN ];
   char logPath[ STD_LINE_LEN ]; 
   int processCount = 0;
   PCB process;

   // STATE: Enter/Start

      // set process state to enter
      process.state = "Enter";

      // read in configuration file

         // check for incorrect arguments 
         if( argc != 2 )
         {
            // print failure 
            cout << "Incorrect arguments given. Please try again." << endl;

            // return failure
            return 1;
         }

         // save configuration information
         saveSucess = saveConfig( argv[1], phase, metaPath, logPath );

         // check for failed save
         if( !saveSucess )
         {
            // print failure
            cout << "Error in configuration file. Please try again." << endl;

            // return failure
            return 1;
         }

      // read in meta-data
      processCount = saveMetaData( metaPath, process );

         // check for invalid meta-data
         if( processCount < 1 )
         {
            // print failure
            cout << "Error in meta-data. Please try again." << endl;

            // return failure
            return 1;            
         }

   // STATE: Running

      // change process to running
      process.state = "Running";

      // prepare for output 

         // for file output 
         if( logLocation != 'M' )
         {
            // open log file 
            outputFile.open( logPath, ofstream::out | ofstream::trunc );

            // check for valid file
            if( !outputFile.is_open() )
            {
               // print failure 
               cout << "Error in log file. Please try again." << endl;

               // return failure
               return 1;
            }
         }

      // run simulator
      threadActions( process );

      // for file output
      if( logLocation != 'M' )
      {
         // close file
         outputFile.close();      
      }

   // STATE: Exit

      // change process to exit
      process.state = "Exit";

      // return success
      return 0;
}


// function implementation 

/**
saveConfig

Saves configuration file information.
First, opens file for input and ensures that it is valid.
Ignores information where appropriate and saves information to
appropriate location. Any errors in reading in data will cause 
false to be returned. Valid data returns true. 

@param configPath holds path to configuration file
@param phase holds phase/version value
@param metaPath holds path to meta-data file for saving later
@param logPath holds path to log file, if needed
@pre configPath was passed to program
@post configuration information saved or error returned
@return bool representing sucessful saving of configuration
*/
bool saveConfig( char* configPath, float &phase, char* metaPath, char* logPath )
{
   // initialize variables
   ifstream configFile;
   float tempTime;
   char tempPath[ STD_LINE_LEN ];

   // read in file

      // open file
      configFile.open(configPath, ifstream::in);

         // check for open failure
         if( !configFile.is_open() )
         {
            // return failure
            return false;
         }

      // ignore start line
      configFile.ignore( STD_LINE_LEN, '\n' );

      // read in version
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile >> phase;

      // read in meta-data filepath
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile.ignore( SPACE_BUF, ' ' );
      configFile.getline( metaPath, STD_LINE_LEN );

      // read in cycle data

         // read in processor cycle time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;
         cycleData[ "run" ] = tempTime;

         // read in monitor display time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;
         cycleData[ "monitor" ] = tempTime;

         // read in hard drive cycle time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;
         cycleData[ "hard drive" ] = tempTime;         

         // read in printer cycle time 
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;
         cycleData[ "printer" ] = tempTime;         

         // read in keyboard cycle time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;
         cycleData[ "keyboard" ] = tempTime;         

      // read in log location
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile.ignore( SPACE_BUF, ' ' );
      configFile.getline( tempPath, STD_LINE_LEN );

         // log to monitor
         if( strcmp( tempPath, "Log to Monitor" ) == 0 )
         {
            logLocation = 'M';
         }

         // log to both
         else if( strcmp( tempPath, "Log to Both" ) == 0 )
         {
            logLocation = 'B';
         }

         // log to monitor
         else if( strcmp( tempPath, "Log to File" ) == 0 )
         {
            logLocation = 'F';
         }

         // unrecognized location
         else 
         {
            return false;
         }

      // read in log file path
      if( logLocation  != 'M' )
      {
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile.ignore( SPACE_BUF, ' ' );
         configFile.getline( logPath, STD_LINE_LEN );
      }

   // close file
   configFile.close();

   // check for save failure
   if( cycleData.size() != OP_COUNT || configFile.eof() )
   {
      return false;
   }

   // return sucess
   return true;

}

/**
saveMetaData

Saves meta-data file information.
First, opens file for input and ensures that it is valid.
Ignores information where appropriate and saves information to
appropriate location. Actions are sent into a process control
block and into a queue within the PCB. The number of seperate
process programs is returned. For now this is either 0 (error),
or 1 (success).

@param metaPath holds path to meta-data file for saving later
@param process process control block with process information
@pre valid config information was saved
@post configuration information saved or error returned
@return int representing number of processes saved (1 or 0)
*/
int saveMetaData( char* metaPath, PCB& process )
{
   // initialize variables
   int processCount = 0;
   ifstream metaFile;
   char tempType;
   string tempDescriptor;
   int tempCycle;
   Action tempAction;

   // open meta-data file
   metaFile.open( metaPath, ifstream::in );
         
      // check for open failure
      if( !metaFile.is_open() )
      {
         // return 0 process count
         return processCount;
      }   

   // read in meta-data

         // read in start - skip first line
         metaFile.ignore( STD_LINE_LEN, ';' );
         metaFile.ignore( SPACE_BUF, ' ' );         

         // add processes until end of file or end simulation command
         while( metaFile.good() && tempType != 'S' )
         {
            // get process start action
            metaFile.get( tempType );

            // recognized start action
            if( tempType == 'A' )
            {
               // queue start action
               tempAction.actionType = tempType;
               tempAction.actionDescriptor = "start";
               tempAction.actionCycle = 0;
               process.actions.push_back( tempAction );

               // move to next action
               metaFile.ignore( STD_LINE_LEN, ';' );
               metaFile.ignore( SPACE_BUF, ' ' ); 

               // get next action
               metaFile.get( tempType );

               // until end action
               while( tempType != 'A' )
               {
                  // clear space
                  if( tempType == ' ' || tempType == '\n' )
                  {
                     metaFile.get( tempType );
                  }                 

                  // recognized actions
                  if( tempType == 'P' || tempType == 'I' || tempType == 'O' )
                  {
                     // save meta-data for process action
                     metaFile.ignore( SPACE_BUF, '(' );
                     getline( metaFile, tempDescriptor, ')' );
                     metaFile >> tempCycle; 

                     // save data to temp process               
                     tempAction.actionType = tempType;
                     tempAction.actionDescriptor = tempDescriptor;
                     tempAction.actionCycle = tempCycle;

                     // queue process action 
                     process.actions.push_back( tempAction ); 
                  }

                  // unrecognized action
                  else 
                  {
                     // return no saved processes
                     return 0;
                  }

                  // move to next action
                  metaFile.ignore( STD_LINE_LEN, ';' );
                  metaFile.ignore( SPACE_BUF, ' ' ); 

                  // get next action
                  metaFile.get( tempType );                  
               }

               // end action
               if( tempType == 'A' )
               {
                  // queue start action
                  tempAction.actionType = tempType;
                  tempAction.actionDescriptor = "end";
                  tempAction.actionCycle = 0;
                  process.actions.push_back( tempAction );

                  // move to next action
                  metaFile.ignore( STD_LINE_LEN, ';' );
                  metaFile.ignore( SPACE_BUF, ' ' ); 

                  // get next action
                  metaFile.get( tempType );                  

                  // increment successful process save
                  processCount++;                  
               }
            }

            // unrecognized start action
            else
            {
               // return no saved processes
               return 0;
            }
         }

   // close meta-data file
   metaFile.close();

   // return process count - will only be 1 for now
   return processCount;
}

/**
threadActions

Creates threads for each action
The start simulation action is executed, then processes actions in
process control block in order of arrival from meta-data file. Lastly,
the end simulation action is executed. Each action creates a thread
and calls the runner function. Each thread is joined to previous threads
upon exit. 

@param process process control block with process information
@pre valid config information and meta-data was saved
@post each action in the PCB was executed via a thread
@return void 
*/
void threadActions( PCB& process )
{
   // initialize variables
   int tidCount = process.actions.size() + 2; // number of actions in queue plus system start/end
   pthread_t tids[ tidCount ]; // an array of threads to be joined
   pthread_attr_t attr;
   void* actVoidPtr;
   int tidIndex = 0;

   // create start and end actions
   Action startAction('S', "start", 0);
   Action endAction( 'S', "end", 0);

   // set time output      
   timer = clock();   

   // get the default attributes
   pthread_attr_init(&attr);

   // thread each action

      // thread system start
      actVoidPtr = &startAction;
      pthread_create( &( tids[ tidIndex ] ), &attr, runner, actVoidPtr );
      pthread_join( tids[ tidIndex ], NULL );
      tidIndex++;

      // thread each action in action queue
      while( !( process.actions.empty() ) )
      {
         // get first item in queue
         actVoidPtr = &(process.actions.front());

         // create thread and run action
         pthread_create( &( tids[ tidIndex ] ), &attr, runner, actVoidPtr );
         pthread_join( tids[ tidIndex ], NULL );

         // increment to next thread
         tidIndex++;

         // remove completed action
         process.actions.pop_front();
      }

      // thread end system
      actVoidPtr = &endAction;
      pthread_create( &( tids[ tidIndex ] ), &attr, runner, actVoidPtr ); 
      pthread_join( tids[ tidIndex ], NULL );          
}

/**
runner

Simulates execution of action.
Each thread calls this function upon its creation in threadActions(). 
Each thread prints appropriate information by calling printAction(),
waits until it has completed the appropriate cycle time and then ends.

@param void pointer to the current action
@pre a valid action is given
@post action is completed and logged
@return void
*/
void *runner( void* actVoidPtr )
{
   // initialize variables

      // set void pointer to Action pointer
      Action* actPtr = static_cast<Action*>( actVoidPtr );

      // prepare output string
      stringstream actionOutput;
      actionOutput.precision( PRECISION );
      actionOutput << fixed; 

      // action start time
      float floatTime;
      float runTime;

      // action run duration 
      runTime = actPtr->actionCycle * (cycleData[ actPtr->actionDescriptor ]) * CYCLE_TO_MS;

   // system action type
   if( actPtr->actionType == 'S' )
   {
      // start system descriptor
      if( actPtr->actionDescriptor == "start")
      {
         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "Simulator program starting \n";
         printAction( actionOutput );
      }

      // end system descriptor
      else
      {
         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "Simulator program ending \n";
         printAction( actionOutput );
      }

   }

   // application action type
   else if( actPtr->actionType == 'A' )
   {
      // start application descriptor
      if( actPtr->actionDescriptor == "start")
      {
         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: preparing process 1 \n";
         printAction( actionOutput );

         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: starting process 1 \n";   
         printAction( actionOutput );
      }      

      // end application descriptor
      else
      {
         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: removing process 1 \n";    
         printAction( actionOutput );     
      }
       
   }

   // process action type
   else if( actPtr->actionType == 'P' )
   {
      // set current time
      floatTime = ((float)(clock() - timer)/CLOCKS_PER_SEC);

      // run application descriptor start
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: start processing action \n"; 
      printAction( actionOutput );

      // run for specified time
      while( ((float)(clock()-timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // run application desciptor end 
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: end processing action \n";
      printAction( actionOutput );      
   }

   // input action type
   else if( actPtr->actionType == 'I' )
   {
      // set current time
      floatTime = ((float)(clock() - timer)/CLOCKS_PER_SEC);

      // input descriptor start
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: start " << actPtr->actionDescriptor << " input \n"; 
      printAction( actionOutput );

      // run for specified time
      while( ((float)(clock()-timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // input desciptor end 
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: end " << actPtr->actionDescriptor << " input \n";
      printAction( actionOutput );
   }

   // output action type
   else if( actPtr->actionType == 'O' )
   {
      // set current time
      floatTime = ((float)(clock() - timer)/CLOCKS_PER_SEC);

      // output descriptor start
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: start " << actPtr->actionDescriptor << " output \n"; 
      printAction( actionOutput );
      
      // run for specified time
      while( ((float)(clock()-timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // output desciptor end 
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: end " << actPtr->actionDescriptor << " output \n";
      printAction( actionOutput );
   }   

   // unrecognized action type
   else 
   {
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: unrecognized action \n";
      printAction( actionOutput );
   }   

   // end thread
   pthread_exit(0);
}

/**
printAction

Prints information for action.
Logs the passed string stream to appropriate location(s) and 
clears the string stream. 

@param actionOutput stringstream holds information to be logged
@pre none
@post actionOutput was logged appropriately 
@return none
*/
void printAction( stringstream& actionOutput )
{
   // print to console
   if( logLocation == 'M' || logLocation == 'B' )
   {
      cout << actionOutput.str();
   }

   // print to file
   if( logLocation == 'F' || logLocation == 'B' )
   {
      outputFile << actionOutput.str();
   }

   // clear string stream
   actionOutput.str( string() );
   actionOutput.clear();
}

