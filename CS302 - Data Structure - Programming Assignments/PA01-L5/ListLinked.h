//--------------------------------------------------------------------
//
//  Laboratory 5                                          ListLinked.h
//
//  Class declaration for the linked implementation of the List ADT
//
//  Catherine Pollock
//
//--------------------------------------------------------------------

#ifndef LISTLINKED_H
#define LISTLINKED_H

#include <stdexcept>
#include <iostream>

using namespace std;

template <typename DataType>
class List {
  public:
    List(int ignored = 0);
    List(const List& other);
    List& operator=(const List& other);
    ~List();

    bool insert(const DataType& newDataItem);
    bool remove();
    bool replace(const DataType& newDataItem);
    void clear();

    bool isEmpty() const;
    bool isFull() const;

    bool gotoBeginning();
    bool gotoEnd();
    bool gotoNext();
    bool gotoPrior();

    DataType getCursor() const;

    // Programming exercise 2
    bool moveToBeginning ();

    // Programming exercise 3
    bool insertBefore(const DataType& newDataItem);
    
    void showStructure() const;

  private:
    class ListNode {
      public:
        ListNode(const DataType& nodeData, ListNode* nextPtr);

        DataType dataItem;
        ListNode* next;
    };

    ListNode* head;
    ListNode* cursor;

};

#endif

