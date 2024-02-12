#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;

SortedMultiMap::SortedMultiMap(Relation r) {
    this->relation = r;
    this->count = 0;
    this->capacity = 1;
    this->amount = 0;
    this->tree = new BSTTree;
    this->tree->key = new TKey[this->capacity];
    this->tree->root = -1;
    this->tree->left = new TKey[this->capacity];
    for (int i = 0; i < this->capacity; i++)
        this->tree->left[i] = -1;
    this->tree->right = new TKey[this->capacity];
    for (int i = 0; i < this->capacity; i++)
        this->tree->right[i] = -1;
    this->tree->parent = new TKey[this->capacity];
    for (int i = 0; i < this->capacity; i++)
        this->tree->parent[i] = -1;
    this->tree->keyValues = new Values[this->capacity];
    for (int i = 0; i < this->capacity; i++) {
        this->tree->keyValues[i].capacity = 1;
        this->tree->keyValues[i].values = new TValue[this->tree->keyValues[i].capacity];
        this->tree->keyValues[i].count = 0;
    }
}
void SortedMultiMap::add(TKey c, TValue v) {

    bool found = false;
    int index = this->tree->root;
    while (index != -1 && !found) {
        if (this->tree->key[index] == c) {
            found = true;
        }
        else if (this->tree->key[index] >= c)
            index = this->tree->left[index];
        else
            index = this->tree->right[index];
    }

    if (found) {
        if (this->tree->keyValues[index].count == this->tree->keyValues[index].capacity) {
            TValue* newKeyValues = new TValue[this->tree->keyValues[index].capacity * 2];
            for (int i = 0; i < this->tree->keyValues[index].count; i++) {
                newKeyValues[i] = this->tree->keyValues[index].values[i];
            }
            delete[] this->tree->keyValues[index].values;
            this->tree->keyValues[index].values = newKeyValues;
            this->tree->keyValues[index].capacity *= 2;
        }
        this->tree->keyValues[index].values[this->tree->keyValues[index].count] = v;
        this->tree->keyValues[index].count++;
        this->amount++;
    }
    else if (this->tree->root == -1)
    {
        this->tree->root = 0;
        this->tree->key[this->tree->root] = c;
        this->count++;
        this->tree->keyValues[this->tree->root].values[0] = v;
        this->tree->keyValues[this->tree->root].count++;
        this->amount++;
    }
    else {
        if (this->count == this->capacity)
        {
            this->resize();
        }
        int key = c;
        TValue* copyValues = new TValue[this->tree->keyValues[this->count].capacity];
        copyValues[0] = v;
        int i = this->count;
        while (i > 0 && !relation(this->tree->key[i - 1], c))
        {
            this->tree->key[i] = this->tree->key[i - 1];
            this->tree->left[i] = this->tree->left[i - 1];
            this->tree->right[i] = this->tree->right[i - 1];
            this->tree->parent[i] = this->tree->parent[i - 1];
            this->tree->keyValues[i] = this->tree->keyValues[i - 1];
            if (this->tree->parent[i] == -1)
                this->tree->root = i;
            else
            {
                if (this->tree->key[this->tree->right[this->tree->parent[i]]] == this->tree->key[i])
                    this->tree->right[this->tree->parent[i]]++;
                else if (this->tree->key[this->tree->left[this->tree->parent[i]]] == this->tree->key[i])
                    this->tree->left[this->tree->parent[i]]++;
            }
            if (this->tree->left[i] != -1)
                this->tree->parent[this->tree->left[i]] ++;
            if (this->tree->right[i] != -1)
                this->tree->parent[this->tree->right[i]]++;


            i--;
        }
        int position = this->tree->root, previousPos;
        while (position != -1)
        {
            previousPos = position;
            if (this->tree->key[position] >= c)
                position = this->tree->left[position];
            else
                position = this->tree->right[position];
        }
        this->tree->key[i] = key;
        this->tree->right[i] = -1;
        this->tree->left[i] = -1;
        this->tree->parent[i] = previousPos;
        this->tree->keyValues[i].values = copyValues;
        this->tree->keyValues[i].capacity = 1;
        this->tree->keyValues[i].count = 1;
        if (this->tree->key[previousPos] >= c)
            this->tree->left[previousPos] = i;
        else
            this->tree->right[previousPos] = i;
        this->count++;
        this->amount++;
    }
}

vector<TValue> SortedMultiMap::search(TKey c) const {
    vector<TValue> keyValues;
    bool found = false;
    int index = this->tree->root;
    while (index != -1 && !found) {
        if (this->tree->key[index] == c) {
            for (int j = 0; j < this->tree->keyValues[index].count; j++)
                keyValues.push_back(this->tree->keyValues[index].values[j]);
            found = true;
        }
        else if (this->tree->key[index] >= c)
            index = this->tree->left[index];
        else
            index = this->tree->right[index];
    }

    return keyValues;
}

