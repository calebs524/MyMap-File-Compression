/*
Project 6
File- util.h
Caleb Satvedi
Class: Cs 251 Sp 2022
Description: This file is the for the project,
which will hold all the functions to manipulate the files and the strings we input.
We will be implementing huffman encoding practices
in order to get the files and strings compressed into binary encoding.
I have made an example of this in secretmessage.txt.huf.
*/

#include <iostream>
#include <fstream>
#include <map>
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater
#include <string>
#include "bitstream.h"
#include "hashmap.h"
#include "mymap.h"
#pragma once


struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;
    HuffmanNode* one;
};
// we create a prioritize class which will be used in the build encoding tree
// function to help a priority queue
class prioritize {
  public:
// takes in 2 huffmannodes and compares their values
bool operator()(const HuffmanNode *  p1, const HuffmanNode * p2) {
    // priority queue returns below comparison and will
    // change ordering of queue based of such output
    return p1->count > p2->count;
  }
};

/*
The below two functions are used to free the nodes The freeRecurisve is called
recurisvely and takes in the current node. The freeTree is called once and 
takes in the root node.
*/
void freeRecursive(HuffmanNode * curr) {
  // if we hit a null node, we go back
  if (curr == nullptr) {
      return;
    }
  freeRecursive(curr -> zero);
  freeRecursive(curr->one);
  delete curr;
}

void freeTree(HuffmanNode* node) {
  // base case if the root node is null
    if (node == nullptr) {
      return;
    }
  // call to recursive function
  freeRecursive(node);
}

//
// *This function build the frequency map.  If isFile is true, then it reads
// from filename.  If isFile is false, then it reads from a string filename.
//
void buildFrequencyMap(string filename, bool isFile, hashmap &map) {
  // create an ifstream object
  ifstream inFS(filename);
  // char created to iterate through the file
char c;
  // we are dealing with a file
  if (isFile) {
    // get each character in the file
while (inFS.get(c)) {
  int ascii = (int) c;
  if (map.containsKey(ascii)) {
    map.put(ascii, map.get(ascii)+1);    
  } else {
    map.put(ascii, 1);
  }  }
    map.put(PSEUDO_EOF, 1);
    } else {
    // we are dealing with a string
    for (char c : filename) {
      // convert character to ascii
        int ascii = (int) c;
      // put character in map
  if (map.containsKey(ascii)) {
    // if we already have the ascii in the map, increment count by 1
    map.put(ascii, map.get(ascii)+1);
  } else {
    // if we don't have ascii in map, put it in with 1 count
    map.put(ascii, 1);
  }  }
    // manually put in the end of file character in map
    map.put(PSEUDO_EOF, 1);
      }  }



//
// *This function builds an encoding tree from the frequency map.
//
HuffmanNode* buildEncodingTree(hashmap &map) {
  // create priority queue to hold huffman nodes
  priority_queue<
  HuffmanNode *,          // huffman node
  vector<HuffmanNode *>,  // store huffman nodes in a vector
  prioritize> pq;         // function object declared at beginning of file
  // get all keys in form of vector of ints
  vector <int> keys = map.keys();
  // go through each key in the vector
  for (int i = 0; i < keys.size(); i++) {
    // create new huffman node holder
    // put in information in huffman node
    // set pointers accordingly to nullptr
     HuffmanNode * holder = new HuffmanNode;
      holder->character = keys[i];
      holder -> count = map.get(keys[i]);
      holder -> zero = nullptr;
      holder -> one = nullptr;
    // put huffmannode into priority queue
    // it will sort it self out
      pq.push(holder);
  }
  // create huffmannode tree
  // go till we have one huffmannode left in the priority queue
  while (pq.size() > 1) {
    // get first two huffman nodes from queue
    HuffmanNode * first = pq.top();
    pq.pop();
    HuffmanNode * second = pq.top();
    pq.pop();
    // make new huffmannode and have them link
    // to first two nodes we popped from top
    HuffmanNode * nac = new HuffmanNode;
    nac -> character = NOT_A_CHAR;
    nac -> count = first->count + second->count;
    nac -> zero = first;
    nac -> one = second;
    // push new huffmannode to the priority queue
    pq.push(nac);
  }
  // take last huffmannode off the priority queue
  HuffmanNode * become = pq.top();
  // return huffmannode
  return become;
}

