#pragma once

#include "stdafx.h"

template <class T>
class Queue {
private:
	struct Node {
		Node(const T& value) : Value(value), Next() {
		}
		~Node() {
		}

		T Value;
		Node* Next;
	};

public:
	Queue() : head(), tail(), count() {
	}

	~Queue() {
		while (head) {
			Node* next = head->Next;
			delete head;
			head = next;
		}
	}

	int Count() {
		return count;
	}

	void Enqueue(const T& value) {
		Node* node = new Node(value);
		if (tail == nullptr) {
			if (head == nullptr) {
				head = node;
			} else {
				tail = node;
				head->next = tail;
			}
		} else {
			tail->Next = node;
			tail = node;
		}
		count++;
	}

	T Dequeue() {
		if (head == nullptr) {
			return T();
		}
		Node* dequeued = head;
		T value = dequeued->Value;
		head = dequeued->Next;
		delete dequeued;
		if (head == tail) {
			tail = nullptr;
		}
		count--;
		return value;
	}

private:
	Node* head, tail;
	int count;
};