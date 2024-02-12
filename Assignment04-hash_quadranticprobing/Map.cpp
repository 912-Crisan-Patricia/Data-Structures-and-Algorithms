#include "Map.h"
#include "MapIterator.h"
#include <exception>
#include <iostream>
#include <stdlib.h>


using namespace std;

int Map::hash(TKey k, int i) const
{
	int result1 = abs(k) % this->capacity + 0.5 * i + 0.5 * i * i;
	int result = result1 % this->capacity;
	return result;
}


Map::Map() {
	this->capacity = 8;// if m is a power of 2 , and c1,c2 = 0.5 ==> probe sequence will always be a permutation (lecture 8)
	this->length = 0;
	this->elems = new TElem[this->capacity];
	for (int i = 0; i < capacity; i++)
		this->elems[i] = NULL_TELEM;
}

Map::~Map() {
	delete[] this->elems;
}


//BestCase - Theta(1) - when the element is found on the first position
//WorstCase - Theta(capacity) - when the element is found on the last position
//AverageCase - Theta(capacity) - when the element is found on the middle position ==> O(n)
// when resize is needed, the complexity becomes the complexity of the resize function
// (can reach theta(capacity^2), but rarely)
TValue Map::add(TKey c, TValue v){
	
	TValue prevvalue = NULL_TVALUE;
	int i = 0;
	int pos =this-> hash(c, i);
	
	while (i < this->capacity && this->elems[pos] != NULL_TELEM && this->elems[pos] != DELETED && this->elems[pos].first != c)
	{
		i++;
		pos = hash(c, i);
	}
	if (i == this->capacity)
	{
		//resize and rehash 
		this->resize();

		int i = 0;
		int pos = this->hash(c, i);
		while (i < this->capacity && this->elems[pos] != NULL_TELEM && this->elems[pos] != DELETED && this->elems[pos].first != c)
		{
			//search until we find an empty spot (deleted or empty)
			i++;
			pos = hash(c, i);
		}
		pair<TKey, TValue> newPair(c, v);
		this->elems[pos] = newPair;
	}
	else
	{
		//memorise value for return
		if (this->elems[pos] == DELETED)
			prevvalue = NULL_TVALUE;
		else
			prevvalue = this->elems[pos].second;


		pair<TKey, TValue> newPair(c, v);
		this->elems[pos] = newPair;
	}

	if (prevvalue == NULL_TVALUE)
		this->length++;
	return prevvalue;
}



// BestCase = theta(1) WorstCase = theta(m) AC = theta(1) overall O(m)
// the average case is theta(1) because the load factor(length/capacity) is < 1

TValue Map::search(TKey c) const{
	int i = 0;
	int pos = this->hash(c, i);

	while (i < this->capacity && this->elems[pos]!= NULL_TELEM && this->elems[pos].first!=c)
	{
		i++;
		pos = this->hash(c, i);

	}
	if (this->elems[pos].first == c)
		return this->elems[pos].first;

	return NULL_TVALUE;
	
}


// BestCase = theta(1) WorstCase = theta(capacity) AC = theta(1) overall O(capacity)
// the average case is theta(1) because the load factor(n/m) is < 1
TValue Map::remove(TKey c){

	int i = 0;
	int pos = hash(c, i);

	while (i < this->capacity && this->elems[pos].first != c && this->elems[pos] != NULL_TELEM)
	{
		// we keep calculating position until we found the one having the element that need to be rmeoved
		i++;
		pos = this->hash(c, i);
	}

	if (this->elems[pos].first == c)
	{
		// if the element is found, it is marked as deleted
		this->length--;
		TValue prevvalue = this->elems[pos].second;
		this->elems[pos] = DELETED;
		return prevvalue ;
	}

	return NULL_TVALUE;

}


int Map::size() const {
	return this->length;
}

bool Map::isEmpty() const{
	if (this->length == 0)
		return true;
	return false;
}

MapIterator Map::iterator() const {
	return MapIterator(*this);
}


// BestCase= theta(capacity) WC = theta(capacity^2) AC = theta(capacity) overall O(capacity^2)
// the wc is given by the addition of elements in the new dynamic array
void Map::resize()
{

	this->capacity *= 2;
	TElem* newElems = new TElem[this->capacity];

	//initialize new elements with NULL_TElem
	for (int index = 0; index < this->capacity; index++)
		newElems[index] = NULL_TELEM;

	for (int index = 0; index < this->capacity / 2; index++)
	{
		int i = 0;
		int pos = this->hash(this->elems[index].first, i);

		while (i < this->capacity && newElems[pos] != NULL_TELEM && newElems[pos].first != this->elems[index].first)
		{
			i++;
			pos = this->hash(this->elems[index].first, i);

		}
		newElems[pos] = this->elems[index];
	}

	delete this->elems;
	this->elems = newElems;
}


std::vector<TValue> Map::valueBag() const {
	std::vector<TValue> values;

	for (int i = 0; i <this-> capacity; i++) {
		if (elems[i] != NULL_TELEM && elems[i] != DELETED) {
			values.push_back(elems[i].second);
		}
	}

	return values;
}


