#pragma once

template <typename T>
struct Color {
    T r, g, b, a;

    template <typename U>
    Color(const Color<U>& c) :
    r((T)c.r), g((T)c.g), b((T)c.b), a((T)c.a) {
    }

    template <typename U, typename V, typename W, typename X>
    Color(U r, V g, W b, X a) :
    r((T)r), g((T)g), b((T)b), a((T)a) {
    }

    template <typename U, typename V, typename W>
    Color(U r, V g, W b) :
    r((T)r), g((T)g), b((T)b), a(1) {
    }
};

typedef Color<byte>          ColorI;
typedef Color<float>         ColorF;
typedef Color<double>        ColorD;