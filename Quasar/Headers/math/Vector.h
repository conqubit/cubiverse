#pragma once

template <typename T>
struct Vector3;

typedef Vector3<int>    Vector3I;
typedef Vector3<float>  Vector3F;
typedef Vector3<double> Vector3D;

template <typename T>
struct Vector3 {
    static const Vector3<T> ZERO;
    static const Vector3<T> AXIS_X;
    static const Vector3<T> AXIS_Y;
    static const Vector3<T> AXIS_Z;
    static const Vector3<T> AXIS[3];

    T x, y, z;

    //---------------- Constructors / Destructors ----------------
    Vector3() : x(), y(), z() {
    }

    ~Vector3() {
    }

    template <typename U>
    explicit Vector3(const Vector3<U>& v) : x((T)v.x), y((T)v.y), z((T)v.z) {
    }

    template <typename U, typename V, typename W>
    Vector3(U x, V y, W z) : x((T)x), y((T)y), z((T)z) {
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

    glm::vec3 ToGlmVec3() {
        return glm::vec3(x, y, z);
    }

    glm::vec4 ToGlmVec4() {
        return glm::vec4(x, y, z, 1);
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

    bool operator==(Vector3<T> o)const {
        return x == o.x && y == o.y && z == o.z;
    }

    bool operator!=(Vector3<T> o)const {
        return x != o.x || y != o.y || z != o.z;
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
    Vector3<T> operator+()const {
        return Vector3<T>(x, y, z);
    }

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

    string ToString(int prec = -1)const {
        return '{' + str(x, prec) + ", " + str(y, prec) + ", " + str(z, prec) + '}';
    }

    //---------------- Utilities ----------------
    double InsideAngle(Vector3<T> v)const {
        return acos(Dot(v) / (Length() * v.Length()));
    }

    template <typename U>
    Vector3<T> Offset(U s)const {
        return Vector3<T>(x + s, y + s, z + s);
    }

    Vector3<T> X() {
        return Vector3<T>(x, 0, 0);
    }

    Vector3<T> Y()const {
        return Vector3<T>(0, y, 0);
    }

    Vector3<T> Z()const {
        return Vector3<T>(0, 0, z);
    }

    Vector3<T> XY()const {
        return Vector3<T>(x, y, 0);
    }

    Vector3<T> YZ()const {
        return Vector3<T>(0, y, z);
    }

    Vector3<T> XZ()const {
        return Vector3<T>(x, 0, z);
    }

    Vector3I Floor()const {
        return Vector3I(floor(x), floor(y), floor(z));
    }

    Vector3I Ceil()const {
        return Vector3I(ceil(x), ceil(y), ceil(z));
    }

    Vector3<T> Abs()const {
        return Vector3<T>(ABS(x), ABS(y), ABS(z));
    }
};

template <typename T>
const Vector3<T> Vector3<T>::ZERO = Vector3<T>(0, 0, 0);
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

// Vector3I looping macros for convenience and readability.
#define VEC3_RANGE(vec) \
    Vector3I p; \
    const Vector3I& _v = vec; \
    for(p.x = 0; p.x < _v.x; p.x++) \
    for(p.y = 0; p.y < _v.y; p.y++) \
    for(p.z = 0; p.z < _v.z; p.z++)

#define VEC3_RANGE_AB(start, end) \
    Vector3I p; \
    const Vector3I& _s = start; \
    const Vector3I& _e = end; \
    for(p.x = _s.x; p.x <= _e.x; p.x++) \
    for(p.y = _s.y; p.y <= _e.y; p.y++) \
    for(p.z = _s.z; p.z <= _e.z; p.z++)

#define VEC3_RANGE_OFFSET(off, vec) \
    Vector3I p; \
    const Vector3I& _o = off; \
    const Vector3I& _v = _o + vec; \
    for(p.x = _o.x; p.x < _v.x; p.x++) \
    for(p.y = _o.y; p.y < _v.y; p.y++) \
    for(p.z = _o.z; p.z < _v.z; p.z++)

#define SIDES(code) \
    Vector3I s; \
    s =  Vector3I::AXIS_X; code \
    s = -Vector3I::AXIS_X; code \
    s =  Vector3I::AXIS_Y; code \
    s = -Vector3I::AXIS_Y; code \
    s =  Vector3I::AXIS_Z; code \
    s = -Vector3I::AXIS_Z; code