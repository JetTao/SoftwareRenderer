//====================================
//      线性代数库（右手系）
//====================================  

#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <cstdlib>

//-----------------------------------------------------------------------
//                               向量         
//-----------------------------------------------------------------------

constexpr float PI = 3.1415926;
constexpr float EPSILON = 1e-5f;

template<size_t N, typename T> struct Vector {
    T m[N];
    inline Vector() { for (size_t i = 0; i < N; i++) m[i] = T(); }
    inline explicit Vector(const T *ptr) { for (size_t i = 0; i < N; i++) m[i] = ptr[i]; }
    inline Vector(const Vector<N, T> &v) { for (size_t i = 0; i < N; i++) m[i] = v.m[i]; }
    inline       T& operator[](size_t i)       { assert(i < N); return m[i]; }
    inline const T& operator[](size_t i) const { assert(i < N); return m[i]; }
};

template<typename T> struct Vector<2, T> { 
    union {
        struct { T x, y; };
        struct { T u, v; };
        T m[2];
    };

    Vector() = default;
    Vector(T X, T Y): x(X), y(Y) {}
    inline explicit Vector(T value) : x(value), y(value) {}
    inline Vector(const Vector<2, T> &v) = default;
    inline Vector(Vector<2, T> &&v) noexcept = default;
    inline explicit Vector(const T *ptr): x(ptr[0]), y(ptr[1]) {}
    inline Vector<2, T>& operator=(const Vector<2, T> &v) = default;
    inline Vector<2, T>& operator=(Vector<2, T> &&v) noexcept = default;
    inline       T& operator[](size_t i)       { assert(i < 2); return m[i]; }
    inline const T& operator[](size_t i) const { assert(i < 2); return m[i]; }
};

template<typename T> struct Vector<3, T> {
    union { 
        struct { T x, y, z; };
        struct { T r, g, b; };
        T m[3];
    };  
    inline Vector() = default;
    inline Vector(T X, T Y, T Z): x(X), y(Y), z(Z) {}
    inline explicit Vector(T value) : x(value), y(value), z(value) {}
    inline explicit Vector(const Vector<2, T> &v, T Z = static_cast<T>(1) ): x(v.x), y(v.y), z(Z) {}
    inline Vector(const Vector<3, T> &v) = default;
    inline explicit Vector(const Vector<4, T> &v): x(v.x), y(v.y), z(v.z) {}
    inline explicit Vector(Vector<2, T> &&v, T Z = static_cast<T>(1) ) noexcept : x(v.x), y(v.y), z(Z) { v.x = v.y = v.z = 0; }
    inline Vector(Vector<3, T> &&v) noexcept = default;
    inline explicit Vector(Vector<4, T> &&v) noexcept : x(v.x), y(v.y), z(v.z) { v.x = v.y = v.z = v.w = 0; }
    inline explicit Vector(const T *ptr): x(ptr[0]), y(ptr[1]), z(ptr[2]) {}
    inline static Vector<3, T> GetIdentity();
    inline Vector<3, T>& Homogenize();
    inline Vector<3, T>& operator=(const Vector<2, T> &v);
    inline Vector<3, T>& operator=(const Vector<3, T> &v) = default;
    inline Vector<3, T>& operator=(const Vector<4, T> &v);
    inline Vector<3, T>& operator=(Vector<2, T> &&v) noexcept;
    inline Vector<3, T>& operator=(Vector<3, T> &&v) noexcept = default;
    inline Vector<3, T>& operator=(Vector<4, T> &&v) noexcept;
    inline       T& operator[](size_t i)       { assert(i < 3); return m[i]; }
    inline const T& operator[](size_t i) const { assert(i < 3); return m[i]; }
};

template<typename T> 
inline Vector<3, T> Vector<3, T>::GetIdentity()
{
    Vector<3, T> ret;
    for (size_t i = 0; i < 3; i++) {
        ret[i] = static_cast<T>(1);
    }
    return ret;
}

