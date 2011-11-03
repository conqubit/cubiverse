#pragma once

#include "stdafx.h"
#define BEGINNING_CAPACITY 32

template <class T>
class ArrayList {
public:
	ArrayList() {
		capacity = 0;
		count = 0;

		data = malloc(sizeof(T) * BEGINNING_CAPACITY);
		if (data != nullptr) {
			capacity = BEGINNING_CAPACITY;
		}
	}

	ArrayList(const ArrayList<T>& ArrayList) {
		capacity = 0;
		count = 0;

		data = malloc(sizeof(T) * ArrayList.capacity);
		if (data != nullptr) {
			capacity = ArrayList.capacity;
			count = ArrayList.count;
			memcpy(data, ArrayList.data, sizeof(T) * count)
		}
	}

	~ArrayList() {
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

	size_t Count() {
		return count;
	}

private
	T* data;
	size_t capacity;
	size_t count;
};