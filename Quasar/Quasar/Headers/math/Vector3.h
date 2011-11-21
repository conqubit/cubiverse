#pragma once

#include "stdafx.h"

template <typename T>
class Vector3;

typedef Vector3<int>    Vector3I;
typedef Vector3<float>  Vector3F;
typedef Vector3<double> Vector3D;

template <typename T>
class Vector3 {
public:
    static const Vector3<T> ZERO;
    static const Vector3<T> AXIS_X;
    static const Vector3<T> AXIS_Y;
    static const Vector3<T> AXIS_Z;
    static const Vector3<T> AXIS[3];

    T x, y, z;

    //---------------- Constructors / Destructors ----------------
    Vector3() : x(), y(), z() {
    }

    template <typename U>
    explicit Vector3(const Vector3<U>& v) : x((T)v.x), y((T)v.y), z((T)v.z) {
    }

    template <typename U, typename V, typename W>
    explicit Vector3(U x, V y, W z) : x((T)x), y((T)y), z((T)z) {
    }

    explicit Vector3(const XMFLOAT3& v) : x((T)v.x), y((T)v.y), z((T)v.z) {
    }

    explicit Vector3(const XMVECTOR& v) : x((T)v.m128_f32[0]), y((T)v.m128_f32[1]), z((T)v.m128_f32[2]) {
    }

    ~Vector3() {
    }

    Vector3I ToInt()const {
        return Vector3I(*this);
    }

    Vector3F ToFloat()const {
        return Vector3F(*this);
    }

    Vector3D ToDouble()const {
        return Vector3D(*this);
    }

    XMFLOAT3 ToXMFloat3()const {
        return XMFLOAT3((float)x, (float)y, (float)z);
    }

    XMVECTOR ToXMVector()const {
        return XMVectorSet((float)x, (float)y, (float)z, 0.0f);
    }

    //---------------- Initializers ----------------
    template <typename U, typename V, typename W>
    Vector3<T>& Set(U x, V y, W z) {
        return this->x = (T)x, this->y = (T)y, this->z = (T)z, *this;
    }

    template <typename U>
    Vector3<T>& Set(const Vector3<U>& v) {
        return x = (T)v.x, y = (T)v.y, z = (T)v.z, *this;
    }

    //---------------- Comparators ----------------
    bool IsZero()const {
        return x == 0 && y == 0 && z == 0;
    }
    
    //---------------- Lengths ----------------
    double Length()const {
        return sqrt(LengthSquared());
    }

    double LengthSquared()const {
        return (double)x * x + (double)y * y + (double)z * z;
    }
    
    //---------------- Normalizers ----------------
    Vector3D Normalize()const {
        if (IsZero()) return Vector3D();
        return ToDouble() / Length();
    }

    template <typename U>
    Vector3D Normalize(U newLength)const {
        if (IsZero()) return Vector3D();
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
        return v.ToDouble() * (Dot(v) / v.LengthSquared());
    }

    //---------------- Accessors ----------------
    T& operator[](int i) {
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
        return x = (T)v.x, y = (T)v.y, z = (T)v.z, *this;
    }

    template <typename U>
    Vector3<T>& operator+=(const Vector3<U>& v) {
        return x += (T)v.x, y += (T)v.y, z += (T)v.z, *this;
    }

    template <typename U>
    Vector3<T>& operator-=(const Vector3<U>& v) {
        return x -= (T)v.x, y -= (T)v.y, z -= (T)v.z, *this;
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

    string ToString()const {
        return '{' + str(x) + ", " + str(y) + ", " + str(z) + '}';
    }

    //---------------- Utilities ----------------
    Vector3<T> OnlyX() {
        return Vector3<T>(x, 0, 0);
    }

    Vector3<T> OnlyY() {
        return Vector3<T>(0, y, 0);
    }

    Vector3<T> OnlyZ() {
        return Vector3<T>(0, 0, z);
    }

    Vector3I Floor() {
        return Vector3I(floor(x), floor(y), floor(z));
    }

    Vector3I Ceil() {
        return Vector3I(ceil(x), ceil(y), ceil(z));
    }

    Vector3<T> Abs() {
        return Vector3<T>(abs(x), abs(y), abs(z));
    }
};

template <typename T>
const Vector3<T> Vector3<T>::ZERO = Vector3<T>();
template <typename T>
const Vector3<T> Vector3<T>::AXIS_X = Vector3<T>(1, 0, 0);
template <typename T>
const Vector3<T> Vector3<T>::AXIS_Y = Vector3<T>(0, 1, 0);
template <typename T>
const Vector3<T> Vector3<T>::AXIS_Z = Vector3<T>(0, 0, 1);
template <typename T>
const Vector3<T> Vector3<T>::AXIS[3] = { Vector3<T>::AXIS_X,
                                         Vector3<T>::AXIS_Y,
                                         Vector3<T>::AXIS_Z };

template <>
inline XMFLOAT3 Vector3F::ToXMFloat3()const {
    return *((XMFLOAT3*)(&x));
}