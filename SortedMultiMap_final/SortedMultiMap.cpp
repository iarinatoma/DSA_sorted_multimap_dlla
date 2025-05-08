#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include<algorithm>
#include <exception>
using namespace std;

// Complexity
// θ(n), n = capacity
SortedMultiMap::SortedMultiMap(Relation r) {
	this -> relation = r;
    this -> capacity = 10;
    this -> length = 0;
	this -> head = -1;
	this -> tail = -1;
	this -> firstEmpty = 0;
    this -> nodes = new Node[capacity];
    // creating the initial links between the nodes
	for (int i = 0; i < capacity - 1; i++) {
		nodes[i].next = i + 1;
		nodes[i].previous = -1;
		nodes[i].data = NULL_TELEM;
	}
	nodes[capacity - 1].next = -1;
	nodes[capacity - 1].previous = -1;
}

// Complexity
// θ(1)
int SortedMultiMap::allocate() {
	int newElem = this -> firstEmpty;
   	// if the array is not full, we can allocate a new node
    if (newElem != -1) {
      	// updating the first empty node
    	this -> firstEmpty = this -> nodes[this -> firstEmpty].next;

        if (this -> firstEmpty != -1)
        	this -> nodes[this -> firstEmpty].previous = -1;

        this -> nodes[newElem].next = -1;
    	this -> nodes[newElem].previous = -1;

    }
	// if there are no empty nodes, we need to resize the array
    return newElem;
}

// Complexity
// θ(1)
void SortedMultiMap::free(int pos) {
	nodes[pos].next = this -> firstEmpty; // pozitia de aici pe care vreau sa o golesc o leg de fostul firstEmpty
	nodes[pos].previous = -1; // inainte nu mai este niciun alt firstEmpty
	firstEmpty = pos; // numesc firstEmpty noua pozitie pe care am golit-o
}

// Complexity
// θ(n), n = newCapacity
void SortedMultiMap::sizeUp() {
	// Dublam capacitatea pana avem destul spatiu
	while (this -> length >= this -> capacity)
		this -> capacity *= 2;

	//Alocam noul array cu noua capacitate
	Node* newNodes = new Node[this -> capacity];

	// Copiem nodurile existente, doar cele cu date
	for (int i = 0; i < this -> length; i++)
		newNodes[i] = this -> nodes[i]; // Copiem datele

	this -> firstEmpty = this -> length;

	newNodes[capacity - 1].previous = -1;
	newNodes[capacity - 1].next = -1;

	// Initializam lista de spatii libere
	for (int i = this -> firstEmpty; i < this -> capacity - 1; i++) {
		newNodes[i].next = i + 1;
		newNodes[i].previous = -1;
	}

	// Eliberăm memoria veche și actualizăm legaturile
	delete[] this->nodes;
	this -> nodes = newNodes;
}

// Complexity
// θ(n), n = newCapacity
void SortedMultiMap::sizeDown() {
	if (capacity <= 10 || length * 4 > capacity)  // nu micșorăm prea devreme
		return;

	int newCapacity = capacity / 2;
	Node* newNodes = new Node[newCapacity];

	int current = head;
	int newIndex = 0;
	int newHead = -1;
	int newTail = -1;

	// Copiem doar nodurile active
	while (current != -1 && newIndex < newCapacity) {
		newNodes[newIndex].data = nodes[current].data; //transfer data din nodes[current] current = head,..,tail

		// setam legatura previous
		if (newIndex > 0)
			newNodes[newIndex].previous = newIndex - 1; //setez previous daca nu s pe prima pozitie (sa fie mereu pe indexu de dinainte)
		else
			newNodes[newIndex].previous = -1;

		//  Setăm legătura next
		if (nodes[current].next == -1) {  // Dacă e ultimul nod
			newNodes[newIndex].next = -1;
		} else {
			newNodes[newIndex].next = newIndex + 1;
		}

		//Actualizez head si Tail
		if (newIndex == 0) {
			newHead = newIndex;
		}
		newTail = newIndex;

		// Trec la urmatorul nod
		current = nodes[current].next;
		newIndex++;
	}

	// Initializam lista de spatii libere
	for (int i = newIndex; i < newCapacity; i++) {
		newNodes[i].next = i + 1;
		newNodes[i].previous = -1;
	}

	// Marcam sfarsitul listei de spatii libere
	newNodes[newCapacity - 1].next = -1;

	// Eliminam memoria veche si actualizam structura
	delete[] nodes;
	nodes = newNodes;
	capacity = newCapacity;
	head = newHead;
	tail = newTail;
	firstEmpty = newIndex;
}