bool SortedMultiMap::remove(TKey c, TValue v) {
    bool found = false;
    int index = this->tree->root;
    while (index != -1 && !found) {
        if (this->tree->key[index] == c) {
            found = true;
        }
        else if (this->tree->key[index] >= c)
            index = this->tree->left[index];
        else
            index = this->tree->right[index];
    }
    if (!found)
        return false;
    else {
        found = false;
        int position;
        for (int i = 0; i < this->tree->keyValues[index].count && !found; i++)
            if (this->tree->keyValues[index].values[i] == v) {
                found = true;
                position = i;
            }
        if (!found)
            return false;
        else {
            for (int i = position; i < this->tree->keyValues[index].count - 1; i++)
                this->tree->keyValues[index].values[i] = this->tree->keyValues[index].values[i + 1];
            this->tree->keyValues[index].count--;
            this->amount--;
            if (this->tree->keyValues[index].count == 0)
            {
                if (this->count == 1) {
                    this->count--;
                    return true;
                }
                else if (this->tree->left[index] == -1 && this->tree->right[index] == -1) {

                    if (this->tree->parent[index] != -1) {
                        if (this->tree->key[this->tree->left[this->tree->parent[index]]] == this->tree->key[index])
                            this->tree->left[this->tree->parent[index]] = -1;
                        if (this->tree->key[this->tree->right[this->tree->parent[index]]] == this->tree->key[index])
                            this->tree->right[this->tree->parent[index]] = -1;
                    }
                    for (int i = index; i < this->count - 1; i++) {
                        this->tree->key[i] = this->tree->key[i + 1];
                        this->tree->left[i] = this->tree->left[i + 1];
                        this->tree->right[i] = this->tree->right[i + 1];
                        this->tree->parent[i] = this->tree->parent[i + 1];
                        this->tree->keyValues[i] = this->tree->keyValues[i + 1];
                        if (this->tree->parent[i] == -1)
                            this->tree->root = i;
                        else {
                            if (this->tree->key[this->tree->right[this->tree->parent[i]]] == this->tree->key[i])
                                this->tree->right[this->tree->parent[i]]--;
                            else if (this->tree->key[this->tree->left[this->tree->parent[i]]] == this->tree->key[i])
                                this->tree->left[this->tree->parent[i]]--;
                        }
                        if (this->tree->left[i] != -1)
                            this->tree->parent[this->tree->left[i]]--;
                        if (this->tree->right[i] != -1)
                            this->tree->parent[this->tree->right[i]]--;
                    }
                    this->count--;

                }
                else if (this->tree->right[index] == -1 && this->tree->left[index] != -1) {
                    if (this->tree->parent[index] != -1) {
                        if (this->tree->key[this->tree->right[this->tree->parent[index]]] == this->tree->key[index])
                            this->tree->right[this->tree->parent[index]] = this->tree->left[index];
                        else if (this->tree->key[this->tree->left[this->tree->parent[index]]] ==
                            this->tree->key[index])
                            this->tree->left[this->tree->parent[index]] = this->tree->left[index];
                    }
                    for (int i = index; i < this->count - 1; i++) {
                        this->tree->key[i] = this->tree->key[i + 1];
                        this->tree->left[i] = this->tree->left[i + 1];
                        this->tree->right[i] = this->tree->right[i + 1];
                        this->tree->parent[i] = this->tree->parent[i + 1];
                        this->tree->keyValues[i] = this->tree->keyValues[i + 1];
                        if (this->tree->parent[i] == -1)
                            this->tree->root = i;
                        else {
                            if (this->tree->key[this->tree->right[this->tree->parent[i]]] == this->tree->key[i])
                                this->tree->right[this->tree->parent[i]]--;
                            else if (this->tree->key[this->tree->left[this->tree->parent[i]]] == this->tree->key[i])
                                this->tree->left[this->tree->parent[i]]--;
                        }
                        if (this->tree->left[i] != -1)
                            this->tree->parent[this->tree->left[i]]--;
                        if (this->tree->right[i] != -1)
                            this->tree->parent[this->tree->right[i]]--;
                    }
                    this->count--;

                }
                else if (this->tree->right[index] != -1 && this->tree->left[index] == -1) {
                    if (this->tree->parent[index] != -1) {
                        if (this->tree->key[this->tree->right[this->tree->parent[index]]] == this->tree->key[index])
                            this->tree->right[this->tree->parent[index]] = this->tree->right[index];

                        else if (this->tree->key[this->tree->left[this->tree->parent[index]]] ==
                            this->tree->key[index])
                            this->tree->left[this->tree->parent[index]] = this->tree->right[index];
                    }
                    for (int i = index; i < this->count - 1; i++) {
                        this->tree->key[i] = this->tree->key[i + 1];
                        this->tree->left[i] = this->tree->left[i + 1];
                        this->tree->right[i] = this->tree->right[i + 1];
                        this->tree->parent[i] = this->tree->parent[i + 1];
                        this->tree->keyValues[i] = this->tree->keyValues[i + 1];
                        if (this->tree->parent[i] == -1)
                            this->tree->root = i;
                        else {
                            if (this->tree->key[this->tree->right[this->tree->parent[i]]] == this->tree->key[i])
                                this->tree->right[this->tree->parent[i]]--;
                            else if (this->tree->key[this->tree->left[this->tree->parent[i]]] == this->tree->key[i])
                                this->tree->left[this->tree->parent[i]]--;
                        }
                        if (this->tree->left[i] != -1)
                            this->tree->parent[this->tree->left[i]]--;
                        if (this->tree->right[i] != -1)
                            this->tree->parent[this->tree->right[i]]--;
                    }
                    this->count--;

                }
                else
                {
                    if (index > 0) {
                        if (this->tree->key[this->tree->left[this->tree->parent[index - 1]]] ==
                            this->tree->key[index - 1])
                            this->tree->left[this->tree->parent[index - 1]] = -1;
                        if (this->tree->key[this->tree->right[this->tree->parent[index - 1]]] ==
                            this->tree->key[index - 1])
                            this->tree->right[this->tree->parent[index - 1]] = -1;
                        this->tree->left[index - 1] = this->tree->left[index];
                        this->tree->right[index - 1] = this->tree->right[index];
                        this->tree->parent[index - 1] = this->tree->parent[index];
                        this->tree->keyValues[index - 1] = this->tree->keyValues[index];
                        if (this->tree->root == index)
                            this->tree->root--;
                    }
                    else {
                        if (this->tree->key[this->tree->left[this->tree->parent[index + 1]]] ==
                            this->tree->key[index + 1])
                            this->tree->left[this->tree->parent[index + 1]] = +1;
                        if (this->tree->key[this->tree->right[this->tree->parent[index + 1]]] ==
                            this->tree->key[index + 1])
                            this->tree->right[this->tree->parent[index + 1]] = +1;
                        this->tree->left[index + 1] = this->tree->left[index];
                        this->tree->right[index + 1] = this->tree->right[index];
                        this->tree->parent[index + 1] = this->tree->parent[index];
                        if (this->tree->root == index)
                            this->tree->root++;


                    }
                    for (int i = index; i < this->count - 1; i++) {
                        this->tree->key[i] = this->tree->key[i + 1];
                        this->tree->left[i] = this->tree->left[i + 1];
                        this->tree->right[i] = this->tree->right[i + 1];
                        this->tree->parent[i] = this->tree->parent[i + 1];
                        this->tree->keyValues[i] = this->tree->keyValues[i + 1];
                        if (this->tree->parent[i] == -1)
                            this->tree->root = i;
                        else {
                            if (this->tree->key[this->tree->right[this->tree->parent[i]]] == this->tree->key[i])
                                this->tree->right[this->tree->parent[i]]--;
                            else if (this->tree->key[this->tree->left[this->tree->parent[i]]] == this->tree->key[i])
                                this->tree->left[this->tree->parent[i]]--;
                        }
                        if (this->tree->left[i] != -1)
                            this->tree->parent[this->tree->left[i]]--;
                        if (this->tree->right[i] != -1)
                            this->tree->parent[this->tree->right[i]]--;
                    }
                    this->count--;

                }

            }
            this->tree->parent[this->tree->root] = -1;
            return true;

        }

    }
}


