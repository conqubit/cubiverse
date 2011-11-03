#pragma once

#include "stdafx.h"

#define BEGINNING_CAPACITY 32
template <class T>
class ArrayStack {
public:
	ArrayStack() : head(-1), count(), stackLength(BEGINNING_CAPACITY) {
		stack = (T*)malloc(sizeof(T*) * stackLength);
	}

	~ArrayStack() {
		free(stack);
	}

	int Count() {
		return count;
	}

	bool Empty() {
		return count == 0;
	}

	void Push(const T& value) {
		if (++count > stackLength) {
			resize(stackLength * 2);
		}
		stack[++head] = value;
	}

	T Pop() {
		if (count == 0) {
			return T();
		}
		T ret = stack[head--];
		if (--count >= BEGINNING_CAPACITY && count <= stackLength / 2) {
			resize(stackLength / 2);
		}
		return ret;
	}

	T Peek() {
		if (count == 0) {
			return T();
		}
		return stack[head];
	}

private:
	void resize(int length) {
		stack = (T*)realloc(stack, length);
		stackLength = length;
	}

	int head;
	int count;

	T* stack;
	int stackLength;
};