// following two functions work with eachother to build mymap
// recurusiveBuilder recuvsiveley goes through the huffmannode tree
// and adds the path to get to the mymap encodingMap
void recursiveBuilder(HuffmanNode * tree,
mymap <int,string>& encodingMap, string path) {
  // base case: we reached the end of a path and found a leaf node
  if (tree -> character != NOT_A_CHAR) {
    // if we already have the leaf node in mymap
    if (encodingMap.contains((int)tree -> character)) {
      return;
    } else {
      // we don't have leaf node in mymap
    encodingMap.put((int)tree -> character, path);
    return;}
  }
  // recursive calls, with zero and one paths
  // path string accordingly changed based of zero and one
  recursiveBuilder(tree -> zero, encodingMap, path+"0");
  recursiveBuilder(tree -> one, encodingMap, path+"1");
}

// buildEncodingMap creates map we need and call the recursiveBuilder
mymap <int, string> buildEncodingMap(HuffmanNode* tree) {
  // create a map
    mymap <int, string> encodingMap;
  // base case: the tree is empty
    if (tree == nullptr) {
      return encodingMap;
    }
  // create the path-- it is empty at the beginning
  string path = "";
  // call recursive builder
  recursiveBuilder(tree, encodingMap, path);
  // return map
    return encodingMap;
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream& input, mymap <int, string> &encodingMap, ofbitstream& output, int &size, bool makeFile) {
  // create a string which needs to be returned
    string encoded = "";
  // use character which will go through the file
    char c;
    while (input.get(c)) {
      // add each character to the string
    encoded += encodingMap.get(c);
  }
  // add end of file character to string
  encoded += encodingMap.get(PSEUDO_EOF);
  // if makeFile is true, we do this
  if (makeFile == true) {
    // go through each character int the encoded string
        for (char x : encoded) {
          // write the correct bit to output based
          // off what character we have
      if (x == '0') {
        output.writeBit(0);
      } else {
        output.writeBit(1);
      }
    }
  }
  // we size to encoded string's size
  size = encoded.size();
  // return the string
  return encoded;
}

//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
string decode(ifbitstream &input, HuffmanNode* encodingTree, ofstream &output) {
  // create a string which will be returned
  string decode = "";
  // create a counter huffman node to go through the tree
    HuffmanNode * curr = encodingTree;
    while (!input.eof()) {
      // keep going until we hit leaf node
      while (curr->character == NOT_A_CHAR) {
        // bit from input tells us where to go
        int bit = input.readBit();
        if (bit == 0) {
          curr = curr -> zero;
        } else {
          curr = curr -> one;
        } }
      // if we hit end of file character, we leave
      if (curr -> character == PSEUDO_EOF) {
        break;
      }
      // add leaf node character to decode
      decode += curr -> character;
      // return curr counter to root of encoding tree
      curr = encodingTree;
      }
  // copy each character from decode string to output
  for (char x : decode) {
    output.put(x);
    }
  // return string
    return decode;
}

//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
string compress(string filename) {
  // create an ifstream object to read the file
  ifstream instream(filename);
  // add huf to read file for ofbitstream object
  ofbitstream output(filename +".huf");
  // create a hashmap and populate it with buildFrequencyMap function
  hashmap frequencyMap;
  buildFrequencyMap(filename, true, frequencyMap);
  output << frequencyMap;
  // create huffmannode from buildEncodingTree function
  HuffmanNode * curr = buildEncodingTree(frequencyMap);
  // create mymap from buildEncodingMap funciton
  mymap<int,string> encodingMap = buildEncodingMap(curr);
  int size = 0;
  // get the endocded string from encode function
  string encoded = encode(instream, encodingMap, output, size, true);
  // delete huffman node
  freeTree(curr);
  // return the string
  return encoded;
}

//
// *This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note: this function should reverse what the compress
// function did.
//
string decompress(string filename) {
  // create ifbitstream to read file
  ifbitstream input(filename);
  // change name so file can be read from ofstream
  filename = filename.substr(0, filename.size()-8);
  filename +="_unc.txt";
  ofstream output(filename);
  // create a frequencyMap
  hashmap frequencyMap;
  filename = filename.substr(0, filename.size()-8);
  filename += ".txt";
  // change name and populate frequencyMap
  input >> frequencyMap;
  filename += ".huf";
  // change name and create huffman node from buildEncodingTree
  HuffmanNode * curr = buildEncodingTree(frequencyMap);
  // get the string from decode function
  string decompressed = decode(input, curr, output);
  // free huffman node
  freeTree(curr);
  // return the decompressed string
  return decompressed;  
}
