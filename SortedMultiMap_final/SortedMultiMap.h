#pragma once
//DO NOT INCLUDE SMMITERATOR

//DO NOT CHANGE THIS PART
#include <vector>
#include <utility>
typedef int TKey;
typedef int TValue;
typedef std::pair<TKey, TValue> TElem;
#define NULL_TVALUE -111111;
#define NULL_TELEM pair<TKey, TValue>(-111111, -111111);
using namespace std;
class SMMIterator;
typedef bool(*Relation)(TKey, TKey);


class SortedMultiMap {
	friend class SMMIterator;
    private:

		struct Node {
        TElem data;
        int next;
        int previous;
        };

        Node* nodes;
        int capacity;
        int head;
        int tail;
        int firstEmpty;
        int length;

        Relation relation;

		// allocates a new node and returns its index
        int allocate();
        // frees a node at a given position
		void free(int pos);

        // doubles the capacity of the sorted multi map
        void sizeUp();
        // halves the capacity of the sorted multi map
        void sizeDown();



    public:

    // constructor
    SortedMultiMap(Relation r);

	//adds a new key value pair to the sorted multi map
    void add(TKey c, TValue v);

	//returns the values belonging to a given key
    vector<TValue> search(TKey c) const;

	//removes a key value pair from the sorted multimap
	//returns true if the pair was removed (it was part of the multimap), false if nothing is removed
    bool remove(TKey c, TValue v);

    //returns the number of key-value pairs from the sorted multimap
    int size() const;

    //verifies if the sorted multi map is empty
    bool isEmpty() const;

    // returns an iterator for the sorted multimap. The iterator will return the pairs as required by the relation (given to the constructor)
    SMMIterator iterator() const;

	bool isEqual(const SortedMultiMap& other_smm);


    // destructor
    ~SortedMultiMap();

};