template<typename T> 
inline Vector<3, T>& Vector<3, T>::Homogenize()
{
    for (size_t i = 0; i < 3; i++) {
        m[i] /= z;
    }
    return *this;
}

template<typename T>
inline Vector<3, T>& Vector<3, T>::operator=(const Vector<2, T> &v)
{
    x = v.x;
    y = v.y;
    z = static_cast<T>(1);
    return *this;
}

template<typename T>
inline Vector<3, T>& Vector<3, T>::operator=(const Vector<4, T> &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

template<typename T>
inline Vector<3, T>& Vector<3, T>::operator=(Vector<2, T> &&v) noexcept
{
    x = v.x;
    y = v.y;
    z = static_cast<T>(1);
    v.x = v.y = 0;
    return *this;
}

template<typename T>
inline Vector<3, T>& Vector<3, T>::operator=(Vector<4, T> &&v) noexcept
{
    x = v.x;
    y = v.y;
    z = v.z;
    v.x = v.y = v.z = v.w = 0;
    return *this;
}

template<typename T> struct Vector<4, T> {
    union { 
        struct { T x, y, z, w; };
        struct { T a, r, g, b; };
        T m[4];
    };

    inline Vector() = default;
    inline Vector(T X, T Y, T Z, T W): x(X), y(Y), z(Z), w(W) {}
    inline explicit Vector(T value) : x(value), y(value), z(value), w(value) {}
    inline explicit Vector(const Vector<3, T> &v, T W = static_cast<T>(1) )
    : x(v.x), y(v.y), z(v.z), w(W) {}
    
    inline Vector(const Vector<4, T> &v) = default;
    inline explicit Vector(Vector<3, T> &&v, T W = static_cast<T>(1) ) noexcept
    : x(v.x), y(v.y), z(v.z), w(W) { v.x = v.y = v.z = 0; }

    inline Vector(Vector<4, T> &&v) noexcept = default;
    inline static Vector<4, T> GetIdentity(); 
    inline Vector<4, T>& Homogenize();
    inline Vector<4, T>& operator=(const Vector<3, T> &v);
    inline Vector<4, T>& operator=(const Vector<4, T> &v) = default;
    inline Vector<4, T>& operator=(Vector<3, T> &&v) noexcept;
    inline Vector<4, T>& operator=(Vector<4, T> &&v) noexcept = default;
    inline       T& operator[](size_t i)       { assert(i < 4); return m[i]; }
    inline const T& operator[](size_t i) const { assert(i < 4); return m[i]; }
};

template<typename T> 
inline Vector<4, T> Vector<4, T>::GetIdentity()
{
    Vector<4, T> ret;
    for (size_t i = 0; i < 4; i++) {
        ret[i] = T(1);
    }
    return ret;
}

template<typename T> 
inline Vector<4, T>& Vector<4, T>::Homogenize()
{
    for (size_t i = 0; i < 4; i++) {
        m[i] /= w;
    }
    return *this;
}

template<typename T>
inline Vector<4, T>& Vector<4, T>::operator=(const Vector<3, T> &v)
{
   x = v.x;
   y = v.y; 
   z = v.z;
   w = static_cast<T>(1);

   return *this;
}


template<typename T>
inline Vector<4, T>& Vector<4, T>::operator=(Vector<3, T> &&v) noexcept
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = static_cast<T>(1);
    v.x = v.y = v.z = 0;
    return *this;
}


//-------------------------------------------------------------
//                        向量运算
//-------------------------------------------------------------
template<size_t N, typename T>
inline Vector<N, T> operator+(const Vector<N, T>& v) 
{
    return v;
}

template<size_t N, typename T>
inline Vector<N, T> operator-(const Vector<N, T>& v) 
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++) ret[i] = -v[i];
    return ret;
}

template<size_t N, typename T>
inline bool operator==(const Vector<N, T> &lhs, const Vector<N, T> &rhs) 
{
    for (size_t i = 0; i < N; i++) {
        if (lhs[i] - rhs[i] > EPSILON) {
            return false;
        } 
    }
    return true;
}

