/**
 * @file   OS_Phase_3.cpp
 * @author 831835
 * @date   April 2015
 * @brief  Multi-program operating system simulator
 *
 * Operating system simulator with five states (Enter/Start, 
 * Ready, Running, Blocked/Waiting\, Exit). It will accept meta-data for
 * several programs, run the programs concurrently using a multi-programming
 * strategy, and end the simulation. When each I/O process is called (i.e. 
 * the I/O thread is deployed), the running process must be placed in the 
 * Blocked/Waiting state. When the I/O processhas completed, it must
 * interrupt the processor at the end of the presently running process
 * cycle, causing the OS to move the process back into its appropriate
 * place in the Ready state.
 * Processes run with RR, FIFO-P, or SRTF-P scheduling, according to
 * configuration file. It also runs with previously impemented scheduling codes
 * FIFO, SJF, and SRTF, but time quantum is still required in config file. 
 */

// header files
   #include "Process_Control_Block.h" 
   #include "OS_Control_Block.h"
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

// function prototypes

   // save data functions
   bool saveConfig( char* configPath, OSCB &simulatorConfig );
   int saveMetaData( OSCB &simulatorConfig, list<PCB>& processes );

   // process simulating functions
   void runProcesses( OSCB &simulatorConfig, list<PCB>& processes ); 
   void threadActions( OSCB &simulatorConfig, PCB& process );
   float threadActionQuantum( OSCB &simulatorConfig, list<PCB> &runningQueue );
   void *runner( void *simulatorVoidPtr ); 
   void *quantumRunner( void* simulatorVoidPtr );  

   // helper functions
   void printAction( OSCB* simulatorPtr, stringstream& actionOutput ); 
   bool compareRuntimes( const PCB& first, const PCB& second );    

