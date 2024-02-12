#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>
using namespace std;


Node::Node() {}

Node::Node(TElem info, int next, int prev) : info{ info }, next{ next }, prev{ prev } {}

Node::~Node() {}


SortedMap::SortedMap(Relation r) {
	this->capacity = 10;
	this->head = NULL_TVALUE;
	this->tail = NULL_TVALUE;
	this->length = 0;
	this->rel = r;
	this->nodes = new Node[capacity];
	
	
	for (int i = 0; i < capacity - 1; i++)
		nodes[i].next =  i + 1;
	this->nodes[capacity - 1].next = NULL_TVALUE;
	for (int i = 1; i < capacity; i++)
		this->nodes[i].prev = i - 1;
	this->nodes[0].prev = NULL_TVALUE;
	this->firstempty = 0;


}

//used to allocate new elem and return its position
int SortedMap::allocate() //O(1)
{
	int newelem = this->firstempty;
	if (newelem != NULL_TVALUE)
	{
		this->firstempty = this->nodes[this->firstempty].next;
		if (this->firstempty != NULL_TVALUE) {
			this->nodes[this->firstempty].prev = NULL_TVALUE;
		}
		this->nodes[newelem].next = NULL_TVALUE;
		this->nodes[newelem].prev = NULL_TVALUE;
	}
	return newelem;
}

//void SortedMap::free(int position) { 
//	this->nodes[position].next = this->firstempty;
//	this->nodes[position].prev = NULL_TVALUE;
//	if (this->firstempty != NULL_TVALUE) {
//		this->nodes[this->firstempty].prev = position;
//	}
//	this->firstempty = position;
//}

void SortedMap::resize() { //O(n) Theta(n)
	Node* newElems = new Node[this->capacity * 2];
	//make copy of initial elements
	for (int i = 0; i < this->capacity; i++) {
		newElems[i] = this->nodes[i];
	}
	for (int i = this->capacity; i < this->capacity * 2 - 1; i++) {
		newElems[i].next = i + 1;
	}
	newElems[2 * this->capacity - 1].next = NULL_TVALUE;
	for (int i = this->capacity + 1; i < this->capacity * 2; i++) {
		newElems[i].prev = i - 1;
	}
	newElems[this->capacity].prev = NULL_TVALUE;
	this->firstempty = this->capacity;
	this->capacity *= 2;
	delete[] this->nodes;
	this->nodes = newElems;
}


TValue SortedMap::add(TKey k, TValue v) { //BC-Theta(1) - when in add we already have pair with same key in head or map empty ,
										// WC-Theta(capacity) -when we have to add at last the Map, AC-Theta(capacity) the element to be added in betweem head and tail ==> O(n)
	/*TElem new_elem;
	new_elem.first = k;
	new_elem.second = v;*/

	TValue old_value;
	int current = this->head;

	//the key already exists in the map
	if (this->search(k) != NULL_TVALUE) { 
		while (current != NULL_TVALUE && this->nodes[current].info.first != k) {
			current = this->nodes[current].next;
		}
		old_value = this->nodes[current].info.second; 
		this->nodes[current].info.second = v;
		return old_value;
	}
	else {
	//the key is not in sorted map
		int new_elem = this->allocate();
		if (new_elem == NULL_TVALUE) {
			this->resize();
			new_elem = this->allocate();
		}
		this->nodes[new_elem].info.first = k;
		this->nodes[new_elem].info.second = v;

		//the list is empty ==> just add the element
		if (this->length == 0) { 
			if (this->head == NULL_TVALUE && this->tail == NULL_TVALUE) {
				this->head = new_elem;
				this->tail = new_elem;
				this->length++;
				return NULL_TVALUE;
			}
		}
		else {
			//before head or between head and tail
			if (this->rel(this->nodes[this->head].info.first, k) == false) { 
				this->nodes[new_elem].prev = NULL_TVALUE;
				this->nodes[new_elem].next = this->head;
				this->nodes[this->head].prev = new_elem;
				this->head = new_elem;
				this->length++;
				return NULL_TVALUE;
			}
			else {
				if (this->length == 1) {
					this->nodes[this->head].next = new_elem;
					this->nodes[new_elem].next = NULL_TVALUE;
					this->nodes[this->head].prev = this->head;
					this->tail = new_elem;
					this->length++;
					return NULL_TVALUE;
				}
			}
			//after tail
			if (this->rel(this->nodes[this->tail].info.first, k) == true) { 
				this->nodes[new_elem].prev = this->tail;
				this->nodes[new_elem].next = NULL_TVALUE;
				this->nodes[this->tail].next = new_elem;
				this->length++;
				this->tail = new_elem;
				return NULL_TVALUE;
			}
			while (current != NULL_TVALUE && this->rel(this->nodes[this->nodes[current].next].info.first, k) != false) {
				current = this->nodes[current].next;
			}
			this->nodes[new_elem].prev = current;
			this->nodes[new_elem].next = this->nodes[current].next;
			this->nodes[current].next = new_elem;
			this->length++;
			return NULL_TVALUE;
		}
	}
}

TValue SortedMap::search(TKey k) const { // BC - THeta(capacity)  ,WC-Theta(capacity) , AC-Theta(capacity) , ==> O(n)
	int current = this->head;
	while (current != NULL_TVALUE && this->nodes[current].info.first != k)
	{
		current  = this->nodes[current].next;
	}
	if (current != NULL_TVALUE)
		return this->nodes[current].info.second;
	return NULL_TVALUE;
}

TValue SortedMap::remove(TKey k) { // BC-Theta(1) when key is in head , WC- Theta(capacity) when key in last element or is not found , AC - Theta(capacity) when key is in between 
	if (this->search(k) != NULL_TVALUE) {
		int current = this->head;
		int previous = NULL_TVALUE;
		TValue old_value;
		while (current != NULL_TVALUE && this->nodes[current].info.first != k) { //find position of key in order to return old value from pair
			previous = current;
			current = this->nodes[current].next;
		}
		if (current != NULL_TVALUE) {
			//case when key in head ==> we need to remove the head
			if (current == this->head && previous == NULL_TVALUE) {
				old_value = this->nodes[current].info.second;
				this->head = this->nodes[current].next;
			}
			//case when key in between ==> we need to link the remaining elems
			else {
				old_value = this->nodes[current].info.second;
				this->nodes[previous].next = this->nodes[current].next;
			}
			this->nodes[current].next = this->firstempty;
			this->firstempty = current;
			this->length--;
			return old_value;
		}
	}
	else {
		return NULL_TVALUE;
	}
}

int SortedMap::size() const { // Theta(1);
	return this->length;
}

bool SortedMap::isEmpty() const { //Theta(1)
	if (this->length == 0)
		return true;
	return false;
}

SMIterator SortedMap::iterator() const {
	return SMIterator(*this);
}

SortedMap::~SortedMap() {
}
