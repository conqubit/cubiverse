#pragma once

#include "stdafx.h"

template <typename T>
class LinkedStack {
private:
    struct Node {
        Node(const T& value) :
            Value(value),
            Next() {
        }
        ~Node() {
        }
        T Value;
        Node* Next;
    };

public:
    LinkedStack() : head(), count() {
    }

    ~LinkedStack() {
        while (head) {
            Node* next = head->Next;
            delete head;
            head = next;
        }
    }

    int Count() {
        return count;
    }

    bool Empty() {
        return count == 0;
    }

    void Push(const T& value) {
        Node* node = new Node(value);
        node->Next = head;
        head = node;
        count++;
    }

    T Pop() {
        if (head == nullptr) {
            return T();
        }
        Node* popped = head;
        T value = popped->Value;
        head = popped->Next;
        delete popped;
        count--;
        return value;
    }

    T Peek() {
        if (head == nullptr) {
            return T();
        }
        return head->Value;
    }

private:
    Node* head;
    int count;
};