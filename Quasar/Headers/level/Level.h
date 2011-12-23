#pragma once

#include "level/Chunk.h"

class Level {
private:
    struct Node {
        int lvl;
        int count;
        // Represents array of Chunk*'s at leaf nodes.
        Node* nodes[8];

        Node(int lvl) : lvl(lvl), count(0) {
            ZeroStruct(nodes);
        }

        Node** Get(const Vector3I& cp) {
            return nodes + ((cp.x >> lvl) & 1) + (((cp.y >> lvl) & 1) << 1) + (((cp.z >> lvl) & 1) << 2);
        }

        void Set(const Vector3I& cp, void* ptr) {
            nodes[((cp.x >> lvl) & 1) + (((cp.y >> lvl) & 1) << 1) + (((cp.z >> lvl) & 1) << 2)] = reinterpret_cast<Node*>(ptr);
        }

        void Remove() {
            if (lvl == 0) {
                for (int i = 0; i < 8; i++) {
                    delete reinterpret_cast<Chunk*>(nodes[i]);
                }
            } else {
                for (int i = 0; i < 8; i++) {
                    if (nodes[i]) {
                        nodes[i]->Remove();
                    }
                }
            }
            delete this;
        }
    };

    int Dim(int lvl) {
        return 2 << lvl;
    }

    Node* root;
    // Keeps the octree in the positive octant.
    Vector3I offset;

public:
    Level() : root(nullptr) {
    }

    void Shutdown() {
        root->Remove();
    }

    void InsertChunk(Chunk* c) {
        if (!root) {
            root = new Node(0);
            // Turn off the first bit.
            offset = Vector3I(c->pos.x & ~1, c->pos.y & ~1, c->pos.z & ~1);
        }
        Vector3I cp = c->pos + offset;
        Expand(cp);
        Insert(root, c, cp);
    }

    Chunk* GetChunk(const Vector3I& p)const {
        return Get(root, (p >> Chunk::SHIFT) + offset);
    }

private:
    void Insert(Node* n, Chunk* c, const Vector3I& cp) {
        if (n->lvl == 0) {
            n->Set(cp, c);
            n->count++;
        } else {
            Node** nextNode = n->Get(cp);
            if (!*nextNode) {
                *nextNode = new Node(n->lvl - 1);
                n->count++;
            }
            Insert(*nextNode, c, cp);
        }
    }

    Chunk* Get(Node* n, const Vector3I& cp)const {
        if (!n) {
            return nullptr;
        }
        if (n->lvl == 0) {
            return reinterpret_cast<Chunk*>(*n->Get(cp));
        }
        return Get(*n->Get(cp), cp);
    }

    void Expand(Vector3I& p) {
        if (GetNodePositionContainingPoint(root->lvl, p) != offset) {
            Node* newRoot = new Node(root->lvl + 1);
            Vector3I newOffset = Vector3I(p.x < 0, p.y < 0, p.z < 0);
            newRoot->nodes[newOffset.x + (newOffset.y << 1) + (newOffset.z << 2)] = root;
            newRoot->count = 1;
            newOffset *= Dim(newRoot->lvl);
            offset -= newOffset;
            p -= newOffset;
            root = newRoot;
            Expand(p);
        }
    }

    Vector3I GetNodePositionContainingPoint(int lvl, const Vector3I& p) {
        return Vector3I(p.x & ~(Dim(lvl) - 1), p.y & ~(Dim(lvl) - 1), p.z & ~(Dim(lvl) - 1));
    }
};