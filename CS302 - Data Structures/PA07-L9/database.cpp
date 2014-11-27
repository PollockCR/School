//--------------------------------------------------------------------
//
//  Laboratory 11, In-lab Exercise 1                     database.cpp
//
//  Indexed accounts database program
//
//--------------------------------------------------------------------

// Builds a binary search tree index for the account records in the
// text file accounts.dat.

#include <iostream>
#include <fstream>
#include "BSTree.cpp"

using namespace std;

//--------------------------------------------------------------------
//
// Declarations specifying the accounts database
//

const int nameLength      = 11;   // Maximum number of characters in
                                  //   a name
const long bytesPerRecord = 37;   // Number of bytes used to store
                                  //   each record in the accounts
                                  //   database file

struct AccountRecord
{
    int acctID;                   // Account identifier
    char firstName[nameLength],   // Name of account holder
         lastName[nameLength];
    double balance;               // Account balance
};

//--------------------------------------------------------------------
//
// Declaration specifying the database index
//

struct IndexEntry
{
    int acctID;              // (Key) Account identifier
    long recNum;             // Record number

    int getKey () const
        { return acctID; }   // Return key field
    void setKey (int key)
        { acctID = key; }   // Return key field
};


//--------------------------------------------------------------------

int main ()
{
    ifstream acctFile ("accounts.dat");   // Accounts database file
    AccountRecord acctRec;                // Account record
    BSTree<IndexEntry,int> index;         // Database index
    IndexEntry entry;                     // Index entry
    int searchID;                         // User input account ID
    long recNum;                          // Record number
    int numRecords = 0;                   // Total number of records
    bool foundID;
    int i;

    // Iterate through the database records. For each record, read the
    // account ID and add the (account ID, record number) pair to the
    // index.    
    acctFile.seekg( 0, acctFile.end ); // go to end of file
    numRecords = acctFile.tellg() / 37; // record what line cursor is
    acctFile.clear(); // get rid of end of file flag

    for( i = 0; i < numRecords; i++ )
    {
       acctFile.clear();
       acctFile.seekg( 0, acctFile.beg ); // reset cursor to beginning
       acctFile.seekg( i * 37 ); // skip index number of records/lines
       acctFile >> entry.acctID;
       entry.recNum = i;
       index.insert( entry );
    }

    // Output the account IDs in ascending order.
    cout << endl << "Account IDs : " << endl;
    index.writeKeys();
    cout << endl;

    // Clear the status flags for the database file.
    acctFile.clear();

    // Read an account ID from the keyboard and output the
    // corresponding record.
    cout << "Enter account ID : ";
    cin >> searchID;
    do
    {
      // search for id
      foundID = index.retrieve( searchID, entry );

      if( foundID )
      {
         acctFile.clear();
         acctFile.seekg( 0, acctFile.beg ); // reset cursor to beginning
         acctFile.seekg( entry.recNum * 37 ); // skip index number of records/lines
         
         // save vales
         // save record number
         recNum = entry.recNum;
         
         // save account ID
         acctFile >> acctRec.acctID;
         acctFile.ignore( 37, ' ' ); // ignore space
         
         // save first name
         acctFile.getline( acctRec.firstName, nameLength, ' ' );
         while( acctFile.peek() == ' ' )
         {
            acctFile.ignore( 37, ' ' ); // ignore spaces
         }
          
         // save last name
         acctFile.getline( acctRec.lastName, nameLength, ' ' );
         while( acctFile.peek() == ' ' )
         {
            acctFile.ignore( 37, ' ' ); // ignore spaces
         }
         
         // save balance
         acctFile >> acctRec.balance;
         
         // output values
         cout << recNum << " : ";
         cout << acctRec.acctID << ' ';
         cout << acctRec.firstName << ' ';
         cout << acctRec.lastName << ' ';
         cout << acctRec.balance << endl;
      }
      else
      {
         // for not found id
         cout << "No record with that account ID" << endl;
      }

      cout << "Enter account ID (EOF to quit): ";
      cin >> searchID;
    } while ( !cin.eof() );

    return 0;


}
