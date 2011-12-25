#pragma once

#include "level/Chunk.h"

class Level {
private:
    struct Node {
        short lvl;
        short count;
        // Represents an array of Chunk pointers if leaf node.
        Node* nodes[8];

        Node(int lvl) : lvl(lvl), count(0) {
            ZeroStruct(nodes);
        }

        Node* Get(const Vector3I& cp) {
            return nodes[GetIndex(cp)];
        }

        void Set(const Vector3I& cp, void* ptr) {
            nodes[GetIndex(cp)] = reinterpret_cast<Node*>(ptr);
        }

        int GetIndex(const Vector3I& cp) {
            return ((cp.x >> lvl) & 1) | (((cp.y >> lvl) & 1) << 1) | (((cp.z >> lvl) & 1) << 2);
        }

        void Shutdown() {
            if (lvl == 0) {
                for (int i = 0; i < 8; i++) {
                    if (nodes[i]) {
                        reinterpret_cast<Chunk*>(nodes[i])->Shutdown();
                    }
                }
            } else {
                for (int i = 0; i < 8; i++) {
                    if (nodes[i]) {
                        nodes[i]->Shutdown();
                    }
                }
            }
            delete this;
        }
    };
public:
    Node* root;
    // Keeps the root node exactly flush in the positive octant.
    Vector3I offset;


    Level() : root(nullptr) {
    }

    int Dim() {
        return root ? Dim(root->lvl) : 0;
    }

    void Shutdown() {
        root->Shutdown();
        root = nullptr;
    }

    void InsertChunk(Chunk* c) {
        if (!root) {
            root = new Node(0);
            offset = -(c->pos & ~1);
        }

        Vector3I cp = c->pos + offset;
        Expand(cp);

        Node* node = root;
        while (node->lvl != 0) {
            Node* nextNode = node->Get(cp);
            if (!nextNode) {
                nextNode = new Node(node->lvl - 1);
                node->Set(cp, nextNode);
                node->count++;
            }
            node = nextNode;
        }
        node->Set(cp, c);
        node->count++;
    }

    void RemoveChunk(Vector3I cp) {
        cp += offset;
        Remove(root, cp);
        if (root->count == 0) {
            delete root;
            root = nullptr;
            return;
        }
        while (root->lvl > 0 && root->count == 1) {
            for (int i = 0; i < 8; i++) {
                if (root->nodes[i]) {
                    Node* newRoot = root->nodes[i];
                    offset += Vector3I(i & 1, (i >> 1) & 1, (i >> 2) & 1) << root->lvl;
                    delete root;
                    root = newRoot;
                }
            }
        }
    }

    Chunk* GetChunk(Vector3I cp)const {
        cp += offset;
        if (!root || !InsideRootNode(cp)) {
            return nullptr;
        }
        Node* node = root;
        while (node && node->lvl != 0) {
            node = node->Get(cp);
        }
        return node ? reinterpret_cast<Chunk*>(node->Get(cp)) : nullptr;
    }

private:
    void Expand(Vector3I& cp) {
        while (!InsideRootNode(cp)) {
            Vector3I cpo = cp.Offset(-(1 << root->lvl));
            Vector3I newOffset = Vector3I(cpo.x < 0, cpo.y < 0, cpo.z < 0);
            Node* newRoot = new Node(root->lvl + 1);
            newRoot->nodes[newOffset.x | (newOffset.y << 1) | (newOffset.z << 2)] = root;
            newRoot->count = 1;
            newOffset <<= newRoot->lvl;
            offset += newOffset;
            cp += newOffset;
            root = newRoot;
        }
    }

    void Remove(Node* node, const Vector3I& cp) {
        if (!node) return;
        if (node->lvl > 0) {
            Node* nextNode = node->Get(cp);
            Remove(nextNode, cp);
            if (nextNode && nextNode->count == 0) {
                delete nextNode;
                node->Set(cp, nullptr);
                node->count--;
            }
        } else if (node->Get(cp)) {
            reinterpret_cast<Chunk*>(node->Get(cp))->Shutdown();
            node->Set(cp, nullptr);
            node->count--;
        }
    }

    bool InsideRootNode(const Vector3I& cp)const {
        return GetNodePositionContainingPoint(root->lvl, cp).IsZero();
    }

    static Vector3I GetNodePositionContainingPoint(int lvl, const Vector3I& cp) {
        return cp & ~(Dim(lvl) - 1);
    }

    static int Dim(int lvl) {
        return 2 << lvl;
    }
};