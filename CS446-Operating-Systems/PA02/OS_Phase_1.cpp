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
   #include <sys/types.h> // used for process creation
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

// global variables
   clock_t timer;
   stringstream actionOutput;

// function prototypes
   bool saveConfig( char* configPath, float &phase, char* metaPath,
      map<string, float>& cycleData, char* logLocation, char* logPath );
   int saveMetaData( char* metaPath, PCB& process );
   void threadActions( PCB& process );
   void *runner( void *actVoidPtr ); // threads call this function 

// main program
int main( int argc, char* argv[] )
{
   // initialize variables
   bool saveSucess;
   float phase = 0.0;
   char metaPath[ STD_LINE_LEN ];
   map<string, float> cycleData;
   char logLocation[ STD_STR_LEN ];
   char logPath[ STD_LINE_LEN ];
   int processCount = 0;
   PCB process;

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
         saveSucess = saveConfig( argv[1], phase, metaPath, cycleData, logLocation, logPath );

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

      // start simulator
      threadActions( process );

         /*
         // check for failed save
         if( !saveSucess )
         {
            // print failure
            cout << "Error in processing action. Please try again." << endl;

            // return failure
            return 1;
         }      
         */
      
      cout << actionOutput.str();

   // STATE: Exit

      // return success
      return 0;
}

// function implementation 
bool saveConfig( char* configPath, float &phase, char* metaPath,
   map<string, float>& cycleData, char* logLocation, char* logPath )
{
   // initialize variables
   ifstream configFile;
   float tempTime;

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
         cycleData[ "processor" ] = tempTime;

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
      configFile.getline( logLocation, STD_LINE_LEN );

      // read in log file path
      if( strcmp( logLocation, "Log to Monitor" ) != 0 )
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

         // Read in start
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
                     // FOR TESTING, PRINT EACH ACTION
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

   // return process count
   return processCount;
}

void threadActions( PCB& process )
{
   // initialize variables
   int tidCount = process.actions.size() + 2; // number of actions in queue plus system start/end
   pthread_t tids[ tidCount ]; // an array of threads to be joined
   pthread_attr_t attr;
   void* actVoidPtr;
   int err;
   int tidIndex = 0;
   int actionIndex;

   // create start and end actions
   Action startAction('S', "start", 0);
   Action endAction( 'S', "end", 0);

   // set time output   
   actionOutput.precision(6);
   actionOutput << fixed;    
   timer = clock();   

   // get the default attributes
   pthread_attr_init(&attr);

   // complete actions

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

void *runner( void* actVoidPtr )
{
   // initialize variables

      // set void pointer to Action pointer
      Action* actPtr = static_cast<Action*>( actVoidPtr );

   // system action type
   if( actPtr->actionType == 'S' )
   {
      // start system descriptor
      if( actPtr->actionDescriptor == "start")
      {
         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "Simulator program starting \n";
      }

      // end system descriptor
      else
      {
         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "Simulator program ending \n";
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

         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: starting process 1 \n";   
      }      

      // end application descriptor
      else
      {
         actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
         actionOutput << " - " << "OS: removing process 1 \n";         
      }
       
   }

   // process action type
   else if( actPtr->actionType == 'P' )
   {
      // run application descriptor start
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: start processing action \n"; 
      
      // run for specified time
      /////////////////////// !!!!

      // run application desciptor end 
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: end processing action \n";

   }

   // input action type
   else if( actPtr->actionType == 'I' )
   {
      // input descriptor start
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: start " << actPtr->actionDescriptor << " input \n"; 
      
      // run for specified time
      /////////////////////// !!!!

      // input desciptor end 
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: end " << actPtr->actionDescriptor << " input \n";
   }

   // output action type
   else if( actPtr->actionType == 'O' )
   {
      // output descriptor start
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: start " << actPtr->actionDescriptor << " output \n"; 
      
      // run for specified time
      /////////////////////// !!!!

      // output desciptor end 
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: end " << actPtr->actionDescriptor << " output \n";

   }   

   // unrecognized action type
   else 
   {
      actionOutput << ((float)(clock()-timer)/CLOCKS_PER_SEC);
      actionOutput << " - " << "Process 1: unrecognized action \n";

   }   

   // end thread
   pthread_exit(0);
}