template<size_t N, typename T>
inline bool operator!=(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    return !(lhs == rhs);
}

template<size_t N, typename T>
inline Vector<N, T> operator+(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++) ret[i] = lhs[i] + rhs[i];
    return ret;
}

template<size_t N, typename T>
inline Vector<N, T> operator-(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++) ret[i] = lhs[i] - rhs[i];
    return ret;
}

template<size_t N, typename T>
inline Vector<N, T> operator*(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++) ret[i] = lhs[i] * rhs[i];
    return ret;
}

template<size_t N, typename T>
inline Vector<N, T> operator/(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++) ret[i] = lhs[i] / rhs[i];
    return ret;
}

template<size_t N, typename T>
inline Vector<N, T> operator*(const Vector<N, T> &lhs, const T &scale)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++) ret[i] = lhs[i] * scale;
    return ret;
}

template<size_t N, typename T>
inline Vector<N, T> operator*(const T &scale, const Vector<N, T> &lhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++) ret[i] = lhs[i] * scale;
    return ret;
}

template<size_t N, typename T>
inline Vector<N, T> operator/(const T &scale, const Vector<N, T> &rhs)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++) ret[i] = scale / rhs[i];
    return ret;
}

template<size_t N, typename T>
inline Vector<N, T> operator/(const Vector<N, T> &lhs, const T &scale)
{
    Vector<N, T> ret;
    for (size_t i = 0; i < N; i++) ret[i] = lhs[i] / scale;
    return ret;
}

template<size_t N, typename T>
inline Vector<N, T>& operator+=(Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    for (size_t i = 0; i < N; i++) lhs[i] += rhs[i];
    return lhs;
}

template<size_t N, typename T>
inline Vector<N, T>& operator-=(Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    for (size_t i = 0; i < N; i++) lhs[i] -= rhs[i];
    return lhs;
}

template<size_t N, typename T>
inline Vector<N, T>& operator*=(Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    for (size_t i = 0; i < N; i++) lhs[i] *= rhs[i];
    return lhs;
}

template<size_t N, typename T>
inline Vector<N, T>& operator/=(Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    for (size_t i = 0; i < N; i++) lhs[i] /= rhs[i];
    return lhs;
}

template<size_t N, typename T>
inline Vector<N, T>& operator*=(Vector<N, T>& lhs, const T &scale) {
	for (size_t i = 0; i < N; i++) lhs[i] *= scale;
	return lhs;
}

template<size_t N, typename T>
inline Vector<N, T>& operator*=(const T &scale, Vector<N, T>& lhs) {
	for (size_t i = 0; i < N; i++) lhs[i] *= scale;
	return lhs;
}

template<size_t N, typename T>
inline Vector<N, T>& operator/=(Vector<N, T>& lhs, const T &scale) {
	for (size_t i = 0; i < N; i++) lhs[i] /= scale;
	return lhs;
}

template<size_t N, typename T>
inline Vector<N, T>& operator/=(const T &scale, Vector<N, T>& lhs) {
	for (size_t i = 0; i < N; i++) lhs[i] = scale / lhs[i];
	return lhs;
}

template<size_t N, typename T>
inline std::ostream& operator<<(std::ostream &out, const Vector<N, T> &v)
{
    out << "( ";
    for (size_t i = 0; i < N; i++) {
        out << std::setw(5) << std::setprecision(5) << v[i];
        if (i < N - 1) 
            out << ", ";
    }
    out << " )";

    return out;
}

//-----------------------------------------------------------------
//                          向量函数
//-----------------------------------------------------------------

template<size_t N, typename T>
inline T Dot(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    T ret = 0;
    for (size_t i = 0; i < N; i++)  ret += lhs[i] * rhs[i];
    return ret;
}

template<typename T>
inline Vector<3, T> Cross(const Vector<3, T> &lhs, const Vector<3, T> &rhs)
{
    return Vector<3, T>(lhs.y * rhs.z - lhs.z * rhs.y,
                        lhs.z * rhs.x - lhs.x * rhs.z,
                        lhs.x * rhs.y - lhs.y * rhs.x);
}

