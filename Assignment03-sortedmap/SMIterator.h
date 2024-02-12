#pragma once
#include "SortedMap.h"

//DO NOT CHANGE THIS PART
class SMIterator{
	friend class SortedMap;
private:
	const SortedMap& map;
	SMIterator(const SortedMap& mapionar);

	int elem;

public:

	void first();
	void next();
	bool valid() const;
	void previous();
	void last();
    TElem getCurrent() const;
};

