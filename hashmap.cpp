/*
Project 6
File- hashmap.cpp
Caleb Satvedi
Class: Cs 251 Sp 2022
Description: This file is the implementation of the hashmap class implemented using hashmaps.
The code contains how to insert a new value into the hashmap, retrieve a value,
check if the value exists, and puts the hashmap key's into a vector. This will be used
for util.h 
*/

#include "hashmap.h"
#include <vector>
#include <iostream>
using namespace std;

//
// This constructor chooses number of buckets, initializes size of map to 0, and
// creates a bucket array with nBuckets number of pointers to linked lists.
//
hashmap::hashmap() {
    this->nBuckets=10;
    this->nElems=0;
    this->buckets=createBucketArray(nBuckets);
}

//
// Given a number of buckets, creates a hashtable (array of linked list heads).
// @param nBuckets the number of buckets you want in the hashtable.
// return an array of heads of linked lists of key_val_pairs
//
hashmap::bucketArray hashmap::createBucketArray(int nBuckets) {
    bucketArray newBuckets = new key_val_pair*[nBuckets];
    for (int i = 0; i < nBuckets; i++) {
        newBuckets[i] = nullptr;
    }
    return newBuckets;
}

//
// This destructor frees memory for all elements of buckets & also all elements
// of linked lists that those bucket pointers may point to.
//
hashmap::~hashmap() {
    for (int i=0; i < nBuckets; i++){
        key_val_pair *front =  buckets[i];
        while (front != nullptr) {
            key_val_pair *temp = front->next;
            delete front;
            front = temp;
        }
    }
    delete[] buckets;
    
}

// This method puts key/value pair in the map.  It checks to see if key is
// already in map while traversing the list to find the end of it.
void hashmap::put(int key, int value) {
  // holds the bucket number of where we are going to put key
  int daNumber = hashFunction(key) %10;
  // create counter variables holder and prev of type key_val_pair to
  // iterate through the bucket and insert in the correct place
  key_val_pair * holder = buckets[daNumber];
  key_val_pair * prev = nullptr;
  // base case: we are dealing with an empty bucket, so this is the first 
  // key_val_pair we are inserting
  if (holder == nullptr) {
    // create new key_val_pair node 
    key_val_pair *  temp = new key_val_pair;
      temp->key = key;
      temp->value = value;
    // set pointers accordingly 
    temp->next = nullptr;  
    buckets[daNumber] = temp;
    nElems++;
    return;
  }
  // loop through the bucket and go to the end
  while (holder != nullptr) {
    //if we already have said key in the bucket, then we just update the value
    if (holder->key == key) {
      holder->value = value;
      return;
    }
    // go to next key_val_pair
    prev = holder;
    holder = holder->next;  
  }
  // create new key_val_pair struct
  key_val_pair *  temp = new key_val_pair;
      temp->key = key;
      temp->value = value;
  // adjust pointers accordingly so that temp is at the end 
    temp->next = nullptr; 
  prev -> next = temp;  
  nElems++;
}

//
// This method returns the value associated with key.
//
int hashmap::get(int key) const {
  // daNumber holds the bucket number which hold the key
    int daNumber = hashFunction(key) %10;
  // create a counter key_val_pair to iterate through the list
  key_val_pair * checker = buckets[daNumber];
  // if we get an empty bucket, we throw an error
  if (buckets[daNumber] == nullptr) {
      throw runtime_error("Nothing is in the bucket");
  }
  // use checker to loop through the bucket
  while(checker != nullptr){
    // if we find the key, we return the value
    if (checker->key == key) {
      return checker->value;
    }
    checker = checker->next;
  }
  // if we cant find the key in the bucket, we throw an error
  throw runtime_error("No key exists in the bucket");
}

//
// This function checks if the key is already in the map.
// 
bool hashmap::containsKey(int key) {
    // daNumber holds the bucket number which hold the key
  int daNumber = hashFunction(key) %10;  
  // create a counter key_val_pair to iterate through the list
  key_val_pair * checker = buckets[daNumber];
    // if we get an empty bucket, we return false
  if (buckets[daNumber] == nullptr) {
      return false;
  }
    // use checker to loop through the bucket
  while(checker != nullptr){
        // if we find the key, we return true
    if (checker->key == key) {
      return true;
    }
    checker = checker ->next;
  }
    //if we cant find the key in the bucket, we return false
  return false;
}

