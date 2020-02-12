/* ILplates.h */

//
// Hashing functions to store (license plate, amount) pairs
// using linear probing.  Implementation in associated .cpp
// file.
//
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#pragma once

#include <iostream>
#include <string>
#include <ctype.h>
#include "hashtable.h"

using namespace std;

class ILplates
{
private:
  hashtable<string, int>& HT;  // reference to underlying hashtable:

public:
  //
  // constructor
  //
  ILplates(hashtable<string, int>& ht)
    : HT(ht)
  { }

  int  Hash(string plate);
  int  Search(string plate);
  void Insert(string plate, int newValue);
};


bool validPlate(string plate); // function to check if the plate was valid 