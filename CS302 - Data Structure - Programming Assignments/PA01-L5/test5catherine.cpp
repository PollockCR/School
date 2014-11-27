#include "listLinked.h"
#include "listLinked.cpp"
#include <iostream>
using namespace std;


int main()
{
   List<int> alist;
   for( int i = 0; i<5; i++ )
   {
      alist.insert( i );
      cout << alist.getCursor() << ' ';
   }
   cout << endl;
   List<int> blist;
   blist = alist;
   alist.replace(10);
 //  blist.gotoPrior();
//   blist.remove();
//   cout << "ok" << endl;
 //  blist.gotoPrior();
  // blist.clear();
   cout << endl << alist.getCursor() << endl;
}
