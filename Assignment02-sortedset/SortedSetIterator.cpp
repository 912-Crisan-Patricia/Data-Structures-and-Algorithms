#include "SortedSetIterator.h"
#include <exception>

using namespace std;

SortedSetIterator::SortedSetIterator(const SortedSet& m) : multime(m)
{
	this->currentnode = m.head;

}


void SortedSetIterator::first() 
{
	currentnode = multime.head;
}


void SortedSetIterator::next()
{
	if(currentnode== nullptr)
		throw exception("Invalid current node");
	currentnode = currentnode->next;
}


TElem SortedSetIterator::getCurrent()
{
	if(currentnode == nullptr)
		throw exception("Invalid current node");
	TElem elem = currentnode->value;
	return elem;
}

bool SortedSetIterator::valid() const {
	if (currentnode == NULL)
		return false;
	return true;
}

