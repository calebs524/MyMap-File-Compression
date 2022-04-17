// mymap.h
//
// TODO: Caleb Satvedi MyMap.h file, makes a map using nodes and binary tree
#pragma once

#include <iostream>
#include <sstream>
#include <fstream>


using namespace std;

template<typename keyType, typename valueType>
class mymap {
 private:
    struct NODE {
        keyType key;  // used to build BST
        valueType value;  // stored data for the map
        NODE* left;  // links to left child
        NODE* right;  // links to right child
        int nL;  // number of nodes in left subtree
        int nR;  // number of nodes in right subtree
        bool isThreaded;
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of key/value pairs in the mymap

    //
    // iterator:
    // This iterator is used so that mymap will work with a foreach loop.
    //
    struct iterator {
     private:
        NODE* curr;  // points to current in-order node for begin/end

     public:
        iterator(NODE* node) {
            curr = node;
        }

        keyType operator *() {
            return curr -> key;
        }

        bool operator ==(const iterator& rhs) {
            return curr == rhs.curr;
        }

        bool operator !=(const iterator& rhs) {
            return curr != rhs.curr;
        }

        bool isDefault() {
            return !curr;
        }

        //
        // operator++:
        //
        // This function should advance curr to the next in-order node.
        // O(logN)
        //
        NODE* leftNodeIterator(NODE* curr) {
            if (curr == nullptr) {
                return curr;
            }
            return leftNodeIterator(curr -> left); // Iterates to left-most
        }

        iterator operator++() { // Segfault
            if (curr -> isThreaded) {
                curr = curr -> right;
                return iterator(curr);
            } else {
                curr = curr -> right;
                curr = leftNodeIterator(curr);
                return iterator(curr);
            }
        }
        
    };

 public:
    //
    // default constructor:
    //
    // Creates an empty mymap.
    // Time complexity: O(1)
    //
    mymap() {
    	size = 0;
    	root = nullptr;
        // TODO: write this function.
    }
    //helper function for copy constructor/operator- recursive call
    NODE* copyHelper(NODE* other) {
        if (other != nullptr) {
            NODE* newCreate = new NODE();
            newCreate -> left = copyHelper(other -> left);
            newCreate -> key = other -> key;
            newCreate -> value = other -> value;
            newCreate -> left = other -> left;
            newCreate -> right = other -> right;
            newCreate -> nL = other -> nL;
            newCreate -> nR = other -> nR;
            newCreate -> isThreaded = other -> isThreaded;
            newCreate -> right = copyHelper(other -> right);
            return newCreate;
        }
        return nullptr;
    }
    //
    // copy constructor:
    //
    // Constructs a new mymap which is a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap(const mymap& other) {
        this -> root = copyHelper(other.root);
        this -> size = other.size;
        // TODO: write this function.
    }