template<typename T>
inline Vector<4, T> Cross(const Vector<4, T> &lhs, const Vector<4, T> &rhs)
{
    return Vector<4, T>(lhs.y * rhs.z - lhs.z * rhs.y,
                        lhs.z * rhs.x - lhs.x * rhs.z,
                        lhs.x * rhs.y - lhs.y * rhs.x,
                        lhs.w);
}

template<size_t N, typename T>
inline T VectorLengthSquare(const Vector<N, T> &v) 
{
    T sum = static_cast<T>(0);
    for (size_t i = 0; i < N; i++) sum += v[i] * v[i];
    return sum;
}

template<size_t N, typename T>
inline T VectorLength(const Vector<N, T> &v)
{
    return sqrt(VectorLengthSquare(v));
}

template<size_t N, typename T>
inline Vector<N, T> Normalize(const Vector<N, T> &v)
{
    Vector<N, T> ret = v / VectorLength(v);
    return ret;
}
 

//-----------------------------------------------------------
//                        矩阵
//-----------------------------------------------------------

template<size_t ROW, size_t COL, typename T> struct Matrix 
{
    T m[ROW][COL];
    inline Matrix() = default;
    inline explicit Matrix(const T value);
    inline Matrix(const Matrix<ROW, COL, T> &mat) = default; 
    inline Matrix(Matrix<ROW, COL, T> &&mat) noexcept = default;
    inline Matrix& operator=(const Matrix<ROW, COL, T> &mat) = default; 
    inline Matrix& operator=(Matrix<ROW, COL, T> &&mat) noexcept = default; 
    inline T* operator[] (const size_t row) { assert(row < ROW); return m[row]; }
    inline const T* operator[] (const size_t row) const { assert(row < ROW); return m[row]; }
    inline Vector<COL, T> Row(const size_t row) const; 
    inline Vector<ROW, T> Col(const size_t col) const;
    inline void SetRow(const size_t row, const Vector<COL, T> &v);
    inline void SetCol(const size_t col, const Vector<ROW, T> &v);
    inline Matrix<ROW - 1, COL - 1, T> GetMinor(const size_t row, const size_t col) const;
    inline Matrix<COL, ROW, T> GetTranspose() const;
    inline static Matrix<ROW, COL, T> GetZero();
    inline static Matrix<ROW, COL, T> GetIdentity();
};

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T>::Matrix(const T value)
{
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            m[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
        }
    }
}

template<size_t ROW, size_t COL, typename T>
inline Vector<COL, T> Matrix<ROW, COL, T>::Row(const size_t row) const
{
    assert(row < ROW);
    Vector<COL, T>  ret;
    for (size_t i = 0; i < COL; i++) { ret[i] = m[row][i]; }
    return ret;
}

template<size_t ROW, size_t COL, typename T>
inline Vector<ROW, T> Matrix<ROW, COL, T>::Col(const size_t col) const
{
    assert(col < COL);
    Vector<ROW, T> ret;
    for (size_t i = 0; i < ROW; i++) { ret[i] = m[i][col]; }
    return ret;
}

template<size_t ROW, size_t COL, typename T>
inline void Matrix<ROW, COL, T>::SetRow(const size_t row, const Vector<COL, T> &v)
{
    assert(row < ROW);
    for (size_t i = 0; i < COL; i++) { m[row][i] = v[i]; }
}

template<size_t ROW, size_t COL, typename T> 
inline void Matrix<ROW, COL, T>::SetCol(const size_t col, const Vector<ROW, T> &v)
{
    assert(col < COL);
    for (size_t i = 0; i < ROW; i++) { m[i][col] = v[i]; }
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW - 1, COL - 1, T> Matrix<ROW, COL, T>::GetMinor(const size_t row, const size_t col) const
{
    Matrix<ROW - 1, COL - 1, T> ret;
    for (size_t i = 0; i < ROW - 1; i++) {
        for (size_t j = 0; j < COL - 1; j++) {
            ret[i][j] = m[i < row ? i : i + 1][j < col ? j : j + 1];
        }
    }
    return ret;
}

template<size_t ROW, size_t COL, typename T> 
inline Matrix<COL, ROW, T> Matrix<ROW, COL, T>::GetTranspose() const 
{
    Matrix<COL, ROW, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            ret[j][i] = m[i][j];
        }
    }
    return ret; 
}
    
