/*ILplates.cpp*/

//
// Hashing functions to store (license plate, amount) pairs
// using linear probing.
//
// << YOUR NAME >>
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <string>
#include <ctype.h> //used for isdigit

#include "ILplates.h"

using namespace std;


//
// Hash:
//
// Given a specialized Illinois license plate, returns an index into
// the underyling hash table.  If the given plate does not follow the
// formatting rules given below, -1 is returned.
//
// Personalized:
//   letters and numbers, with a space between the letters 
//   and numbers.  Format: 1-5 letters plus 1..99 *OR* 
//   6 letters plus 1..9
//
//   Examples: A 1, B 99,  ZZZZZ 1, ABCDEF 3
//
// Vanity:
//   Format: 1-3 numbers *OR* 1-7 letters
// 
//   Examples: 007, 1, 42, X, AAA, ZZZEFGH
//


/*Function that checks if the plate is valid */
bool validPlate(string plate){
	
	//no plate can be longer than 8 
	if(plate.size() > 8){
		return false;
	}
	
	//check is the plate starts with a letter 
	if((plate[0]-'A' >= 0) && (plate[0]-'A' <26)){
		int i=0;
		while(plate[i]!=NULL){
			
			//if there is only letters it is fine keep going 
			if((plate[i]-'A' >= 0) && (plate[i]-'A' <26)){
				++i;
				continue;
			}
			
			//if a space is encountered ther can only be 2 numbers left or 1 number left if the string is 6 letters the if in here make sure they are numbers
			else if(plate[i]==' '){
				//checks if there is two numbers 
				
				if(plate.size()-1-i ==2){
					if(( plate[i+1]-'0' >= 0 && plate[i+1]-'0' <10) && ( plate[i+2]-'0' >= 0 && plate[i+2]-'0' <10)){
						return true;
					}
					// if they weren't numbers return false
					else{
						return false;
					}
				}
				
				// checks if only one number remains
				else if (plate.size()-1-i ==1){
					if( plate[i+1]-'0' >= 0 && plate[i+1]-'0' <10){
						return true;
					}
					else{
						return false;
					}
				}
				
				//return false if the number left over wasn't 2 or 1 
				else{
					return false;
				}
			}
			
			//if no letter or space was encountered return false
			else{
				return false;
			}
		}
		
		// if there were only letters there can be only a maximum of 7 letters
		if(i > 7){
			return false;
		}
		
		//if the string started with a letter and passed all cases return true
		return true;
	}
	
	//if the number started with a number make sure there is a maximum of only 3 numbers
	else if ( plate[0]-'0' >= 0 && plate[0]-'0' <10){
		if (plate.size() > 3){
			return false;
		}
		else{
			int i = 0;
			while(plate[i]!= NULL){
				if(isdigit(plate[i])==false){
					return false;
				}
				else{
					i++;
				}
			}
			return true;
		}
	}
	
	else{
		return false;
	}
	
} // end of bool validString


/* Hash function that tries its best to provide a unique index for the plate */ 
int ILplates::Hash(string plate)
{
	// the booleans check if the plate is a vanity plate and if it is which type
	bool allNumbers = true;
	bool allLetters = true;
	
	//start at a random index rather than zero for more spread of hash
	long long index = 37;
	
	//start at the end of the string rather than the beginning
	int i = plate.size()-1;
	
	//loop counter 
	int counter = 0;
	
	//makes sure the string is valid 
	if(validPlate(plate) == false){
		return -1;
	}
	
	//jumps into hashing 
	else{
		
		// holds the position of each character 
		long long pos = 1;
		while(i >= 0){
			
			//skip over the space that doesn't need to be indexed 
			if(plate[i]==' '){
				i--;
				counter ++;
				continue;
			}
			
			else{
				//if the character is a digit add index and move the index to the right by 10 
				// if this if was called that means it can't be the vanity plate type of all letters 
				if(isdigit(plate[i])){
					index = index + pos * (plate[i]-'0' + 1);
					pos = pos * 10;
					allLetters = false;
				}
				//if the character is not a digit its a charcater which can take up to two spots so shift to the right by a 100 
				// and if this else was called that means its not vanity type of all numbers 
				else{
					allNumbers = false;
					index = index + pos * (plate[i]-'A'+1);	
					pos = pos * 100;
				}
				--i;  //decrement the postion since started from the end 
				counter++; //increment the counter 
			}
		} //end of while 
	} // end of else 
	

	/* If the plate was a vanity of type allNumbers then there is no need to "spread" the index out because it will generate a unique value for 
	 * any hashtable greater than or eqaual to 1000 */
	
	if(!allNumbers){
		//if it was a vanity plate of type all letters try to keep the index inbetween 1000 and half the size of the hashtable
		if (allLetters){
			if (index > (HT.Size()/2)){
				index -= (HT.Size()/2);
				if(index < 1000){
					index +=1000;
				}
			}
		}
		// if the plate was personalized than try to keep its index in the upper half of the hashtable
		else if (!allLetters && !allNumbers){
			index = index %HT.Size();
			if(index < (HT.Size()/2)){
				index += (HT.Size()/2);
			}
		} 	
		// do more spreading of the index using a large prime number  
		index += (counter+3) * 5101;
		
	} // enfd of if !allNumbers
	
	//return the index % the hashtable size to make sure the index is within the hashtable
  return index % HT.Size();
} // end of Hash


//
// Search
// 
// Hashes and searches for the given license plate; returns the 
// associated value for this plate if found, or -1 if not found.
//
int ILplates::Search(string plate)
{
	int index = Hash(plate);
	//makes sure that the plate was hashable 
	if(index == -1){
		return -2;
	}
	
	else{
		//continously loop until an empty spot or the value is found 
		while(1){
			//holds the data of the hashable
			bool empty;
			int fine;
			string key;
			//if we hit the end of the hashtable go back to the beginning of the hashtable
			if(index == HT.Size()){
				index = 0;
				continue;
			}
			//get the data at that position of the hashtable
			HT.Get(index, empty, key, fine);
			
			//if empty means no plate was found 
			if(empty == true){
				return -1;
			}
			
			//plate was found return fine
			else if(key == plate){
				return fine;
			}
			//collision
			else{
				++index;
			}
		} // end of while 
	} // end of else 
} // end of Search


//
// Insert
//
// Inserts the given (plate, newValue) into the hash table,
// overwriting an existing value if there.
//
void ILplates::Insert(string plate, int newValue)
{
	int index = Hash(plate);
	//makes sure the plate is of the correct format
	if(index == -1){
		return;
	}
	// continously loops until and empty is found or the plate itself is found 
	while(1){
		bool empty;
		int fine;
		string key;
		
		//jump back to beginning
		if(index == HT.Size()){
			index = 0;
			continue;
		}
		//get the data of the hashtable at the index 
		HT.Get(index, empty, key, fine);
		if (empty == true){
			HT.Set(index,plate,newValue);
			return;
		}
		else if(plate == key){
			HT.Set(index, key, newValue);
			return;
		}
		else{
			++index;
		}
	} // end of while 
} //end of Insert 



