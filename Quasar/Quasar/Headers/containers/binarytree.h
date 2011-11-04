#pragma once

#include "stdafx.h"
#include "arraystack.h"

// ------------------------------------------
// Implements an ordered set with an AA tree.
// ------------------------------------------

template <typename T>
class BinaryTree {
private:
    // ******** START INTERNAL TYPES ********
    struct Node {
        Node(const T& value) : Value(value), Left(), Right(), Level(1) {
        }
        Node* Left;
        Node* Right;
        int Level;
        T Value;
    };

    class Iterator {
    public:
        Iterator(Node* node) : node(node), stack(), goRight() {
            Next();
        }
        void Next() {
            if (node == nullptr) {
                return;
            }
            if (goRight) {
                node = node->Right;
                goRight = false;
            }
            while (true) {
                if (node) {
                    stack.Push(node);
                    node = node->Left;
                } else {
                    node = stack.Pop();
                    goRight = true;
                    return;
                }
            }
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
        ArrayStack<Node*> stack;
        bool goRight;
    };
    // ******* END INTERNAL TYPES ********

public:
    BinaryTree() : root(), count() {
    }

    ~BinaryTree() {
    }

    int Count() {
        return count;
    }

    Iterator Start() {
        return Iterator(root);
    }

    // Returns false if value already existed, true otherwise.
    bool Insert(const T& value) {
        int before = count;
        root = insert(value, root);
        return count > before;
    }

    // Returns false if value was not found, true otherwise.
    bool Remove(const T& value) {
        int before = count;
        root = remove(value, root);
        return count < before;
    }

    bool Contains(const T& value);

    T* ToArray();

public:
    Node* root;
    int count;

    Node* insert(const T& value, Node* node);

    Node* remove(const T& value, Node* node);

    Node* skew(Node* node) {
        if (node && node->Left && node->Level == node->Left->Level) {
            Node* left = node->Left;
            node->Left = left->Right;
            left->Right = node;
            return left;
        }
        return node;
    }

    Node* split(Node* node) {
        if (node && node->Right && node->Right->Right && node->Level == node->Right->Right->Level) {
            Node* middle = node->Right;
            node->Right = middle->Left;
            middle->Left = node;
            middle->Level++;
            return middle;
        }
        return node;
    }

    T successor(Node* node) {
        node = node->Right;
        while (node->Left) {
            node = node->Left;
        }
        return node->Value;
    }

    T predecessor(Node* node) {
        node = node->Left;
        while (node->Right) {
            node = node->Right;
        }
        return node->Value;
    }

    void fixLevels(Node* node) {
        if (node->Left == nullptr || node->Right == nullptr) {
            return;
        }
        int level = 1 + min(node->Left->Level, node->Right->Level);

        if (node->Level > level) {
            node->Level = level;
            if (node->Right->Level > level) {
                node->Right->Level = level;
            }
        }
    }
};

template <typename T>
auto BinaryTree<T>::insert(const T& value, Node* node) -> Node* {
    if (node == nullptr) {
        count++;
        return new Node(value);
    }

    if (value < node->Value) {
        node->Left = insert(value, node->Left);
    }
    else if (value > node->Value) {
        node->Right = insert(value, node->Right);
    }

    node = skew(node);
    node = split(node);

    return node;
}

template <typename T>
auto BinaryTree<T>::remove(const T& value, Node* node) -> Node* {
    if (node == nullptr) {
        return nullptr;
    }

    if (value < node->Value) {
        node->Left = remove(value, node->Left);
    }
    else if (value > node->Value) {
        node->Right = remove(value, node->Right);
    }
    else {
        if (node->Left == nullptr ) {
            if (node->Right == nullptr) {
                count--;
                delete node;
                return nullptr;
            }
            T next = successor(node);
            node->Right = remove(next, node->Right);
            node->Value = next;
        }
        else {
            T prev = predecessor(node);
            node->Left = remove(prev, node->Left);
            node->Value = prev;
        }
    }

    fixLevels(node);
    node = skew(node);
    if (node->Right) {
        node->Right = skew(node->Right);
        node->Right->Right = skew(node->Right->Right);
    }
    node = split(node);
    node->Right = split(node->Right);
    return node;
}

template <typename T>
bool BinaryTree<T>::Contains(const T& value) {
    Node* node = root;
    while (node) {
        if (value < node->Value) {
            node = node->Left;
        }
        else if (value > node->Value) {
            node = node->Right;
        }
        else {
            return true;
        }
    }
    return false;
}

template <typename T>
T* BinaryTree<T>::ToArray() {
    T* ret = new T[count];
    int i = 0;
    for (auto j = Start(); !j.End(); j.Next(), i++) {
        ret[i] = j.Get();
    }
    return ret;
}