int SortedMultiMap::size() const {
    return this->amount;
}

bool SortedMultiMap::isEmpty() const {
    if (this->amount)
        return false;
    return true;
}

SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}

SortedMultiMap::~SortedMultiMap() {
    delete this->tree;
}



void SortedMultiMap::resize() {
    BSTTree* newTree = new BSTTree;
    this->capacity *= 2;
    newTree->key = new TKey[this->capacity];
    for (int i = 0; i < this->capacity / 2; i++)
        newTree->key[i] = this->tree->key[i];
    newTree->root = this->tree->root;
    newTree->left = new TKey[this->capacity];
    for (int i = 0; i < this->capacity; i++) {
        if (i < this->capacity / 2)
            newTree->left[i] = this->tree->left[i];
        else
            newTree->left[i] = -1;
    }
    newTree->right = new TKey[this->capacity];
    for (int i = 0; i < this->capacity; i++) {
        if (i < this->capacity / 2)
            newTree->right[i] = this->tree->right[i];
        else
            newTree->right[i] = -1;
    }
    newTree->parent = new TKey[this->capacity];
    for (int i = 0; i < this->capacity / 2; i++) {
        if (i < this->capacity / 2)
            newTree->parent[i] = this->tree->parent[i];
        else
            newTree->parent[i] = -1;
    }
    newTree->keyValues = new Values[this->capacity];
    for (int i = 0; i < this->capacity; i++) {
        if (i < this->capacity / 2)
        {
            newTree->keyValues[i].capacity = this->tree->keyValues[i].capacity;
            newTree->keyValues[i].values = new TValue[newTree->keyValues[i].capacity];
            newTree->keyValues[i].count = this->tree->keyValues[i].count;
            for (int j = 0; j < newTree->keyValues[i].count; j++)
                newTree->keyValues[i].values[j] = this->tree->keyValues[i].values[j];

        }
        else {
            newTree->keyValues[i].capacity = 1;
            newTree->keyValues[i].values = new TValue[newTree->keyValues[i].capacity];
            newTree->keyValues[i].count = 0;
        }
    }
    delete this->tree;
    this->tree = newTree;

}