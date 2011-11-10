#pragma once

#include <math.h>

template <typename T>
class Vector3;

typedef Vector3<int> Vector3I;
typedef Vector3<float> Vector3F;
typedef Vector3<double> Vector3D;

template <typename T>
class Vector3 {
public:
    T x, y, z;

    #pragma region Methods

    //---------------- Constructors / Destructors ----------------
    Vector3() :
    x(), y(), z() {
    }

    template <typename U>
    Vector3(U x, U y, U z) :
    x(x), y(y), z(z) {
    }

    template <typename U>
    explicit Vector3(const Vector3<U>& v) :
    x(v.x), y(v.y), z(v.z) {
    }

    ~Vector3() {
    }

    Vector3I ToInt() {
        return Vector3I(*this);
    }

    Vector3F ToFloat() {
        return Vector3F(*this);
    }

    Vector3D ToDouble() {
        return Vector3F(*this);
    }

    //---------------- Initializers ----------------
    Vector3<T>& Set(T x, T y, T z) {
        return this->x = x, this->y = y, this->z = z, *this;
    }

    template <typename U>
    Vector3<T>& Set(const Vector3<U>& v) {
        return x = v.x, y = v.y, z = v.z, *this;
    }

    //---------------- Comparators ----------------
    bool IsZero()const {
        return x == 0 && y == 0 && z == 0;
    }
    
    //---------------- Lengths ----------------
    double Length()const {
        return sqrt(LengthSquaredPrecise());
    }

    T LengthSquared()const {
        return x * x + y * y + z * z;
    }

    double LengthSquaredPrecise()const {
        return (double)x * x + (double)y * y + (double)z * z;
    }
    
    //---------------- Normalizers ----------------
    Vector3D Normalize()const {
        if (IsZero()) {
            return this->ToDouble();
        }
        return this->ToDouble() / Length();
    }

    template <typename U>
    Vector3D Normalize(U newLength)const {
        if (IsZero()) {
            return this->ToDouble();
        }
        return this->ToDouble() * (newLength / Length());
    }
    
    //---------------- Products ----------------

    #pragma region Dot

    template <typename U>
    U Vector3I::Dot(const Vector3<U>& v)const {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    template <typename U>
    float Vector3F::Dot(const Vector3<U>& v)const {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    double Vector3F::Dot(const Vector3<double>& v)const {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    template <typename U>
    double Vector3D::Dot(const Vector3<U>& v)const {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    #pragma endregion

    #pragma region Cross

    template <typename U>
    Vector3<U> Vector3I::Cross(const Vector3<U>& v)const {
        return Vector3<U>((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x));
    }

    template <typename U>
    Vector3F Vector3F::Cross(const Vector3<U>& v)const {
        return Vector3F((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x));
    }

    Vector3D Vector3F::Cross(const Vector3D& v)const {
        return Vector3D((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x));
    }

    template <typename U>
    Vector3D Vector3D::Cross(const Vector3<U>& v)const {
        return Vector3D((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x));
    }

    #pragma endregion
    
    //---------------- Projections ----------------
    double ScalarProjectionOver(const Vector3<T>& v)const {
        return Dot(v).ToDouble() / v.Length();
    }

    Vector3D VectorProjectionOver(const Vector3<T>& v)const {
        return v * (Dot(v).ToDouble() / v.LengthSquaredPricise());
    }

    #pragma endregion

    #pragma region Operators

    //---------------- Accessors ----------------
    T operator[](int i)const {
        switch (i) {
            case 0:  return x;
            case 1:  return y;
            default: return z;
        }
    }
    
    //---------------- Unitary ----------------
    Vector3<T> operator-()const {
        return Vector3<T>(-x, -y, -z);
    }
    
    //---------------- Vector-Vector Arithmetic ----------------
    template <typename U>
    Vector3<T> operator+(const Vector3<U>& v)const {
        return Vector3<T>(x + v.x, y + v.y, z + v.z);
    }

    template <typename U>
    Vector3<T> operator-(const Vector3<U>& v)const {
        return Vector3<T>(x - v.x, y - v.y, z - v.z);
    }
    
    //---------------- Vector-Scalar Arithmetic ----------------
    template <typename U>
    Vector3<T> operator*(U s)const {
        return Vector3<T>(x * s, y * s, z * s);
    }

    template <typename U>
    Vector3<T> operator/(U s)const {
        return Vector3<T>(x / s, y / s, z / s);
    }
    
    //---------------- Vector-Vector Assignment ----------------
    template <typename U>
    Vector3<T>& operator=(const Vector3<U>& v) {
        return x = v.x, y = v.y, z = v.z, *this;
    }

    template <typename U>
    Vector3<T>& operator+=(const Vector3<U>& v) {
        return x += v.x, y += v.y, z += v.z, *this;
    }

    template <typename U>
    Vector3<T>& operator-=(const Vector3<U>& v) {
        return x -= v.x, y -= v.y, z -= v.z, *this;
    }
    
    //---------------- Vector-Scalar Assignment ----------------
    template <typename U>
    Vector3<T>& operator*=(U s) {
        return x *= s, y *= s, z *= s, *this;
    }

    template <typename U>
    Vector3<T>& operator/=(U s) {
        return x /= s, y /= s, z /= s, *this;
    }

    #pragma endregion
    
};