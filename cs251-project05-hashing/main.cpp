/*main.cpp*/

//
// Hashing program for specialized Illinois license plates,
// which processes an input file of license plates and fines.
// The output is the total fines per license plate, in 
// sorted order.
//
// Original author: Prof. Joe Hummel
// Modified by:     Shawn John UIN: 663331719
//
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>

#include "ILplates.h"

using namespace std;


//
// hashInput:
//
void hashInput(string basename, ILplates& hashplates)
{
  string infilename = basename + ".txt";
  ifstream infile(infilename);

  if (!infile.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << infilename << "', exiting." << endl;
    cout << endl;
    exit(-1);
  }

  //
  // input the plates and fines:
  //
  cout << "Reading '" << infilename << "'..." << endl;

  string fine;
  string plate;

  getline(infile, fine);

  //
  // for each pair (fine, license plate), hash and store/update fine:
  //
  while (!infile.eof())
  {
    getline(infile, plate);

    //cout << fine << endl;
    //cout << plate << endl;

    // 
    // is plate valid?  Only process valid plates:
    //
    if (hashplates.Hash(plate) >= 0)  // yes:
    {
      int amount = hashplates.Search(plate);
      if (amount < 0)  // not found:
      {
        hashplates.Insert(plate, stoi(fine));
				//cout<<"Exited insert"<<endl;
      }
      else  // we found it, so update total in hash table:
      {
        amount += stoi(fine);
        hashplates.Insert(plate, amount);
      }

    }//valid

    getline(infile, fine);
  } //end of while
	
} //end of inputHash

/* Writes the values from the hashtable using vectors and write them into an output file */
void writeOutput(vector<string> &plates, vector<int> &amounts, string basename){
	
	string outFileName = basename + "-output.txt";
	ofstream outfile(outFileName);
	
	cout << "Writing '" << outFileName << "'..." << endl;
	
	for (size_t i = 0; i < plates.size(); ++i)
  {
		outfile <<"\""<< plates[i] << "\"" <<" $"<< amounts[i] << endl;
  } //end of for loop
	
} //end of writeOutput

/* Modified Version of insertion sort with strings. Takes in both fines and plates because the fine positions are associated with the plates
 * so if you switch the plates, switch the fines as well. This is different from simple insertion sort because you have to check the characters of the string */ 
void MySort(vector<string> &plates, vector<int> &amounts){
	cout<<"Sorting..."<<endl;
	int i =1; // the position that is being compared againist start at the second word
	int j = 0; // the position that moves down the vector
	int k = 0; //the position of the character in each string 
	int letter; // holds the character of the string being compared aganist 
	
	
	string originalString; //is the original word being compared againist
	int originalAmount;    //holds the original amount since both fines and plates  are associated with their positions
	int size = plates.size(); //size of the vectors
	
	//loops through the vector and swaps when it finds a word that is more alphabetically infront of it 
	for (i = 1; i < size ; i++){
		k = 0; //compares against the first character
		letter = plates[i][k];
		originalString = plates[i];
		originalAmount = amounts[i];
		j = i-1; // start at the word before
		//only enter this loop if the character before is "bigger"
		while (j>= 0 && plates[j][k]>=letter){
			
			//if the words have the same character move on to the next character  and compare aganist the original string character
			if (plates[j][k]==letter){
				++k;
				letter = originalString[k];
				continue;
			}
			
			// else swap and reset the starting character position to zero and 
			else{
				plates[j+1] = plates[j];
				amounts[j+1] = amounts[j];
				j = j -1;
				k = 0;
				letter = originalString[0];
			}
		} //end of while loop
		
		//swap the original word to where the "smallest word " was found 
		plates[j+1] = originalString;
		amounts[j+1] = originalAmount;
		
	} // end of for loop
} //end of MySort


int main()
{
  int    N;        // = 10000;
  string basename; // = "tickets1";

  cout << "Enter hashtable size> ";
  cin >> N;

  hashtable<string, int>  ht(N);
  ILplates                hashplates(ht);

  cout << "Enter base filename> ";
  cin >> basename;
  cout << endl;

  //
  // process input file of fines and license plates:
  //
  hashInput(basename, hashplates);

  //
  // debugging:
  //
  vector<string> plates = ht.Keys();
  vector<int> amounts = ht.Values();
	MySort(plates, amounts);

	writeOutput(plates, amounts, basename);

  //
  // done:
  //
  return 0;
}