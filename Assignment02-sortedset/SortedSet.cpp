#include "SortedSet.h"
#include "SortedSetIterator.h"

SortedSet::SortedSet(Relation r) {
	this->rel = r;
	this->head = nullptr;
	this->length = 0;
}


bool SortedSet::add(TComp elem) {
	SLLNode* toadd = new SLLNode;
	toadd->value = elem;
	toadd->next = nullptr;

	//case when we first add in the sortedset
	if (this->head == nullptr)
	{
		this->head = toadd;
		this->length++;
		return true;
	}
	else
	{
		//case when element = 1st value
		if (elem == this->head->value)
			return false;

		//case when element < 1st value (from head) ==> to be added in head
		if (this->rel(elem, this->head->value) == true)
		{
			toadd->next = this->head;
			this->head = toadd;
			this->length++;
			return true;
		}
		//case when element > 1st value  
		else
		{
			SLLNode* currentnode;
			currentnode = this->head;

			//search its place and verify along the way if it exists
			{
				while (currentnode->next != nullptr && this->rel(currentnode->next-> value, elem) == true)
				{
					if (currentnode->value == elem)
						return false;
					currentnode = currentnode->next;
				}
			}
			//case when element = last value
			if (elem == currentnode->value)
				return false;
			toadd->next = currentnode->next;
			currentnode->next = toadd;
			this->length++;
			return true;
		}
	}
	return false;
}


bool SortedSet::remove(TComp elem) {
	
	if (search(elem) == true)
	{
		//case when elem is in head
		if (this->head->value == elem)
		{
			/*SLLNode* nextnode;
			nextnode = this->head->next;
			delete this->head;
			this->head = nextnode;*/


			this->head = this->head->next;
			this->length--;
			return true;
		}
		//case when elem is outside head ==> interconnecting neighbor values
		else
		{
			SLLNode* currentnode;
			SLLNode* previousnode = new SLLNode;

			currentnode = this->head;
			while (currentnode != nullptr)
			{
				if (currentnode->value == elem)
				{
					previousnode->next = currentnode->next;
					delete currentnode;
					this->length--;
					return true;
				}
				previousnode = currentnode;
				currentnode = currentnode->next;
			}


		}
	}
	return false;
}


bool SortedSet::search(TComp elem) const {
	if (this->length == 0)
		return false;
	SLLNode* searchnode;
	searchnode = this->head;
	while (searchnode != nullptr )
	{
		if (searchnode->value == elem)
			return true;
		searchnode = searchnode->next;
	}
	return false;
}


int SortedSet::size() const {
	return this->length;
}



bool SortedSet::isEmpty() const {
	if (this->length == 0)
		return true;
	return false;
}

SortedSetIterator SortedSet::iterator() const {
	return SortedSetIterator(*this);
}


SortedSet::~SortedSet() {
	SLLNode* node;
	while (this->head != nullptr)
	{
		node = this->head;
		this->head = this->head->next;
		delete node;
	}
}


