#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;

SMIterator::SMIterator(const SortedMap& m) : map(m),elem(m.head)
{}

void SMIterator::first(){
	this->elem = this->map.head;
}

void SMIterator::next(){
	if (this->valid())
		this->elem = this->map.nodes[elem].next;
	else
		throw exception("Stop iteration \n");
}

bool SMIterator::valid() const{
	if (this->elem != NULL_TVALUE)
		return true;
	return false;
}

TElem SMIterator::getCurrent() const{
	if (this->valid())
		return this->map.nodes[elem].info;
	throw exception ("Invalid element");

}



void SMIterator::previous()
{
	if (this->valid())
		this->elem = this->map.nodes[elem].prev;
	else
		throw exception("Invalid element");
}

void SMIterator::last()
{
	this->elem = this->map.tail;
}