template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> Matrix<ROW, COL, T>::GetZero() 
{
    Matrix<ROW, COL, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            ret[i][j] = static_cast<T>(0);
        }
    }
    return ret;
}

template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> Matrix<ROW, COL, T>::GetIdentity()
{
    Matrix<ROW, COL, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            ret[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
        }
    }
    return ret;
}

template<size_t ROW, size_t COL, typename T> 
inline std::ostream& operator<<(std::ostream &out, const Matrix<ROW, COL, T> &mat)
{
    for (size_t i = 0; i < ROW; i++) {
        out << "( ";
        for (size_t j = 0; j < COL; j++) {
            out << std::setw(5) << std::setprecision(5)  << mat[i][j];
            if (j != COL - 1) {
                out << ", ";
            } 
        }
        out << " )\n";
    }
    return out;
}


//--------------------------------------------------
//                   矩阵运算(全局函数)
//--------------------------------------------------

template<size_t ROW, size_t COL, typename T> 
inline bool operator==(const Matrix<ROW, COL, T> &lhs, const Matrix<ROW, COL, T> &rhs)
{ 
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            if (lhs[i][j] - rhs[i][j] > EPSILON) 
                return false;
        }
    }
    return true;
}

template<size_t ROW, size_t COL, typename T> 
inline bool operator!=(const Matrix<ROW, COL, T> &lhs, const Matrix<ROW, COL, T> &rhs)
{
    return !(lhs == rhs);
}

template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> operator+(const Matrix<ROW, COL, T> &mat)
{
    return mat;
}

template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> operator-(const Matrix<ROW, COL, T> &mat)
{
    Matrix<ROW, COL, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            ret[i][j] = -mat.m[i][j];
        }
    }
    return ret;
}

template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> operator+(const Matrix<ROW, COL, T> &lhs, const Matrix<ROW, COL, T> &rhs)
{
    Matrix<ROW, COL, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            ret[i][j] = lhs[i][j] + rhs[i][j];
        }
    }
    return ret;
}

template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> operator-(const Matrix<ROW, COL, T> &lhs, const Matrix<ROW, COL, T> &rhs)
{
    Matrix<ROW, COL, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            ret[i][j] = lhs[i][j] - rhs[i][j];
        }
    }
    return ret;
}

template<size_t ROW, size_t COL, size_t NEWCOL, typename T> 
inline Matrix<ROW, NEWCOL, T> operator*(const Matrix<ROW, COL, T> &lhs, const Matrix<COL, NEWCOL, T> &rhs)
{
    Matrix<ROW, NEWCOL, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < NEWCOL; j++) {
            ret[i][j] = Dot(lhs.Row(i), rhs.Col(j));
        }
    }
    return ret;
}

template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> operator*(const Matrix<ROW, COL, T> &lhs, T scale) 
{
    Matrix<ROW, COL, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            ret[i][j] = lhs[i][j] * scale;
        }
    }
    return ret;
}


template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> operator/(const Matrix<ROW, COL, T> &lhs, T scale) 
{
    Matrix<ROW, COL, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            ret[i][j] = lhs[i][j] / scale;
        }
    }
    return ret;
}

template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> operator*(T scale, const Matrix<ROW, COL, T> &rhs) 
{
    return (rhs * scale);
}

template<size_t ROW, size_t COL, typename T> 
inline Matrix<ROW, COL, T> operator/(T scale, const Matrix<ROW, COL, T> &rhs) 
{
    Matrix<ROW, COL, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; i < COL; j++) {
            ret[i][j] = scale / rhs[i][j];
        }
    }
    return ret;
}

