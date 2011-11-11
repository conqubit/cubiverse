#pragma once

#include <math.h>
#include <xnamath.h>

template <typename T>
class Vector3;

typedef Vector3<int> Vector3I;
typedef Vector3<float> Vector3F;
typedef Vector3<double> Vector3D;

template <typename T>
class Vector3 {
public:
    T x, y, z;

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

    XMFLOAT3 ToXMFloat3() {
        return XMFLOAT3((float)x, (float)y, (float)z);
    }

    XMVECTOR ToXMVector() {
        return XMVectorSet((float)x, (float)y, (float)z, 0.0f);
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
        if (IsZero()) return this->ToDouble();
        return this->ToDouble() / Length();
    }

    template <typename U>
    Vector3D Normalize(U newLength)const {
        if (IsZero()) return ToDouble();
        return ToDouble() * (newLength / Length());
    }
    
    //---------------- Products ----------------
    T Dot(const Vector3<T>& v)const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3<T> Cross(const Vector3<T>& v)const {
        return Vector3<T>((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x));
    }
    
    //---------------- Projections ----------------
    double ScalarProjectionOnto(const Vector3<T>& v)const {
        return Dot(v).ToDouble() / v.Length();
    }

    Vector3D VectorProjectionOnto(const Vector3<T>& v)const {
        return v.ToDouble() * (Dot(v) / v.LengthSquaredPricise());
    }

    //---------------- Accessors ----------------
    T operator[](int i)const {
        return (&x)[i];
    }
    
    //---------------- Unitary ----------------
    Vector3<T> operator-()const {
        return Vector3<T>(-x, -y, -z);
    }
    
    //---------------- Vector-Vector Arithmetic ----------------
    Vector3<T> operator+(const Vector3<T>& v)const {
        return Vector3<T>(x + v.x, y + v.y, z + v.z);
    }

    Vector3<T> operator-(const Vector3<T>& v)const {
        return Vector3<T>(x - v.x, y - v.y, z - v.z);
    }
    
    //---------------- Vector-Scalar Arithmetic ----------------
    Vector3<T> operator*(T s)const {
        return Vector3<T>(x * s, y * s, z * s);
    }

    Vector3<T> operator/(T s)const {
        return Vector3<T>(x / s, y / s, z / s);
    }
    
    //---------------- Vector-Vector Assignment ----------------
    Vector3<T>& operator=(const Vector3<T>& v) {
        return x = v.x, y = v.y, z = v.z, *this;
    }

    Vector3<T>& operator+=(const Vector3<T>& v) {
        return x += v.x, y += v.y, z += v.z, *this;
    }

    Vector3<T>& operator-=(const Vector3<T>& v) {
        return x -= v.x, y -= v.y, z -= v.z, *this;
    }
    
    //---------------- Vector-Scalar Assignment ----------------
    Vector3<T>& operator*=(T s) {
        return x *= s, y *= s, z *= s, *this;
    }

    Vector3<T>& operator/=(T s) {
        return x /= s, y /= s, z /= s, *this;
    }
    
};

template <>
inline XMFLOAT3 Vector3F::ToXMFloat3() {
    return *((XMFLOAT3*)(&x));
}