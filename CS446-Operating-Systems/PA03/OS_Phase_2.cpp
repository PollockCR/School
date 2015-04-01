/**
 * @file   OS_Phase_2.cpp
 * @author 831835
 * @date   April 2015
 * @brief  Batch program operating system simulator
 *
 * Operating system simulator with four states (Enter/Start, 
 * Ready, Running, Exit). It will accept meta-data for several programs, 
 * run programs one at a time, and end the simulation. Each unique action
 * within the processes is conducted with its own unique thread. 
 * Processes ran with FIFO, SJF, or SRTF scheduling, according to
 * configuration file. 
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
   #include <list> // used to store processes
   #include <queue> // used to store process actions
   #include <algorithm> // used for finding min
   #include <pthread.h> // used for threads

   using namespace std;

// global constants
   const int STD_STR_LEN = 45;
   const int STD_LINE_LEN = 256;
   const int SPACE_BUF = 1;
   const int OP_COUNT = 5;
   const int PRECISION = 6;
   const float CYCLE_TO_MS = 0.001;
   const float VERSION_MIN = 0.0;

// global variables
   clock_t timer; // keeps track of time
   map<string, float> cycleData; // cycles per action
   char logLocation; // location to log to
   ofstream outputFile; // output file stream
   string schedulingCode; // scheduling code 

// function prototypes
   bool saveConfig( char* configPath, float &phase, char* metaPath, char* logPath );
   int saveMetaData( char* metaPath, list<PCB>& processes );
   void runProcesses( list<PCB>& processes );
   bool compareRuntimes( const PCB& first, const PCB& second );  
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
   list<PCB> processes;

   // STATE: Enter/Start

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
      processCount = saveMetaData( metaPath, processes );

         // check for invalid meta-data
         if( processCount < 0 )
         {
            // return failure
            return 1;            
         }

   // STATE: Running

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
      runProcesses( processes );

      // for file output
      if( logLocation != 'M' )
      {
         // close file
         outputFile.close();      
      }

   // STATE: Exit

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
   string tempPath;

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

         // check for invalid version
         if( phase < VERSION_MIN )
         {
            // return failure
            return false;
         }

      // read in meta-data filepath
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile.ignore( SPACE_BUF, ' ' );
      configFile.getline( metaPath, STD_LINE_LEN );

      // read in scheduling code 
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile.ignore( SPACE_BUF, ' ' );
      configFile >> schedulingCode;  

         // check for invalid scheduling code
         if( schedulingCode != "FIFO" && schedulingCode != "SJF" && schedulingCode != "SRTF")
         {
            // return failure
            return false;
         }

      // read in cycle data

         // read in processor cycle time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;

            // check for valid time
            if( tempTime >= 0 )
            {
               cycleData[ "run" ] = tempTime;
            }

         // read in monitor display time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;

            // check for valid time
            if( tempTime >= 0 )
            {
               cycleData[ "monitor" ] = tempTime;
            }

         // read in hard drive cycle time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;

            // check for valid time
            if( tempTime >= 0 )
            {
               cycleData[ "hard drive" ] = tempTime;
            }
        
         // read in printer cycle time 
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;

            // check for valid time
            if( tempTime >= 0 )
            {
               cycleData[ "printer" ] = tempTime;
            }
        
         // read in keyboard cycle time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;

            // check for valid time
            if( tempTime >= 0 )
            {
               cycleData[ "keyboard" ] = tempTime;
            }
         
      // ignore until after "Log to "
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile.ignore( STD_LINE_LEN, ' ' );
      configFile.ignore( STD_LINE_LEN, ' ' );
      configFile.ignore( STD_LINE_LEN, ' ' );
      
      // read in log location
      configFile >> logLocation;

         // check for unrecognized location
         if( logLocation != 'M' && logLocation != 'B' && logLocation != 'F' )
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
appropriate location. Each process is added to a process queue 
and actions within each process are sent into a process control
block and into a queue within the PCB. The number of seperate
process programs is returned. An error results in -1 being returned.

@param metaPath holds path to meta-data file for saving later
@param process process control block queue with process information
@pre valid config information was saved
@post configuration information saved or error returned
@return int representing number of processes saved
*/
int saveMetaData( char* metaPath, list<PCB>& processes )
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
         // print failure
         cout << "Error in meta-data file. Please try again." << endl;

         // return invalid meta-data
         return -1;
      }   

   // read in meta-data

      // read in start
      metaFile.ignore( STD_LINE_LEN, '\n' );
      
         // get OS start action
         metaFile.get( tempType );

         // check for invalid OS start
         if( tempType != 'S' )
         {
            // print failure
            cout << "Error in meta-data. OS start failure. Please try again." << endl;      

            // return invalid meta-data
            return -1;
         }

         // skip first line
         metaFile.ignore( STD_LINE_LEN, ';' );
         metaFile.ignore( SPACE_BUF, ' ' );    

      // get process start action
      metaFile.get( tempType );

      // add processes until end of file or end simulation command
      while( metaFile.good() && tempType != 'S' )
      {
         // recognized start action
         if( tempType == 'A' )
         {
            // create process

               // create process with pid equal to process number
               PCB *tempProcess = new PCB( processCount+1 );

               // set process to enter state 
               tempProcess->state = "Enter";

               // queue start action
               tempAction.actionType = tempType;
               tempAction.actionDescriptor = "start";
               tempAction.actionCycle = 0;
               tempAction.pcbID = processCount + 1;
               tempProcess->actions.push( tempAction );

            // move to next action
            metaFile.ignore( STD_LINE_LEN, ';' );
            metaFile.ignore( SPACE_BUF, ' ' ); 

            // get next action
            metaFile.get( tempType );

               // ignore space
               if( tempType == ' ' || tempType == '\n' )
               {
                  metaFile.get( tempType );
               }             

            // until end of action
            while( tempType != 'A' )
            {
               // recognized actions
               if( tempType == 'P' || tempType == 'I' || tempType == 'O' )
               {
                  // save meta-data for process action
                  metaFile.ignore( SPACE_BUF, '(' );
                  getline( metaFile, tempDescriptor, ')' );
                  metaFile >> tempCycle; 

                     // check for invalid action data (cycle time or descriptor)
                     if( tempCycle < 0 )
                     {
                        // print failure
                        cout << "Error in meta-data. Unrecognized action cycle count found. Please try again." << endl;

                        // return no saved processes
                        return -1;                        
                     } 
                     if( tempDescriptor != "monitor" && tempDescriptor != "keyboard" &&
                         tempDescriptor != "run" && tempDescriptor != "hard drive" &&
                         tempDescriptor != "printer" )
                     {
                        // print failure
                        cout << "Error in meta-data. Unrecognized action descriptor found. Please try again." << endl;

                        // return no saved processes
                        return -1;                        
                     }

                  // save data to temp process               
                  tempAction.actionType = tempType;
                  tempAction.actionDescriptor = tempDescriptor;
                  tempAction.actionCycle = tempCycle;
                  tempAction.pcbID = processCount + 1;

                  // queue process action 
                  tempProcess->actions.push( tempAction ); 

                  // update estimated runtime
                  tempProcess->estimatedRuntime += ( tempCycle * cycleData[ tempDescriptor ] );
               }

               // unrecognized action type
               else 
               {
                  // print failure
                  cout << "Error in meta-data. Unrecognized action found. Please try again." << endl;

                  // return no saved processes
                  return -1;
               }

               // move to next action
               metaFile.ignore( STD_LINE_LEN, ';' );
               metaFile.ignore( SPACE_BUF, ' ' ); 

               // save next action
               metaFile.get( tempType );  

                  // ignore space
                  if( tempType == ' ' || tempType == '\n' )
                  {
                     metaFile.get( tempType );
                  }                                
            }

            // end action
            if( tempType == 'A' )
            {
               // queue end action to process
               tempAction.actionType = tempType;
               tempAction.actionDescriptor = "end";
               tempAction.actionCycle = 0;
               tempAction.pcbID = processCount + 1;
               tempProcess->actions.push( tempAction );

               // increment successful process save
               processCount++;  

               // set process to ready state 
               tempProcess->state = "Ready";

               // add process to list
               processes.push_back( *tempProcess );

                  // delete temporary process
                  delete tempProcess;
                  tempProcess = '\0';               

               // move to next action
               metaFile.ignore( STD_LINE_LEN, ';' );
               metaFile.ignore( SPACE_BUF, ' ' ); 

               // get next action
               metaFile.get( tempType );   

                  // ignore space
                  if( tempType == ' ' || tempType == '\n' )
                  {
                     metaFile.get( tempType );
                  } 
            }
         }

         // unrecognized start action
         else
         {
            // print failure
            cout << "Error in meta-data. Process start/end failure. Please try again." << endl;

            // return no saved processes
            return -1;
         }
      }

      // check for invalid end action
      if( tempType != 'S' )
      {
         // print failure
         cout << "Error in meta-data. OS end failure. Please try again." << endl;      

         // return invalid simulation end
         return -1;
      }

   // close meta-data file
   metaFile.close();

   // return process count
   return processCount;
}