// Complexity
// O(n) - overall complexity
// θ(1) - best case complexity 		<=> adding the first element in the SMM / updating head of array
// θ(n) - worst case complexity     <=> updating tail of array
// θ(n) - average case complexity   <=> adding an element at the correct position given by the relation
void SortedMultiMap::add(TKey c, TValue v) {
	this -> length++;
	// allocating a new node
	int newElem = this -> allocate();
	if (newElem == -1) {
		this -> sizeUp();
		newElem = this -> allocate();
	}
	this -> nodes[newElem].data.first = c;
	this -> nodes[newElem].data.second = v;

  	// adding the first element in the SMM
	if (this -> head == -1) {
          this -> head = newElem;
          this -> tail = newElem;
	}
	// adding an element at the correct position given by the relation
    else {
    	  // array iteration
          int crrtPos = this -> head;
          while (crrtPos != -1 && !this -> relation(c, this -> nodes[crrtPos].data.first)) {
            	crrtPos = this -> nodes[crrtPos].next;
          }

          // updating head
          if (crrtPos == this -> head) {
				this -> nodes[newElem].next = this -> head;
				this -> nodes[this -> head].previous = newElem;
                this -> head = newElem;
          }

          // updating tail
          else if (crrtPos == -1) {
				this -> nodes[newElem].previous = this -> tail;
                this -> nodes[this -> tail].next = newElem;
                this -> tail = newElem;
          }
          // updating the links between the nodes
          else {
          		this -> nodes[this -> nodes[crrtPos].previous].next = newElem;
				this -> nodes[newElem].next = crrtPos;
                this -> nodes[newElem].previous = this -> nodes[crrtPos].previous;
          		this -> nodes[crrtPos].previous = newElem;

          }
    }
}

// Complexity
// O(n) - overall complexity
// θ(1) - best case complexity		<=> no elem to delete, relation not fulfilled / first elem to delete, linear search => θ(1)
// θ(n) - average case complexity	<=> equal probability that the elem to be removed is on any position
// θ(n) - worst case complexity		<=> the last elem shoul be deleted
bool SortedMultiMap::remove(TKey c, TValue v) {

	int current = this->head;
	while (current != -1) {
		if (this -> relation(c, this -> nodes[current].data.first) && this -> nodes[current].data.first != c)
			 // Since the list is sorted and we've passed the target key, stop early
			break;

		if (this -> nodes[current].data.first == c && this -> nodes[current].data.second == v) {
			int prev = this->nodes[current].previous;
			int next = this->nodes[current].next;


			if (prev != -1 && next != -1) { // am ceva si inainte si dupa
				nodes[prev].next = next;
				nodes[next].previous = prev;
			} else if (prev == -1 && next != -1) { // nu am ceva inainte da am dupa
				head = next;
				nodes[next].previous = -1;
			} else if (prev != -1 && next == -1) { // am ceva inainte da nu am dupa
				tail = prev;
				nodes[prev].next = -1;
			} else {  // prev == -1 && next == -1
				head = -1;
				tail = -1;
			}

			free(current); // dupa ce stergem eliberam noul firstEmpty
			length--;
			sizeDown();
			return true;
		}
		current = this->nodes[current].next;
	}
	return false;
}

// Complexity
// θ(n)
vector<TValue> SortedMultiMap::search(TKey c) const {
	vector<TValue> values;
	int current = head;
	while (current != -1 && current < capacity) {
		if (nodes[current].data.first == c)
			values.push_back(nodes[current].data.second);
		current = nodes[current].next;
	}
	return values;
}

// Complexity
// θ(1)
int SortedMultiMap::size() const {
	return this -> length;
}

// Complexity
// θ(1)
bool SortedMultiMap::isEmpty() const {
	if (this -> length == 0)
          return true;
	return false;
}

// Complexity
// θ(1)
SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}

// Complexity
// θ(1)
SortedMultiMap::~SortedMultiMap() {
	delete[] nodes;
}

// Complexity
// O(n) - overall complexity
// θ(1) - best case complexity
// θ(n) - worst case complexity
// θ(n) - average case complexity
bool SortedMultiMap::isEqual(const SortedMultiMap& other_smm) {
	if (this->length != other_smm.length)
		return false;

	vector<TElem> elems1;
	vector<TElem> elems2;

	int current = this->head;
	while (current != -1) {
		elems1.push_back(this->nodes[current].data);
		current = this->nodes[current].next;
	}

	int other_current = other_smm.head;
	while (other_current != -1) {
		elems2.push_back(other_smm.nodes[other_current].data);
		other_current = other_smm.nodes[other_current].next;
	}

	sort(elems1.begin(), elems1.end());
	sort(elems2.begin(), elems2.end());

	for (int i = 0; i < elems1.size(); ++i) {
		if (elems1[i] != elems2[i])
			return false;
	}

	return true;
}

/*

algoritm isEqual(const SortedMultiMap& other_smm)
	daca (this->length != other_smm.length) atunci
		isEqual<-false;

	vector<TElem> elems1;
	vector<TElem> elems2;

	int current <- head;
	cat timp (current != -1) repeta
		elems1.push_back(this->nodes[current].data);
		current = this->nodes[current].next;

	int other_current <- other_smm.head;
	cat timp (other_current != -1) repeta
		elems2.push_back(other_smm.nodes[other_current].data);
		other_current = other_smm.nodes[other_current].next;

	elems1.sort();
	elems2.sort();

	cat timp i <- 0,elems1.size() repeta
		daca (elems1[i] != elems2[i]) atunci
			IsEqual <- false;

	isEqual <- true;

 */