#pragma once

#include <math.h>

template <typename T>
class Vector3 {
    public:
        T x, y, z;

        #pragma region Methods

        //---------------- Constructors / Destructors ----------------
        Vector3<T>() :
            x(), y(), z() {
        }
        Vector3<T>(T x, T y, T z) :
            x(x), y(y), z(z) {
        }
        Vector3<T>(const Vector3<T>& v) :
            x(v.x), y(v.y), z(v.z) {
        }
        ~Vector3<T>() {
        }

        //---------------- Initializers ----------------
        Vector3<T>& Set(T x, T y, T z) {
            return this->x = x, this->y = y, this->z = z, *this;
        }

        Vector3<T>& Set(const Vector3<int>& v) {
            return x = v.x, y = v.y, z = v.z, *this;
        }
        Vector3<T>& Set(const Vector3<float>& v) {
            return x = v.x, y = v.y, z = v.z, *this;
        }
        Vector3<T>& Set(const Vector3<double>& v) {
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
        Vector3<T> Normalize()const {
            return IsZero() ? *this : *this / Length();
        }

        Vector3<T> Normalize(double newLength)const {
            return IsZero() ? *this : *this * newLength / Length();
        }
        
        //---------------- Products ----------------
        T Dot(const Vector3<T>& v)const { 
            return (x * v.x) + (y * v.y) + (z * v.z);
        }

        Vector3<T> Cross(const Vector3<T>& v)const {
            return Vector3<T>((y * v.z - z * v.y),
                              (z * v.x - x * v.z),
                              (x * v.y - y * v.x));
        }
        
        //---------------- Projections ----------------
        double ScalarProjectionOnto(const Vector3<T>& v)const {
            return this->Dot(v) / v.Length();
        }

        Vector3<T> VectorProjectionOnto(const Vector3<T>& v)const {
            return v * (this->Dot(v) / v.LengthSquared());
        }

        #pragma endregion

        #pragma region Operators
        
        //---------------- Casting ----------------
        operator Vector3<int>()const {
            return Vector3<int>(*this);
        }
        operator Vector3<float>()const {
            return Vector3<float>(*this);
        }
        operator Vector3<double>()const {
            return Vector3<double>(*this);
        }

        //---------------- Accessors ----------------
        T operator [](int i)const {
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
        Vector3<T> operator+(const Vector3<int>& v)const {
            return Vector3<T>(x + v.x, y + v.y, z + v.z);
        }
        Vector3<T> operator+(const Vector3<float>& v)const {
            return Vector3<T>(x + v.x, y + v.y, z + v.z);
        }
        Vector3<T> operator+(const Vector3<double>& v)const {
            return Vector3<T>(x + v.x, y + v.y, z + v.z);
        }

        Vector3<T> operator-(const Vector3<int>& v)const {
            return Vector3<T>(x - v.x, y - v.y, z - v.z);
        }
        Vector3<T> operator-(const Vector3<float>& v)const {
            return Vector3<T>(x - v.x, y - v.y, z - v.z);
        }
        Vector3<T> operator-(const Vector3<double>& v)const {
            return Vector3<T>(x - v.x, y - v.y, z - v.z);
        }
        
        //---------------- Vector-Scalar Arithmetic ----------------
        Vector3<T> operator*(int s)const {
            return Vector3<T>(x * s, y * s, z * s);
        }
        Vector3<T> operator*(float s)const {
            return Vector3<T>(x * s, y * s, z * s);
        }
        Vector3<T> operator*(double s)const {
            return Vector3<T>(x * s, y * s, z * s);
        }

        Vector3<T> operator/(int s)const {
            return Vector3<T>(x / s, y / s, z / s);
        }
        Vector3<T> operator/(float s)const {
            return Vector3<T>(x / s, y / s, z / s);
        }
        Vector3<T> operator/(double s)const {
            return Vector3<T>(x / s, y / s, z / s);
        }
        
        //---------------- Vector-Vector Assignment ----------------
        Vector3<T>& const operator=(const Vector3<int>& v) {
            return x = v.x, y = v.y, z = v.z, *this;
        }
        Vector3<T>& const operator=(const Vector3<float>& v) {
            return x = v.x, y = v.y, z = v.z, *this;
        }
        Vector3<T>& const operator=(const Vector3<double>& v) {
            return x = v.x, y = v.y, z = v.z, *this;
        }

        Vector3<T>& const operator+=(const Vector3<int>& v) {
            return x += v.x, y += v.y, z += v.z, *this;
        }
        Vector3<T>& const operator+=(const Vector3<float>& v) {
            return x += v.x, y += v.y, z += v.z, *this;
        }
        Vector3<T>& const operator+=(const Vector3<double>& v) {
            return x += v.x, y += v.y, z += v.z, *this;
        }

        Vector3<T>& const operator-=(const Vector3<int>& v) {
            return x -= v.x, y -= v.y, z -= v.z, *this;
        }
        Vector3<T>& const operator-=(const Vector3<float>& v) {
            return x -= v.x, y -= v.y, z -= v.z, *this;
        }
        Vector3<T>& const operator-=(const Vector3<double>& v) {
            return x -= v.x, y -= v.y, z -= v.z, *this;
        }
        
        //---------------- Vector-Scalar Assignment ----------------
        Vector3<T>& const operator*=(int s) {
            return x *= s, y *= s, z *= s, *this;
        }
        Vector3<T>& const operator*=(float s) {
            return x *= s, y *= s, z *= s, *this;
        }
        Vector3<T>& const operator*=(double s) {
            return x *= s, y *= s, z *= s, *this;
        }

        Vector3<T>& const operator/=(int s) {
            return x /= s, y /= s, z /= s, *this;
        }
        Vector3<T>& const operator/=(float s) {
            return x /= s, y /= s, z /= s, *this;
        }
        Vector3<T>& const operator/=(double s) {
            return x /= s, y /= s, z /= s, *this;
        }

        #pragma endregion

};

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;