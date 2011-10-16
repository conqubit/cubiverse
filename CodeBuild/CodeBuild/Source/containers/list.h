#pragma once

#include "stdafx.h"
#define BEGINNING_CAPACITY 32

template <class T>
class List {
public:
	List() {
		capacity = 0;
		count = 0;

		data = malloc(sizeof(T) * BEGINNING_CAPACITY);
		if (data != nullptr) {
			capacity = BEGINNING_CAPACITY;
		}
	}

	List(const List<T>& list) {
		capacity = 0;
		count = 0;

		data = malloc(sizeof(T) * list.capacity);
		if (data != nullptr) {
			capacity = list.capacity;
			count = list.count;
			memcpy(data, list.data, sizeof(T) * count)
		}
	}

	~List() {
		free(data);
		data = nullptr;
		capacity = 0;
		count = 0;
	}

	void Add(const T& element) {
		if (count + 1 >= capacity) {
			T* newLocation = realloc(data, capacity * 2);

			if (newLocation == nullptr) {
				return;
			}
			data = newLocation;
			data[count++] = element;
			capacity *= 2;
		}
	}

	T operator[](int i) {
		return data[i];
	}
	T operatorH(int i) {
		return data[i];
	}

	size_t Count() {
		return count;
	}

private
	T* data;
	size_t capacity;
	size_t count;
};