template<size_t ROW, size_t COL, typename T>
inline Vector<ROW, T> operator*(const Matrix<ROW, COL, T> &lhs, const Vector<COL, T> &v)
{
    Vector<ROW, T> ret;
    for (size_t i = 0; i < ROW; i++) {
        ret[i] = Dot(lhs.Row(i), v);
    }
    return ret;
}

//---------------------------------------------------------
//                     矩阵/行列式函数（全局函数）
//---------------------------------------------------------

template<typename T>
inline T MatrixDet(const Matrix<1, 1, T> &mat)
{
    return mat[0][0];
}

template<typename T> 
inline T MatrixDet(const Matrix<2, 2, T> &mat)
{
    return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

template<size_t N, typename T> 
inline T MatrixDet(const Matrix<N, N, T> &mat)
{
    T ret = 0;
    for (size_t i = 0; i < N; i++) {
        ret +=  mat[0][i] * MatrixCofactor(mat, 0, i);
    }
    return ret;
}

template<typename T>
inline T MatrixCofactor(const Matrix<1, 1, T> &mat, size_t row, size_t col)
{
    return 0;
}

template<size_t N, typename T> 
inline T MatrixCofactor(const Matrix<N, N, T> &mat, size_t row, size_t col)
{
    return MatrixDet(mat.GetMinor(row, col)) * (((row + col) % 2) ? -1 : 1);
}

template<size_t N, typename T> 
inline Matrix<N, N, T> MatrixAdjoint(const Matrix<N, N, T> &mat)
{
    Matrix<N, N, T> ret;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            ret[i][j] = MatrixCofactor(mat, j, i);
        }
    }
    return ret;
}

template<size_t N, typename T> 
inline Matrix<N, N, T> MatrixInvert(const Matrix<N, N, T> &mat)
{
    Matrix<N, N, T> ret = MatrixAdjoint(mat);
    T det = Dot(mat.Row(0), ret.Col(0));
    return ret / det;
}



using Vec2f = Vector<2, float>;
using Vec3f = Vector<3, float>;
using Vec4f = Vector<4, float>;
using Mat4f = Matrix<4, 4, float>;

using Point2i = Vector<2, int>;
using Point2f = Vector<2, float>;
using Point3f = Vector<3, float>;
using Point4f = Vector<4, float>;

using Color3f = Vector<3, float>;
using Color4f = Vector<4, float>;

//-----------------------------------------------------
//                     3D仿射变换
//-----------------------------------------------------

//角度转弧度

inline float Radians(const float degrees)
{
    return degrees * PI / 180.0f;
}

inline Mat4f& Translate(Mat4f &ret, const float x, const float y, const float z)
{
    Mat4f mat = Mat4f::GetIdentity();
    mat[0][3] = x;
    mat[1][3] = y;
    mat[2][3] = z;
    ret = mat * ret;
    return ret;
}

inline Mat4f& Translate(Mat4f &ret, const Vec3f &v)
{
    Mat4f mat = Mat4f::GetIdentity();
    mat[0][3] = v.x;
    mat[1][3] = v.y;
    mat[2][3] = v.z;
    ret = mat * ret;
    return ret;
}


//缩放
inline Mat4f& Scale(Mat4f &ret, const float x, const float y, const float z) 
{
    Mat4f mat = Mat4f::GetIdentity();
    mat[0][0] = x;
    mat[1][1] = y;
    mat[2][2] = z;
    ret = mat * ret;
    return ret;
}

inline Mat4f& Scale(Mat4f &ret, const Vec3f &v) 
{
    Mat4f mat = Mat4f::GetIdentity();
    mat[0][0] = v.x;
    mat[1][1] = v.y;
    mat[2][2] = v.z;
    ret = mat * ret;
    return ret;
}


