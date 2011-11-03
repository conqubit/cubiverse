#pragma once

#include "stdafx.h"

#define MAX_LEVELS 31

// -------------------------------------------
// Implements an ordered set with a skip list.
// -------------------------------------------

template <class T>
class SkipList {
private:
	// ******** START INTERNAL TYPES ********
	struct Node {
		T Value;
		Node* Next[0];
	};

	class Iterator {
	public:
		Iterator(Node* node) : node(node) {
		}
		void Next() {
			if (node == nullptr) {
				return;
			}
			node = node->Next[0];
		}
		T Get() {
			if (node == nullptr) {
				return T();
			}
			return node->Value;
		}
		bool End() {
			return node == nullptr;
		}

	private:
		Node* node;
	};
	// ******* END INTERNAL TYPES ********

public:
	SkipList() : count(), topLevel() {
		head = new_Node(MAX_LEVELS);
	}

	~SkipList() {
		while (head) {
			Node* node = head;
			head = head->Next[0];
			free(node);
		}
	}

	int Count() {
		return count;
	}

	Iterator Start() {
		return Iterator(head->Next[0]);
	}

	// Returns false if value already existed, true otherwise.
	bool Insert(const T& value);

	// Returns false if value was not found, true otherwise.
	bool Remove(const T& value);

	bool Contains(const T& value);

	T* ToArray();

public:
	Node* new_Node(int levels) {
		int size = sizeof(Node) + sizeof(Node*) * levels;
		return (Node*)calloc(size, 1);
	}

	void trimLevels() {
		while (head->Next[topLevel] == nullptr && topLevel >= 1) {
			topLevel--;
		}
	}

	int getRandomLevel() {
		int level = 0;
		unsigned int r;
		for (r ^= rand30(); r & 1; r >>= 1) {
			level++;
		}
		return level;
	}

	Node* head;
	int count;
	int topLevel;
};



template <class T>
bool SkipList<T>::Insert(const T& value) {
	int level = getRandomLevel();
	if (level > topLevel) {
		topLevel = level;
	}
	// The new node to insert.
	Node* node = new_Node(level+1);
	node->Value = value;
	// Rightmost node that is less than or equal to the new node at the current search level.
	Node* left = head;
	bool existed = false;
	for (int i = topLevel; i >= 0; i--) {
		while (left->Next[i] && left->Next[i]->Value < value) {
			left = left->Next[i];
		}
		Node* next = left->Next[i];
		if (next && next->Value == value) {
			left->Next[i] = next->Next[i];
			if (i == 0) {
				free(next);
				existed = true;
			}
		}
		if (i <= level) {
			node->Next[i] = left->Next[i];
			left->Next[i] = node;
		}
	}
	if (existed) {
		trimLevels();
		return false;
	}
	count++;
	return true;
}

template <class T>
bool SkipList<T>::Remove(const T& value) {
	Node* left = head;
	for (int i = topLevel; i >= 0; i--) {
		while (left->Next[i] && left->Next[i]->Value < value) {
			left = left->Next[i];
		}
		Node* next = left->Next[i];
		if (next && next->Value == value) {
			left->Next[i] = next->Next[i];
			if (i == 0) {
				free(next);
				count--;
				trimLevels();
				return true;
			}
		}
	}
	return false;
}

template <class T>
bool SkipList<T>::Contains(const T& value) {
	Node* left = head;
	for (int i = topLevel; i >= 0; i--) {
		while (left->Next[i] && left->Next[i]->Value <= value) {
			left = left->Next[i];
		}
		if (left->Value == value) {
			return true;
		}
	}
	return false;
}

template <class T>
T* SkipList<T>::ToArray() {
	T* ret = new T[count];
	Node* n = head;
	for (int i = 0; i < count; i++) {
		n = n->Next[0];
		ret[i] = n->Value;
	}
	return ret;
}