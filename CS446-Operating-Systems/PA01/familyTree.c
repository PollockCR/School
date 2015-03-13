/**
 * @file   familyTree.c
 * @author Catherine Pollock
 * @date   February 2015
 * @brief  Prints a family tree using processes.
 *
 * Reads in parent names along with children names
 * and creates processes for each first parent and 
 * children. Family tree is printed out with proper 
 * indentaiton and includes name, parent process id,
 * spouse name (if applicable), and children. 
 */

/// header files
#include <sys/types.h> // used for process creation
#include <stdio.h> // used for I/O
#include <unistd.h> // used to define types
#include <string.h> // used for c string comparisons
#include <sys/wait.h> // used for wait

// global constants
const int DIMS = 100; 

// global variables
int tabcount = 0; 
pid_t pid;
int selfIndex;
int child; 

// function prototypes
int saveData( char *filepath, char arr[DIMS][DIMS][DIMS], int arrChild[DIMS] );
void forkAndPrint( char arr[DIMS][DIMS][DIMS], int arrChild[DIMS], int parentRow, int colIndex );
int findChildIndex( char arr[DIMS][DIMS][DIMS], int parentRow, int childIndex );

// main function
int main( int argc, char *argv[] )
{
  // variable instantiation
  char arr[DIMS][DIMS][DIMS];
  int arrChild[DIMS];
  int saveSuccess;

  // check for incorrect arguments
  if( argc != 2 )
  {
    // print failure
    printf("Incorrect arguments given. Please try again.\n");

    // return failure
    return 1;
  }

  // read in data from file
  saveSuccess = saveData( argv[1], arr, arrChild );

  // check for incorrect file or no data
  if( saveSuccess == 0 || (strlen( arr[0][0] ) < 1) )
  {
    // print failure
    printf("File does not exist or does not contain valid information. Please try again.\n");

    // return failure
    return 1;
  }

  // fork and print children
  forkAndPrint( arr, arrChild, 0, 0 );

  // return success
  return 0;
}

// function implementation

/**
saveData

Saves data from file passed through command line input. Each
name passed is saved as a c string to the next column in array arr.
Each time a newline is found, the array is incremented to the next row.
@param filepath the (c string) name of the file passed through command line 
@param arr the array where names are saved
@param arrChild array containing how many children each parent has
@pre A filename was passed to function and arrays were initialized
@post The array contains data from file if file was valid
@exception If file is invalid or unreadable
@return int 1 if file reading successful, 0 if not
*/
int saveData( char *filepath, char arr[DIMS][DIMS][DIMS], int arrChild[DIMS] )
{
  // instantiate variables
  int parentRow = 0;
  int place = 0;
  int i, j;
  FILE *fp; 
  char temp;

  // fill array with null values
  for( i = 0; i < DIMS; i++ )
  {
    for( j = 0; j < DIMS; j++ )
    {
      strcpy( arr[i][j], "" );
    }
  }

  // open file
  fp = fopen( filepath, "r" );

  // check for open failure
  if( fp == NULL )
  {
    // return failure
    return 0;    
  }

  // read in data from file

    // until the end of the file
    while( !feof(fp) )
    {
      // save parent name
      fscanf( fp, "%s", arr[parentRow][place]);
      arrChild[parentRow] = -1;

      // increment to next person
      place++;

      // set temp char
      temp = fgetc(fp);

      // read in spouse and children

        // check for end of line
        while( temp != '\n' && temp != EOF)
        {
          // read in child
          fscanf( fp, "%s", arr[parentRow][place]);
          arrChild[parentRow]++;

          // increment place
          place++;

          // set temp char
          temp = fgetc(fp);
        }

      // reset place index
      place = 0;

      // increment parent index
      parentRow++;
    }

  // close file
  fclose( fp );

  // return success
  return 1;
}

/**
forkAndPrint

Forks the current process.
If process is child, checks if it is
married. If it is, tabs, name, parent process id, and spouse name
are printed and this function is recursively called for however many
children this parent has. If it is not married, the name and parent 
process id are printed.
If process if the parent process, it waits for its child process to finish. 
@param arr that holds char values that make up names in three dimensions
@param arrChild that holds count of children for each parent
@pre none
@post none
@return void
*/
void forkAndPrint( char arr[DIMS][DIMS][DIMS], int arrChild[DIMS], int parentRow, int colIndex )
{
  // variables
  int j;
  pid = fork();

  // for child process
  if( pid == 0 )
  {
    // finds self's row
    selfIndex = findChildIndex( arr, parentRow, colIndex );

    // if married
    if( selfIndex != -1 )
    {
      // print tabs
      for( j = 0; j < tabcount; j++ )
      {
        printf( "\t" );
      }

      // print self
      printf("%s(%d)", arr[selfIndex][0], getppid() );  

      // checks for spouse 
      if( strcmp( arr[selfIndex][1], "" ) != 0 )
      {
        // print spouse      
        printf("-%s", arr[selfIndex][1] );
      }

      // print new line
      printf("\n"); 

      // increase tab count
      tabcount++;

      // recursively call this function for each child
      // (will not be called if no children)
      for( child = 0; child < arrChild[selfIndex]; child++ )
      {
        forkAndPrint( arr, arrChild, selfIndex, child+2 );
      }
    }

    // if not married
    else if( selfIndex == -1 )
    {
      // print tabs
      for( j = 0; j < tabcount; j++ )
      {
        printf( "\t" );
      }

      // print self
      printf("%s(%d)", arr[parentRow][colIndex], getppid() );
      printf("\n" );
    }
  }

  // for parent process
  else 
  {
    // wait for child
    waitpid(pid, NULL, 0);
  }
}

/**
findChildIndex

Searches for the name passed through indecies from parameters 
in the first column of each row of arr. If name is found, that 
means that person is married and index of row is returned. If
the name is not found, the person is unmarried with no children and
-1 is returned. 
@param arr containing characters that make up names 
@param parentRow the index of row name is first found
@param childIndex the index of column name is found in parent row
@pre in bounds arguments passed
@post none
@return int of row name is found on, -1 if not found
*/
int findChildIndex( char arr[DIMS][DIMS][DIMS], int parentRow, int childIndex )
{
  // initialize variables
  int row;

  // loop through each line
  for( row = parentRow; row < DIMS; row++ )
  {
    // check for same name and not null name
    if( ( strcmp( arr[row][0], arr[parentRow][childIndex] ) == 0 ) && ( strcmp( arr[row][0], "" ) != 0 ) )
    {
      // return index of child (success)
      return row;
    }
  }

  // return -1 for failure to find self because unmarried
  return -1;
}