/**
runProcesses

Runs queue of processes according to given scheduling code. 
Can run in either FIFO, SJF, or STRF modes. Each process is
selected and passed to threadActions to be executed. Output
is printed with printAction for each process. 

@param processes a list of process control blocks to be executed
@pre valid config information and meta-data was saved
@post each PCB in the list was executed according to scheduling code
@return void 
*/
void runProcesses( list<PCB>& processes )
{
   // initialize variables
   list<PCB>::iterator minProcess;

      // prepare output string
      stringstream actionOutput;
      actionOutput.precision( PRECISION );
      actionOutput << fixed; 

      // set time output      
      timer = clock(); 

   // start simulator
   actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
   actionOutput << " - " << "Simulator program starting \n";
   printAction( actionOutput ); 

   // output preparing processes
   actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
   actionOutput << " - " << "OS: preparing all processes \n";
   printAction( actionOutput );      

   // enter appropriate scheduling code case

      // first-in first-out scheduling (fifo)
      if( schedulingCode == "FIFO" )
      {
         // dequeue each process and run
         while( !processes.empty() )
         {
            // prepare processes
            actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
            actionOutput << " - " << "OS: selecting next process \n";
            printAction( actionOutput );    

            // change process to running state
            (processes.front()).state = "Running";

            // execute process
            threadActions( *(processes.begin()) );

            // change process to exit state
            (processes.front()).state = "Exit";            

            // remove process
            processes.pop_front();            
         }
      }

      // shortest job first scheduling (sjf)
      if( schedulingCode == "SJF" )
      {
         // sort all processes according to runtime
         processes.sort( compareRuntimes );

         // dequeue each process and run
         while( !processes.empty() )
         {
            // output preparing processes
            actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
            actionOutput << " - " << "OS: selecting next process \n";
            printAction( actionOutput ); 

            // change process to running state
            (processes.front()).state = "Running";            

            // execute process
            threadActions( *(processes.begin()) );

            // change process to exit state
            (processes.front()).state = "Exit";   

            // remove process
            processes.pop_front();
         }         
      }      

      // shortest remaining time first scheduling (srtf)
      if( schedulingCode == "SRTF" )
      {
         // dequeue each process and run
         while( !processes.empty() )
         {
            // output preparing processes
            actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
            actionOutput << " - " << "OS: selecting next process \n";
            printAction( actionOutput );             

            // run process

               // check for > 1 process
               if( processes.size() > 1 )
               {
                  // find minimum process
                  minProcess = min_element( processes.begin(), processes.end(), compareRuntimes );
               }

               // for one process
               else
               {
                  // set process to execute
                  minProcess = processes.begin();
               }

               // change process to running state
               minProcess->state = "Running";                
             
               // execute process
               threadActions( *minProcess );

            // change process to exit state
            minProcess->state = "Exit";           

            // remove process
            processes.erase( minProcess );
         }          
      }   

   // end simulator
   actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
   actionOutput << " - " << "Simulator program ending \n";
   printAction( actionOutput );
}

