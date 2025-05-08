#include "SMMIterator.h"
#include "SortedMultiMap.h"

// Complexity
// θ(1)
SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d){
	this -> crtElem = map.head;
}

// Complexity
// θ(1)
void SMMIterator::first(){
	this -> crtElem = map.head;
}

// Complexity
// θ(1)
void SMMIterator::next(){
	if (!valid()) {
		throw std::exception();
	}
	crtElem = map.nodes[crtElem].next;
}

// Complexity
// θ(1)
bool SMMIterator::valid() const{
	return crtElem != -1;
}

// Complexity
// θ(1)
TElem SMMIterator::getCurrent() const{
	if (!valid()) {
		throw std::exception();
	}
	return map.nodes[crtElem].data;
}

