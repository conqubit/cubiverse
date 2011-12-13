#pragma once

class Block {
public:
    enum {
        Air       = 0,
        Stone     = 1,
        Dirt      = 2,
        Grass     = 3,
        Test      = 4,
        Undefined = 255
    };

    static bool Solid(int t) {
        return solid[t];
    }

    static bool Opaque(int t) {
        return opaque[t];
    }

    static bool Visible(int t) {
        return t != Air && t != Undefined;
    }

    static void Init();

private:
    static bool solid[256];
    static bool opaque[256];
};