/**
compareRuntimes

Compares estimated run times of two processes and returns
true if first process has lower (or equal) runtime as process
two, false otherwise. 

@param first process to compare
@param second process to compare
@pre processes have been created and have estimated runtimes
@post processes' estimated runtimes were compared
@return bool indicating run time was lower for first process 
*/
bool compareRuntimes( const PCB& first, const PCB& second )
{
   // return true if first PCB has lower (or same) estimated runtime
   // than second PCB, false otherwise
   return( first.estimatedRuntime <= second.estimatedRuntime );
}

/**
threadActions

Runs one process and creates threads for each action
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
   int tidCount = process.actions.size(); // number of actions in queue
   pthread_t tids[ tidCount ]; // an array of threads to be joined
   pthread_attr_t attr;
   void* actVoidPtr;
   int tidIndex = 0;  

   // get the default attributes
   pthread_attr_init(&attr);

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
      process.actions.pop();
   }        
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

   // application action type
   if( actPtr->actionType == 'A' )
   {
      // start application descriptor
      if( actPtr->actionDescriptor == "start")
      {
         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: starting process " << actPtr -> pcbID << endl;   
         printAction( actionOutput );
      }      

      // end application descriptor
      else
      {
         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: removing process " << actPtr -> pcbID << endl; 
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
      actionOutput << " - " << "Process " << actPtr -> pcbID << ": start processing action \n"; 
      printAction( actionOutput );

      // run for specified time
      while( ((float)(clock()-timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // run application desciptor end 
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << actPtr -> pcbID << ": end processing action \n";
      printAction( actionOutput );      
   }

   // input action type
   else if( actPtr->actionType == 'I' )
   {
      // set current time
      floatTime = ((float)(clock() - timer)/CLOCKS_PER_SEC);

      // input descriptor start
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << actPtr -> pcbID << ": start " << actPtr->actionDescriptor << " input \n"; 
      printAction( actionOutput );

      // run for specified time
      while( ((float)(clock()-timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // input desciptor end 
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << actPtr -> pcbID << ": end " << actPtr->actionDescriptor << " input \n";
      printAction( actionOutput );
   }

   // output action type
   else if( actPtr->actionType == 'O' )
   {
      // set current time
      floatTime = ((float)(clock() - timer)/CLOCKS_PER_SEC);

      // output descriptor start
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << actPtr -> pcbID << ": start " << actPtr->actionDescriptor << " output \n"; 
      printAction( actionOutput );
      
      // run for specified time
      while( ((float)(clock()-timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // output desciptor end 
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << actPtr -> pcbID << ": end " << actPtr->actionDescriptor << " output \n";
      printAction( actionOutput );
   }   

   // unrecognized action type
   else 
   {
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << actPtr -> pcbID << ": unrecognized action \n";
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

