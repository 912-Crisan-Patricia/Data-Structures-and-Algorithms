#include "Map.h"
#include "MapIterator.h"
#include <exception>
using namespace std;


MapIterator::MapIterator(const Map& d) : map(d)
{
	this->current = 0;
	while ((this->map.elems[this->current] == DELETED || this->map.elems[this->current] == NULL_TELEM) && this->valid())
	{
		this->current++;
	}
}


void MapIterator::first() {
	this->current = 0;
	while ((this->map.elems[this->current] == DELETED || this->map.elems[this->current] == NULL_TELEM) && this->valid())
		this->current++;
}


void MapIterator::next() {
	if (this->valid())
		this->current++;
	else
		throw exception("Invalid next");
	while ((this->map.elems[this->current] == DELETED || this->map.elems[this->current] == NULL_TELEM) && this->valid())
	{
		this->current++;
	}
		
}


TElem MapIterator::getCurrent(){
	if (this->valid())
		return this->map.elems[this->current];
	else
		throw exception("Invalid Get Current");
}


bool MapIterator::valid() const {
	if (this->current < this->map.capacity && this->current >=0)
		return true;
	return false;
}







