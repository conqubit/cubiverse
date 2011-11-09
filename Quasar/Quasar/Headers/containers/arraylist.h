#pragma once

#define BEGINNING_CAPACITY 32

template <typename T>
class ArrayList {

private:
    T* data;
    int capacity;
    int count;

public:
    ArrayList() :
        data(),
        capacity(BEGINNING_CAPACITY),
        count() {
		printf("%d\n", count);
        data = (T*)malloc(sizeof(T) * BEGINNING_CAPACITY);
    }

    ~ArrayList() {
        for (int i = 0; i < count; i++) data[i].~T();

        free(data);
        data = nullptr;
        capacity = 0;
        count = 0;
    }

    void Add(const T& element) {
        if (count + 1 >= capacity) {
            data = (T*)realloc(data, capacity *= 2);
        }
        data[count++] = element;
    }

    T operator[](int i) {
        return data[i];
    }

    int Count() {
        return count;
    }
};