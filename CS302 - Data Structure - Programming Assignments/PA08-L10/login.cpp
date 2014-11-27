/**
 * @file login.cpp
 * @author CatherinePollock
 * @date 10/27/14
 * 
 * This is the file used to read in account usernames and
 * passwords from a file, prompt user for a username and 
 * password, and then authenticate the entered username and 
 * password, based on the accounts read in from file.  
*/

#include <iostream>
#include <fstream>
#include <string>
#include "HashTable.cpp"

using namespace std;

/// data class used in hash table's binary search trees

class TestData {
  public:
    TestData();
    void setKey(const string& newKey); /// set key field
    string getKey() const; /// return key field
    void setPwd(const string& newPwd); /// set password field
    string getPwd() const; /// return password field 
    static unsigned int hash(const string& str);

  private:
    string key; /// (key) account username
    string pwd; /// account password
};

/**
 * default constructor
 *
 * Creates an instance of test data.
*/
TestData::TestData(){}

/**
 * setKey
 *
 * Sets the key of TestData to the key passed
 * @param newKey (const string&) to be assigned to key
*/
void TestData::setKey(const string& newKey) {
    key = newKey;
}

/**
 * getKey
 *
 * Returns the key of TestData
 * @return string of key 
*/
string TestData::getKey() const {
    return key;
}

/**
 * setPwd
 *
 * Sets the password of TestData to the password passed
 * @param newPwd (const string&) to be assigned to password
*/
void TestData::setPwd(const string& newPwd) {
    pwd = newPwd;
}

/**
 * getPwd
 *
 * Returns the password of TestData
 * @return string of password 
*/
string TestData::getPwd() const {
    return pwd;
}

/**
 * hash
 *
 * Gets the index of where to insert passed in string
 * using algorithm involving adding each character value
 * @param string of the key which decides the hash index
 * @return unsigned int containing hash index calculated
*/
unsigned int TestData::hash(const string& str)
{
   unsigned int val = 0;

   for (int i = 0; i < str.length(); ++i)
   {
      val += str[i];
   }

   return val;
}

/**
 * main function
 *
 * Reads in account information, prompts user for login
 * information, and compares with saved data, printing results. 
 * @return int success or failure 
*/
int main()
{
   /// initialize variables
   int numRecords = 8;
   ifstream acctFile( "password.dat" );    
   acctFile.clear();  
   HashTable<TestData, string> index( numRecords );
   TestData entry;
   string tempName;
   string tempPwd;
   bool nameFound;
   bool pwdCorrect;
   int i;
   

   /// loop through each record in file
   for( i = 0; i < numRecords; i++ )
   {
      /// save account username
      getline( acctFile, tempName, ' ' );
      entry.setKey( tempName );
      
      /// skip over spaces
      while( acctFile.peek() == ' ' )
      {
         acctFile.ignore( 37, ' ' );
      }
      
      /// save account password
      getline( acctFile, tempPwd, '\n' );
      entry.setPwd( tempPwd );
      
      /// insert data into hash table
      index.insert( entry );
   }
    
   /// clear data file flags
   acctFile.clear();
   
   /// print the structure of names (keys) saved into hash table
   index.showStructure();
   
   /// prompt for username
   cout << "Login: ";
   cin >> tempName;
   do 
   {
      /// prompt for password
      cout << "Password: ";
      cin >> tempPwd;
      
      /// check for username in table
      nameFound = index.retrieve( tempName, entry );
      
      /// if the name was found
      if( nameFound )
      {
         /// check if password matches and print results
         if( entry.getPwd() == tempPwd )
         {
            cout << "Authentication successful" << endl;
         }
         else 
         {
            cout << "Authentication failure" << endl;
         }
      }
      /// if the name was not found print failure
      else 
      {
         cout << "Authentication failure" << endl;
      }
      
      /// prompt for another login
      cout << "Login: ";
      cin >> tempName;
     
   /// continue until end of input     
   } while ( !cin.eof() );
}

