    //
    // operator=:
    //
    // Clears "this" mymap and then makes a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap& operator=(const mymap& other) {
        this -> clear(); // clears current mymap
        this -> root = nullptr;
        this -> root = copyHelper(other.root);
        this -> size = other.size;
        return *this;  // TODO: Update this return.
    }

    // clear:
    //
    // Frees the memory associated with the mymap; can be used for testing.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    void clear() {
        delete root;
        root = nullptr;
        size = 0;
    }

    //
    // destructor:
    //
    // Frees the memory associated with the mymap.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    ~mymap() {
                if (root) {
            delete(root -> left);
            delete(root -> right);
            delete(root);
        }
        // TODO: write this function.


    }

    //
    // put:
    //
    // Inserts the key/value into the threaded, self-balancing BST based on
    // the key.
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-tree that needs to be re-balanced.
    // Space complexity: O(1)
    //
    void put(keyType key, valueType value) {
    	NODE * prev = nullptr;
    	NODE * curr = root;
    	while(curr != nullptr) {
    		if(key == curr->key) {
                curr -> value = value;
    			return;
    		}
    		if (key < curr->key) {
    			prev = curr;
    			curr = curr-> left;
    		} else {
    			prev = curr;
                if (curr -> isThreaded) {
                    curr = nullptr;
                } else {
    			curr = curr-> right;
                }
    		}
    	}
    	NODE * n = new NODE();
    	n -> key = key;
    	n -> value = value;
    	n -> left = nullptr;
    	n -> right = nullptr;
    	n -> isThreaded = false;
    	n -> nL = 0;
    	n -> nR = 0;
    	if(prev == nullptr) {
    		root = n;
    	}
    	else if(key < prev -> key) {
    		prev -> left = n;
            prev -> nL++;
    	} else {
    		prev -> right = n;
            prev -> nR++;
    	}
    	size++;
        // TODO: write this function.

    }

    //
    // contains:
    // Returns true if the key is in mymap, return false if not.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    bool contains(keyType key) {
    	NODE * curr = root;
    	while(curr != nullptr) {
    	if(key == curr -> key) {
    		return true;
    	}
    	if (key < curr->key) {
    			curr = curr-> left;
    		} else {
            if (curr -> isThreaded) {
                    curr = nullptr;
                } else {
    			curr = curr-> right;
                }
    		}
    	}
        // TODO: write this function.
        return false;  // TODO: Update this return.
    }

    //
    // get:
    //
    // Returns the value for the given key; if the key is not found, the
    // default value, valueType(), is returned (but not added to mymap).
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    valueType get(keyType key) {
    	NODE * curr = root;
    	while(curr != nullptr) {
    	if(key == curr -> key) {
    		return curr-> value;
    	}
    	if (key < curr->key) {
    			curr = curr-> left;
    		} else{
            if (curr -> isThreaded) {
                    curr = nullptr;
                } else {
    			curr = curr-> right;}
    		}
    	}
        // TODO: write this function.
        return valueType();  // TODO: Update this return.
        // TODO: write this function.
        // TODO: Update this return.
    }

    //
    // operator[]:
    //
    // Returns the value for the given key; if the key is not found,
    // the default value, valueType(), is returned (and the resulting new
    // key/value pair is inserted into the map).
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-trees that need to be re-balanced.
    // Space complexity: O(1)
    //
    valueType operator[](keyType key) {
		if(contains(key)){
			return get(key);
		}
		put(key, valueType());
		return valueType();
        // TODO: write this function.

  // TODO: Update this return.
    }

    //
    // Size:
    //
    // Returns the # of key/value pairs in the mymap, 0 if empty.
    // O(1)
    //
    int Size() {
        // TODO: write this function.
        return size;  // TODO: Update this return.
    }

    //
    // begin:
    //
    // returns an iterator to the first in order NODE.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //

        NODE* beginIter(NODE* curr) {
        if (curr -> left == nullptr) {
            return curr;
        }
        return beginIter(curr);  // Iterates to left-most node
    }


    iterator begin() {
        // TODO: write this function.
        NODE* curr = beginIter(this -> root);
        return iterator(curr);  // TODO: Update this return.
    }

    //
    // end:
    //
    // returns an iterator to the last in order NODE.
    // this function is given to you.
    //
    // Time Complexity: O(1)
    //
    iterator end() {
        return iterator(nullptr);
    }




    //
    // toString:
    //
    // Returns a string of the entire mymap, in order.
    // Format for 8/80, 15/150, 20/200:
    // "key: 8 value: 80\nkey: 15 value: 150\nkey: 20 value: 200\n
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    // Helper function to stringIterator
    void stringMaker(stringstream& allInfo, NODE* curr) {
        allInfo << "key: " << curr -> key
        << " value: " << curr -> value << "\n";
    }

    // Helper function to toString
    void stringIterator(NODE* curr, stringstream& allInfo){
        if (curr == nullptr) {
            return;
        }
        stringIterator(curr->left, allInfo);
        stringMaker(allInfo, curr);
        if (curr -> isThreaded) {
            curr = nullptr;
        } else {
            stringIterator(curr -> right, allInfo);
        }
    }

    string toString() {
        NODE* curr = root;
        stringstream allInfo;
        stringIterator(curr, allInfo);
        return allInfo.str();
    }

    //
    // toVector:
    //
    // Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
    // {{8, 80}, {15, 150}, {20, 200}}
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    void toVectorIteration(NODE* node,
    vector<pair<keyType, valueType> >& allInfo) {
        if (node == nullptr) {
            return;
        }
        toVectorIteration(node -> left, allInfo);
        pair<keyType, valueType> addVector;
        addVector.first = node -> key; addVector.second = node -> value;
        allInfo.push_back(addVector);
        toVectorIteration(node -> right, allInfo);
    }


    vector<pair<keyType, valueType> > toVector() {
        NODE* curr = root;
        vector<pair<keyType, valueType> > allInfo;
        toVectorIteration(curr, allInfo);
        return allInfo;  // TODO: Update this return.
    }

    //
    // checkBalance:
    //
    // Returns a string of mymap that verifies that the tree is properly
    // balanced.  For example, if keys: 1, 2, 3 are inserted in that order,
    // function should return a string in this format (in pre-order):
    // "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    void balanceConcat(stringstream& stringSS, NODE* curr) {
        stringSS << "key: " << curr -> key << ", nL: " << curr -> nL
        << ", nR: " << curr -> nR << "\n";
    }

    void checkBalanceIterator(NODE* curr, stringstream& stringSS) {
        if (curr == nullptr) {
            return;
        }
        balanceConcat(stringSS, curr);
        checkBalanceIterator(curr -> left, stringSS);
        checkBalanceIterator(curr -> right, stringSS);
    }

    //
    // checkBalance:
    //
    // Returns a string of mymap that verifies that the tree is properly
    // balanced.  For example, if keys: 1, 2, 3 are inserted in that order,
    // function should return a string in this format (in pre-order):
    // "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    string checkBalance() {
        stringstream stringSS;
        NODE* curr = root;
        checkBalanceIterator(curr, stringSS);
        return stringSS.str();  // TODO: Update this return.
    }
};