//
// This method goes through all buckets and adds all keys to a vector.
//
vector<int> hashmap::keys() const {
  // create a vector which will hold all the keys
  vector <int> allKeys;
  // use for loop to go through each bucket in the hashmap
  for (int i = 0; i < nBuckets; i++) {
    // create counter key_val_pair to go through each key_val_pair in the bucket
    key_val_pair * holder = buckets[i];
    while (holder != nullptr) {
      // push the key into the vector
      allKeys.push_back(holder->key);
      // go to next key_val_pair
      holder = holder->next ;
    }
    }
  // return vector
    return allKeys;
}

//
// The hash function for hashmap implementation.
// For an extension, you might want to improve this function.
//
// @param input - an integer to be hashed
// return the hashed integer
//
int hashmap::hashFunction(int input) const {
    // use unsigned integers for calculation
    // we are also using so-called "magic numbers"
    // see https://stackoverflow.com/a/12996028/561677 for details
    unsigned int temp = ((input >> 16) ^ input) * 0x45d9f3b;
    temp = (temp >> 16) ^ temp;

    // convert back to positive signed int
    // (note: this ignores half the possible hashes!)
    int hash = (int) temp;
    if (hash < 0) {
        hash *= -1;
    }

    return hash;
}

//
// This function returns the number of elements in the hashmap.
//
int hashmap::size() {
    return nElems;
}

//
// Copy constructor
//
hashmap::hashmap(const hashmap &myMap) {
    // make a deep copy of the map
    nBuckets = myMap.nBuckets;

    buckets = createBucketArray(nBuckets);

    // walk through the old array and add all elements to this one
    vector<int> keys = myMap.keys();
    for (size_t i=0; i < keys.size(); i++) {
        int key = keys[i];
        int value = myMap.get(key);
        put(key,value);
    }

}

//
// Equals operator.
//
hashmap& hashmap::operator= (const hashmap &myMap) {
    // make a deep copy of the map

    // watch for self-assignment
    if (this == &myMap) {
        return *this;
    }

    // if data exists in the map, delete it
    for (int i=0; i < nBuckets; i++) {
        hashmap::key_val_pair* bucket = buckets[i];
        while (bucket != nullptr) {
            // walk the linked list and delete each node
            hashmap::key_val_pair* temp = bucket;
            bucket = bucket->next;
            delete temp;
        }
        buckets[i] = nullptr;
    }
    nElems = 0;
    // walk through the old array and add all elements to this one
    vector<int> keys = myMap.keys();
    for (size_t i=0; i < keys.size(); i++) {
        int key = keys[i];
        int value = myMap.get(key);
        put(key,value);
    }
    // return the existing object so we can chain this operator
    return *this;
}

//
// This function overloads the << operator, which allows for ease in printing
// to screen or inserting into a stream, in general.
//
ostream &operator<<(ostream &out, hashmap &myMap) {
    out << "{";
    vector<int> keys = myMap.keys();
    for (size_t i=0; i < keys.size(); i++) {
        int key = keys[i];
        int value = myMap.get(key);
        out << key << ":" << value;
        if (i < keys.size() - 1) { // no commas after the last one
            out << ", ";
        }
    }
    out << "}";
    return out;
}

//
// This function overloads the >> operator, which allows for ease at extraction
// from streams/files.
//
istream &operator>>(istream &in, hashmap &myMap) {
    // assume the format {1:2, 3:4}
    bool done = false;
    in.get(); // get the first char, {
    int nextChar = in.get(); // get the first real character
    while (!done) {
        string nextInput;
        while (nextChar != ',' and nextChar != '}') {
                nextInput += nextChar;
                nextChar = in.get();
        }
        if (nextChar == ',') {
            // read the space as well
            in.get(); // should be a space
            nextChar = in.get(); // get the next character
        } else {
            done = true; // we have reached }
        }
        // at this point, nextInput should be in the form 1:2
        // (we should have two integers separated by a colon)
        // BUT, we might have an empty map (special case)
        if (nextInput != "") {
            //vector<string> kvp;
            size_t pos = nextInput.find(":");
            myMap.put(stoi(nextInput.substr(0, pos)),
                      stoi(nextInput.substr(pos+1, nextInput.length() - 1)));
        }
    }
    return in;
}