//绕任意轴旋转
inline Mat4f& Rotate(Mat4f &ret, const float theta, const Vec3f axis)
{
    Vec3f unitVec{ Normalize(axis) };
    const float x = unitVec.x;
    const float y = unitVec.y;
    const float z = unitVec.z;

    // std::cout  << unitVec << std::endl;
    const float sinTheta = sin(Radians(theta));
    const float cosTheta = cos(Radians(theta));

    Mat4f mat = Mat4f::GetIdentity();

    mat[0][0] = cosTheta + x * x * (1.0f - cosTheta); 
    mat[0][1] = x * y * (1.0f - cosTheta) - z * sinTheta;
    mat[0][2] = x * z * (1.0f - cosTheta) + y * sinTheta;

    mat[1][0] = y * x * (1.0f - cosTheta) + z * sinTheta;
    mat[1][1] = cosTheta + y * y * (1.0f - cosTheta);
    mat[1][2] = y * z * (1.0f - cosTheta) - x * sinTheta;

    mat[2][0] = z * x * (1.0f - cosTheta) - y * sinTheta;
    mat[2][1] = z * y * (1.0f - cosTheta) + x * sinTheta;
    mat[2][2] = cosTheta + z * z * (1.0f - cosTheta);

    mat[3][0] = mat[3][1] = mat[3][2] = 0.0f;
    mat[0][3] = mat[1][3] = mat[2][3] = 0.0f;

    // std::cout << mat << std::endl;
    ret = mat * ret;
    
    return ret;
}

//将世界坐标变换为屏幕坐标，参数为窗口的分辨率
inline Mat4f Viewport(const int width, const int height)
{
    Mat4f ret = Mat4f::GetIdentity();
    ret[0][0] = width  / 2.0f;
    ret[1][1] = height / 2.0f;
    ret[0][3] = width  / 2.0f;
    ret[1][3] = height / 2.0f;
    
    return ret;
}

//以相机位置为原点建立uvw坐标系，空间内所有的物体一起做相对运动
inline Mat4f LookAt(const Vec3f &eye, const Vec3f &gaze, const Vec3f &up)
{
    Vec3f w{ -gaze };
    Vec3f u{ Normalize(Cross(up, w)) };
    Vec3f v{ Cross(w, u) };

    Mat4f lhs = Mat4f::GetIdentity();

    lhs.SetRow(0, Vec4f(u.x, u.y, u.z, 0.0f));
    lhs.SetRow(1, Vec4f(v.x, v.y, v.z, 0.0f));
    lhs.SetRow(2, Vec4f(w.x, w.y, w.z, 0.0f));
    lhs.SetRow(3, Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

    Mat4f rhs = Mat4f::GetIdentity();
    rhs = Translate(rhs, -eye.x, -eye.y, -eye.z);
    // std::cout << "lhs: \n" << lhs << std::endl;
    // std::cout << "eye: \n" << eye << std::endl;
    // std::cout << "rhs: \n" << rhs << std::endl;
    // std::cout << "ret: \n" << ret << std::endl;

    return lhs * rhs;
}

//透视投影
inline Mat4f Perspective(const float fov, const float aspect, const float n, const float f)
{
    const float zNear = std::abs(n);
    const float zFar = std::abs(f);
    const float t = zNear * tan(Radians(fov) * 0.5f); 
    const float b = -t;
    const float r = t * aspect;
    const float l = -r;

    // std::cout << "fov: " << fov << "tan(fov / 2)" << tan(fov * 0.5f) << std::endl;
    //std::cout << "t: " << t << " b: " << b << " r: " << r << " l" << l << " n: " << zNear << " f: " << zFar << std::endl; 

    Mat4f mat = Mat4f::GetZero();

    mat[0][0] = (2.0f * zNear) / (r - l);
    mat[0][2] = (r + l) / (r - l);
    mat[1][1] = (2.0f * zNear) / (t - b);
    mat[1][2] = (t + b) / (t - b);
    mat[2][2] = (zNear + zFar) / (zNear - zFar);
    mat[2][3] = (2.0f * zFar * zNear) / (zNear - zFar);
    mat[3][2] = -1.0f;

    // std::cout << mat << std::endl;

    return mat;
}


static float Max(const float& lhs, const float& rhs)
{
    return lhs > rhs ? lhs : rhs;
}