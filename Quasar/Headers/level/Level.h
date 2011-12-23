#pragma once

#include "level/Chunk.h"

class Level {
private:
    struct Node {
        int lvl;
        // Represents an array of Chunk*'s at leaf nodes.
        Node* nodes[8];

        Node(int lvl) : lvl(lvl) {
            ZeroStruct(nodes);
        }

        Node* Get(const Vector3I& cp) {
            return nodes[GetIndex(cp)];
        }

        void Set(const Vector3I& cp, void* ptr) {
            nodes[GetIndex(cp)] = reinterpret_cast<Node*>(ptr);
        }

        int GetIndex(const Vector3I& cp) {
            return ((cp.x >> lvl) & 1) + (((cp.y >> lvl) & 1) << 1) + (((cp.z >> lvl) & 1) << 2);
        }

        void Shutdown() {
            if (lvl == 0) {
                for (int i = 0; i < 8; i++) {
                    reinterpret_cast<Chunk*>(nodes[i])->Shutdown();
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

    Node* root;
    // Keeps the octree in the positive octant.
    Vector3I offset;

public:
    Level() : root(nullptr) {
    }

    void Shutdown() {
        root->Shutdown();
        root = nullptr;
    }

    void InsertChunk(Chunk* c) {
        if (!root) {
            root = new Node(0);
            // Turn off the first bit.
            offset = Vector3I(c->pos.x & ~1, c->pos.y & ~1, c->pos.z & ~1);
        }

        Vector3I cp = c->pos + offset;
        Expand(cp);

        Node* node = root;
        while (node->lvl != 0) {
            Node* nextNode = node->Get(cp);
            if (!nextNode) {
                nextNode = new Node(node->lvl - 1);
                node->Set(cp, nextNode);
            }
            node = nextNode;
        }
        node->Set(cp, c);
    }

    Chunk* GetChunk(const Vector3I& cp)const {
        Node* node = root;
        while (node && node->lvl != 0) {
            node = node->Get(cp);
        }
        return node ? reinterpret_cast<Chunk*>(node->Get(cp)) : nullptr;
    }

private:
    // I'm not sure how this works, but my brain says it should.
    void Expand(Vector3I& p) {
        if (GetNodePositionContainingPoint(root->lvl, p) != offset) {
            Node* newRoot = new Node(root->lvl + 1);
            Vector3I newOffset = Vector3I(p.x < 0, p.y < 0, p.z < 0);
            newRoot->nodes[newOffset.x + (newOffset.y << 1) + (newOffset.z << 2)] = root;
            newOffset *= Dim(newRoot->lvl);
            offset -= newOffset;
            p -= newOffset;
            root = newRoot;
            Expand(p);
        }
    }

    static Vector3I GetNodePositionContainingPoint(int lvl, const Vector3I& p) {
        return Vector3I(p.x & ~(Dim(lvl) - 1), p.y & ~(Dim(lvl) - 1), p.z & ~(Dim(lvl) - 1));
    }

    static int Dim(int lvl) {
        return 2 << lvl;
    }
};