// main program
int main( int argc, char* argv[] )
{
   // initialize variables
   OSCB simulatorConfig;
   bool saveSucess;
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
         saveSucess = saveConfig( argv[1], simulatorConfig );

         // check for failed save
         if( !saveSucess )
         {
            // print failure
            cout << "Error in configuration file. Please try again." << endl;

            // return failure
            return 1;
         }

      // read in meta-data
      processCount = saveMetaData( simulatorConfig, processes );

         // check for invalid meta-data
         if( processCount < 0 )
         {
            // return failure
            return 1;            
         }

   // STATE: Running

      // prepare for output 

         // for file output 
         if( simulatorConfig.logLocation != 'M' )
         {
            // open log file 
            simulatorConfig.outputFile.open( simulatorConfig.logPath, ofstream::out | ofstream::trunc );

            // check for valid file
            if( !simulatorConfig.outputFile.is_open() )
            {
               // print failure 
               cout << "Error in log file. Please try again." << endl;

               // return failure
               return 1;
            }
         }

      // run simulator
      runProcesses( simulatorConfig, processes );

      // for file output
      if( simulatorConfig.logLocation != 'M' )
      {
         // close file
         simulatorConfig.outputFile.close();      
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
@param simulatorConfig reference to OSCB object holding all config info
@pre configPath was passed to program
@post configuration information saved or error returned
@return bool representing sucessful saving of configuration
*/
bool saveConfig( char* configPath, OSCB &simulatorConfig )
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
      configFile >> simulatorConfig.phase;

         // check for invalid version
         if( simulatorConfig.phase < VERSION_MIN )
         {
            // return failure
            return false;
         }

      // read in meta-data filepath
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile.ignore( SPACE_BUF, ' ' );
      configFile >> simulatorConfig.metaPath;

      // read in scheduling code 
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile.ignore( SPACE_BUF, ' ' );
      configFile >> simulatorConfig.schedulingCode;  

         // check for invalid scheduling code
         if( simulatorConfig.schedulingCode != "FIFO-P" && simulatorConfig.schedulingCode != "RR"
             && simulatorConfig.schedulingCode != "SRTF-P" && simulatorConfig.schedulingCode != "SRTF"
             && simulatorConfig.schedulingCode != "SJF" && simulatorConfig.schedulingCode != "FIFO" )
         {
            // return failure
            return false;
         }

      // read in quantum time
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile.ignore( SPACE_BUF, ' ' );
      configFile >> simulatorConfig.quantum;  

         // check for invalid quantum time
         if( simulatorConfig.quantum <= 0 )
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
               simulatorConfig.cycleData[ "run" ] = tempTime;
            }

         // read in monitor display time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;

            // check for valid time
            if( tempTime >= 0 )
            {
               simulatorConfig.cycleData[ "monitor" ] = tempTime;
            }

         // read in hard drive cycle time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;

            // check for valid time
            if( tempTime >= 0 )
            {
               simulatorConfig.cycleData[ "hard drive" ] = tempTime;
            }
        
         // read in printer cycle time 
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;

            // check for valid time
            if( tempTime >= 0 )
            {
               simulatorConfig.cycleData[ "printer" ] = tempTime;
            }
        
         // read in keyboard cycle time
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile >> tempTime;

            // check for valid time
            if( tempTime >= 0 )
            {
               simulatorConfig.cycleData[ "keyboard" ] = tempTime;
            }
         
      // ignore until after "Log to "
      configFile.ignore( STD_LINE_LEN, ':' );
      configFile.ignore( STD_LINE_LEN, ' ' );
      configFile.ignore( STD_LINE_LEN, ' ' );
      configFile.ignore( STD_LINE_LEN, ' ' );
      
      // read in log location
      configFile >> simulatorConfig.logLocation;

         // check for unrecognized location
         if( simulatorConfig.logLocation != 'M' && simulatorConfig.logLocation != 'B' && simulatorConfig.logLocation != 'F' )
         { 
            return false;
         }

      // read in log file path
      if( simulatorConfig.logLocation  != 'M' )
      {
         configFile.ignore( STD_LINE_LEN, '\n' );
         configFile.ignore( STD_LINE_LEN, ':' );
         configFile.ignore( SPACE_BUF, ' ' );
         configFile >> simulatorConfig.logPath;
      }

   // close file
   configFile.close();

   // check for save failure
   if( simulatorConfig.cycleData.size() != OP_COUNT || configFile.eof() )
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

@param simulatorConfig holds all configuration info to use, including meta-data info
@param process process control block queue with process information
@pre valid config information was saved
@post configuration information saved or error returned
@return int representing number of processes saved
*/
int saveMetaData( OSCB &simulatorConfig, list<PCB>& processes )
{
   // initialize variables
   int processCount = 0;
   ifstream metaFile;
   char tempType;
   string tempDescriptor;
   int tempCycle;
   Action tempAction;

   // open meta-data file
   metaFile.open( simulatorConfig.metaPath, ifstream::in );
         
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
                  tempProcess->estimatedRuntime += ( tempCycle * simulatorConfig.cycleData[ tempDescriptor ] );
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
Can run in either FIFO-P, RR, SRTF-P, FIFO, SJF, or STRF modes.
Each process is selected and passed to threadActions or threadActionsQuantum
to be executed, depending on if scheduling method is preemptive or not. 
Output is printed with printAction for each process. 

@param simulatorConfig a reference to an OSCB object with current action & configuration info
@param processes a list of process control blocks to be executed
@pre valid config information and meta-data was saved
@post each PCB in the list was executed according to scheduling code
@return void 
*/
void runProcesses( OSCB &simulatorConfig, list<PCB>& processes )
{
   // initialize variables
   list<PCB>::iterator minProcess;
   list<PCB> runningQueue;
   OSCB* simulatorPtr = &simulatorConfig;
   float remainingActions;

      // prepare output string
      stringstream actionOutput;
      actionOutput.precision( PRECISION );
      actionOutput << fixed; 

      // set time output      
      simulatorConfig.timer = clock(); 

   // start simulator
   actionOutput << ((float)(clock()-simulatorConfig.timer)/CLOCKS_PER_SEC);
   actionOutput << " - " << "Simulator program starting \n";
   printAction( simulatorPtr, actionOutput ); 

   // output preparing processes
   actionOutput << ((float)(clock()-simulatorConfig.timer)/CLOCKS_PER_SEC);
   actionOutput << " - " << "OS: preparing all processes \n";
   printAction( simulatorPtr, actionOutput );      

   // enter appropriate scheduling code case
   // (includes both required cases and previously implemented)

      // first-in first-out preemptive scheduling (fifo-p)
      if( simulatorConfig.schedulingCode == "FIFO-P" )
      {
         // dequeue each process and run
         while( !processes.empty() )
         {
            // prepare processes
            actionOutput << ((float)(clock()-simulatorConfig.timer)/CLOCKS_PER_SEC);
            actionOutput << " - " << "OS: selecting next process \n";
            printAction( simulatorPtr, actionOutput );  

            // move process into Running/Blocked queue  
            runningQueue.push_front( *(processes.begin()) );

            // change process to running state
            runningQueue.front().state = "Running";

            // execute process
            remainingActions = threadActionQuantum( simulatorConfig, runningQueue );

            // check state

               // remove process from Ready queue
               processes.pop_front();

               // check for blocked
               if( remainingActions > 0 )
               {
                  // changed to blocked state
                  runningQueue.front().state = "Blocked";

                  // insert back into Ready queue (at front because priority queue)
                  processes.push_front( *(runningQueue.begin()) );
               }

               // check for completion
               else 
               {
                  // change process to exit state
                  runningQueue.front().state = "Exit"; 
               }        

            // remove process from Running/Blocked queue
            runningQueue.pop_front();  
         }
      }

      // round robin scheduling (rr)
      if( simulatorConfig.schedulingCode == "RR" )
      {
         // dequeue each process and run
         while( !processes.empty() )
         {
            // prepare processes
            actionOutput << ((float)(clock()-simulatorConfig.timer)/CLOCKS_PER_SEC);
            actionOutput << " - " << "OS: selecting next process \n";
            printAction( simulatorPtr, actionOutput );  

            // move process into Running/Blocked queue  
            runningQueue.push_front( *(processes.begin()) );

            // change process to running state
            runningQueue.front().state = "Running";

            // execute process
            remainingActions = threadActionQuantum( simulatorConfig, runningQueue );

            // remove process from Ready queue
            processes.pop_front();

            // check state

               // check for blocked
               if( remainingActions > 0 )
               {
                  // changed to blocked state
                  runningQueue.front().state = "Blocked";

                  // insert back into Ready queue (at end because queue)
                  processes.push_back( *(runningQueue.begin()) );
               }

               // check for completion
               else 
               {
                  // change process to exit state
                  runningQueue.front().state = "Exit"; 
               }        

            // remove process from Running/Blocked queue
            runningQueue.pop_front();  
         }
      }      

      // shortest remaining time first preemptive (srtf-p)
      if( simulatorConfig.schedulingCode == "SRTF-P" )
      {
         // dequeue each process and run
         while( !processes.empty() )
         {
            // prepare processes
            actionOutput << ((float)(clock()-simulatorConfig.timer)/CLOCKS_PER_SEC);
            actionOutput << " - " << "OS: selecting next process \n";
            printAction( simulatorPtr, actionOutput );  

            // select next process

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

            // move process into Running/Blocked queue  
            runningQueue.push_front( *minProcess );

            // change process to running state
            runningQueue.front().state = "Running";

            // execute process
            remainingActions = threadActionQuantum( simulatorConfig, runningQueue );

            // check state

               // remove process from Ready queue
               processes.erase( minProcess );

               // check for blocked
               if( remainingActions > 0 )
               {
                  // changed to blocked state
                  runningQueue.front().state = "Blocked";

                  // insert back into Ready queue 
                  processes.push_back( *(runningQueue.begin()) );
               }

               // check for completion
               else 
               {
                  // change process to exit state
                  runningQueue.front().state = "Exit"; 
               }        

            // remove process from Running/Blocked queue
            runningQueue.pop_front();  
         }
      }      

      // first-in first-out scheduling (fifo)
      if( simulatorConfig.schedulingCode == "FIFO" )
      {
         // dequeue each process and run
         while( !processes.empty() )
         {
            // prepare processes
            actionOutput << ((float)(clock()-simulatorConfig.timer)/CLOCKS_PER_SEC);
            actionOutput << " - " << "OS: selecting next process \n";
            printAction( simulatorPtr, actionOutput );    

            // change process to running state
            (processes.front()).state = "Running";

            // execute process
            threadActions( simulatorConfig, *(processes.begin()) );

            // change process to exit state
            (processes.front()).state = "Exit";            

            // remove process
            processes.pop_front();            
         }
      }

      // shortest job first scheduling (sjf)
      if( simulatorConfig.schedulingCode == "SJF" )
      {
         // sort all processes according to runtime
         processes.sort( compareRuntimes );

         // dequeue each process and run
         while( !processes.empty() )
         {
            // output preparing processes
            actionOutput << ((float)(clock()-simulatorConfig.timer)/CLOCKS_PER_SEC);
            actionOutput << " - " << "OS: selecting next process \n";
            printAction( simulatorPtr, actionOutput ); 

            // change process to running state
            (processes.front()).state = "Running";            

            // execute process
            threadActions( simulatorConfig, *(processes.begin()) );

            // change process to exit state
            (processes.front()).state = "Exit";   

            // remove process
            processes.pop_front();
         }         
      }      

      // shortest remaining time first scheduling (srtf)
      if( simulatorConfig.schedulingCode == "SRTF" )
      {
         // dequeue each process and run
         while( !processes.empty() )
         {
            // output preparing processes
            actionOutput << ((float)(clock()-simulatorConfig.timer)/CLOCKS_PER_SEC);
            actionOutput << " - " << "OS: selecting next process \n";
            printAction( simulatorPtr, actionOutput );             

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
               threadActions( simulatorConfig, *minProcess );

            // change process to exit state
            minProcess->state = "Exit";           

            // remove process
            processes.erase( minProcess );
         }          
      }   

   // end simulator
   actionOutput << ((float)(clock()-simulatorConfig.timer)/CLOCKS_PER_SEC);
   actionOutput << " - " << "Simulator program ending \n";
   printAction( simulatorPtr, actionOutput );
}

/**
compareRuntimes

Compares estimated remaining run times of two processes and returns
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
void threadActions( OSCB &simulatorConfig, PCB& process )
{
   // initialize variables
   int tidCount = process.actions.size(); // number of actions in queue
   pthread_t tids[ tidCount ]; // an array of threads to be joined
   pthread_attr_t attr;
   void* simulatorVoidPtr;
   int tidIndex = 0;  

   // get the default attributes
   pthread_attr_init(&attr);

   // thread each action in action queue
   while( !( process.actions.empty() ) )
   {
      // get first action in queue
      simulatorConfig.currentAction = &(process.actions.front());

      // save void pointer
      simulatorVoidPtr = &(simulatorConfig);

      // create thread and run action
      pthread_create( &( tids[ tidIndex ] ), &attr, runner, simulatorVoidPtr );
      pthread_join( tids[ tidIndex ], NULL );

      // increment to next thread
      tidIndex++;

      // remove completed action
      process.actions.pop();
   }        
}

/**
threadActionsQuantum

Creates a thread for an action of current process and runs for specified time quantum.
After time quantum, process is Blocked and OS returns to runProcesses to select another process.
The start simulation action is executed, then processes actions in process control block in order of
arrival from meta-data file. Current process was already selected in runProcesses according to
scheduling code. Lastly, the end simulation action is executed if that action has completed.
Each action creates a thread and calls the quantumRunner function. Each thread is joined to previous threads
upon completion, and blocked if incomplete. 

@param simulatorConfig reference to OSCB object with configuration information
@param runningQueue a list of PCB objects that holds all currently running proceeses
@pre valid config information and meta-data was saved
@post each action in the PCB was executed via a thread
@return float with remaining number of actions in running process 
*/
float threadActionQuantum( OSCB &simulatorConfig, list<PCB> &runningQueue )
{
   // initialize variables
   int tidCount = runningQueue.front().actions.size(); // number of actions in queue
   pthread_t tids[ tidCount ]; // an array of threads to be joined
   pthread_attr_t attr;
   void* simulatorVoidPtr;
   int tidIndex = 0;  

   // get the default attributes
   pthread_attr_init(&attr);

   // thread each action in action queue
   if( !( runningQueue.front().actions.empty() ) )
   {
      // get first action in queue
      simulatorConfig.currentAction = &(runningQueue.front().actions.front());

      // update remaining estimated process run time (for SRTF-P)
      runningQueue.front().estimatedRuntime -= (simulatorConfig.cycleData[ simulatorConfig.currentAction->actionDescriptor ] * simulatorConfig.quantum );      

      // save void pointer
      simulatorVoidPtr = &(simulatorConfig);

      // create thread and run action
      pthread_create( &( tids[ tidIndex ] ), &attr, quantumRunner, simulatorVoidPtr );
      pthread_join( tids[ tidIndex ], NULL );

      // increment to next thread
      tidIndex++;

      // check for action completion 
      if( simulatorConfig.currentAction->actionCycle <= 0 )
      {
         // remove completed action
         runningQueue.front().actions.pop();            
      }
   } 
   
   // return number of remaining actions in process
   return runningQueue.front().actions.size();
       
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
void *runner( void* simulatorVoidPtr )
{
   // initialize variables

      // set void pointer to Action pointer
      OSCB* simulatorPtr = static_cast<OSCB*>( simulatorVoidPtr );

      // prepare output string
      stringstream actionOutput;
      actionOutput.precision( PRECISION );
      actionOutput << fixed; 

      // action start time
      float floatTime;
      float runTime;

      // action run duration 
      runTime = simulatorPtr->currentAction->actionCycle * simulatorPtr->cycleData[ simulatorPtr->currentAction->actionDescriptor ] * CYCLE_TO_MS;

   // application action type
   if( simulatorPtr->currentAction->actionType == 'A' )
   {
      // start application descriptor
      if( simulatorPtr->currentAction->actionDescriptor == "start")
      {
         actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: starting process " << simulatorPtr->currentAction->pcbID << endl;   
         printAction( simulatorPtr, actionOutput );
      }      

      // end application descriptor
      else
      {
         actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: removing process " << simulatorPtr->currentAction->pcbID << endl; 
         printAction( simulatorPtr, actionOutput );     
      }
       
   }

   // process action type
   else if( simulatorPtr->currentAction->actionType == 'P' )
   {
      // set current time
      floatTime = ((float)(clock() - simulatorPtr->timer)/CLOCKS_PER_SEC);

      // run application descriptor start
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": start processing action \n"; 
      printAction( simulatorPtr, actionOutput );

      // run for specified time
      while( ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // run application desciptor end 
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": end processing action \n";
      printAction( simulatorPtr, actionOutput );      
   }

   // input action type
   else if( simulatorPtr->currentAction->actionType == 'I' )
   {
      // set current time
      floatTime = ((float)(clock() - simulatorPtr->timer)/CLOCKS_PER_SEC);

      // input descriptor start
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": start " << simulatorPtr->currentAction->actionDescriptor << " input \n"; 
      printAction( simulatorPtr, actionOutput );

      // run for specified time
      while( ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // input desciptor end 
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": end " << simulatorPtr->currentAction->actionDescriptor << " input \n";
      printAction( simulatorPtr, actionOutput );
   }

   // output action type
   else if( simulatorPtr->currentAction->actionType == 'O' )
   {
      // set current time
      floatTime = ((float)(clock() - simulatorPtr->timer)/CLOCKS_PER_SEC);

      // output descriptor start
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": start " << simulatorPtr->currentAction->actionDescriptor << " output \n"; 
      printAction( simulatorPtr, actionOutput );
      
      // run for specified time
      while( ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // output desciptor end 
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": end " << simulatorPtr->currentAction->actionDescriptor << " output \n";
      printAction( simulatorPtr, actionOutput );
   }   

   // unrecognized action type
   else 
   {
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": unrecognized action \n";
      printAction( simulatorPtr, actionOutput );
   }   

   // end thread
   pthread_exit(0);
}

/**
quantumRunner

Simulates execution of action for given time quantum.
Each thread calls this function upon its creation in threadActionsQuantum(). 
Each thread prints appropriate information by calling printAction(),
waits until it is blocked or completed, according to appropriate cycle time and then
returns to threadActionsQuantum to either Exit or return to Ready queue.

@param void pointer to simulatorConfig with current action
@pre a valid action is given
@post action is blocked or completed, and logged
@return void
*/
void *quantumRunner( void* simulatorVoidPtr )
{
   // initialize variables

      // set void pointer to Action pointer
      OSCB* simulatorPtr = static_cast<OSCB*>( simulatorVoidPtr );

      // prepare output string
      stringstream actionOutput;
      actionOutput.precision( PRECISION );
      actionOutput << fixed; 
      string endStatus;

      // action start time
      float floatTime;
      float runTime;

      // for if action will complete within given time quantum
      if( simulatorPtr-> quantum > simulatorPtr -> currentAction -> actionCycle )
      {
         // update estimated run time
         runTime = simulatorPtr->quantum * simulatorPtr->cycleData[ simulatorPtr->currentAction->actionDescriptor ] * CYCLE_TO_MS;

         // update action status to exit 
         endStatus = ": end ";
      }

      // for if action will not complete in this time quantum
      else
      {
         // update estimed run time
         runTime = simulatorPtr-> currentAction ->actionCycle * simulatorPtr->cycleData[ simulatorPtr->currentAction->actionDescriptor ] * CYCLE_TO_MS;

         // update action status to blocked
         endStatus = " : block ";
      }
      
      // update number of remaining cycles (for SRTF-P)
      simulatorPtr->currentAction->actionCycle = simulatorPtr->currentAction->actionCycle - simulatorPtr->quantum;

   // application action type
   if( simulatorPtr->currentAction->actionType == 'A' )
   {
      // start application descriptor
      if( simulatorPtr->currentAction->actionDescriptor == "start")
      {
         actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: preparing process " << simulatorPtr->currentAction->pcbID << endl;   
         printAction( simulatorPtr, actionOutput );
      }      

      // end application descriptor
      else
      {
         actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: removing process " << simulatorPtr->currentAction->pcbID << endl; 
         printAction( simulatorPtr, actionOutput );     
      }
       
   }

   // process action type
   else if( simulatorPtr->currentAction->actionType == 'P' )
   {
      // set current time
      floatTime = ((float)(clock() - simulatorPtr->timer)/CLOCKS_PER_SEC);

      // run application descriptor start
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": start " << "processing action \n"; 
      printAction( simulatorPtr, actionOutput );

      // run for specified time
      while( ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // run application desciptor end 
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << endStatus << "processing action \n";
      printAction( simulatorPtr, actionOutput );      
   }

   // input action type
   else if( simulatorPtr->currentAction->actionType == 'I' )
   {
      // set current time
      floatTime = ((float)(clock() - simulatorPtr->timer)/CLOCKS_PER_SEC);

      // input descriptor start
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": start " << simulatorPtr->currentAction->actionDescriptor << " input \n"; 
      printAction( simulatorPtr, actionOutput );

      // run for specified time
      while( ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // input desciptor end 
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << endStatus << simulatorPtr->currentAction->actionDescriptor << " input \n";
      printAction( simulatorPtr, actionOutput );
   }

   // output action type
   else if( simulatorPtr->currentAction->actionType == 'O' )
   {
      // set current time
      floatTime = ((float)(clock() - simulatorPtr->timer)/CLOCKS_PER_SEC);

      // output descriptor start
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": start " << simulatorPtr->currentAction->actionDescriptor << " output \n"; 
      printAction( simulatorPtr, actionOutput );
      
      // run for specified time
      while( ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC) < (floatTime + runTime) ); // busy wait

      // output desciptor end 
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << endStatus << simulatorPtr->currentAction->actionDescriptor << " output \n";
      printAction( simulatorPtr, actionOutput );
   }   

   // unrecognized action type
   else 
   {
      actionOutput << ((float)(clock()-simulatorPtr->timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process " << simulatorPtr->currentAction->pcbID << ": unrecognized action \n";
      printAction( simulatorPtr, actionOutput );
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
void printAction( OSCB* simulatorPtr, stringstream& actionOutput )
{
   // print to console
   if( simulatorPtr->logLocation == 'M' || simulatorPtr->logLocation == 'B' )
   {
      cout << actionOutput.str();
   }

   // print to file
   if( simulatorPtr->logLocation == 'F' || simulatorPtr->logLocation == 'B' )
   {
      simulatorPtr->outputFile << actionOutput.str();
   }

   // clear string stream
   actionOutput.str( string() );
   actionOutput.clear();
}

