#pragma once
#include <cmath>
#include "Vector.hpp"

template <typename T>
class QuaternionT {
public:
	T s;
	Vector3T<T> v;

	constexpr QuaternionT (T s, Vector3T<T> v) : s(s), v(std::move(v)) { }
	static QuaternionT<T> Rotation (T angle, Vector3T<T> axis) {
        axis.Normalize();
        return RotationN(std::move(angle), std::move(axis));
    }
    
	static QuaternionT<T> RotationN (T angle, Vector3T<T> normalizedAxis) {
        const T halfAng = angle/2;
        return QuaternionT<T> (cos(halfAng), sin(halfAng) * normalizedAxis);
    }

    // friend constexpr QuaternionT<T> operator*(const QuaternionT<T>& a, const QuaternionT<T>& b) {
    //     const T scalar = a.s * b.s - Vector3T<T>::Dot(a.v, b.v);
    //     const Vector3T<T> imaginary = b.v * a.s + a.v * b.s + Vector3T<T>::Cross(a.v, b.v);
    //     return QuaternionT<T>(std::move(scalar), std::move(imaginary));
    // }

    friend constexpr QuaternionT<T> operator*(const QuaternionT<T>& a, const QuaternionT<T>& b) {
        return QuaternionT<T> (
            a.s*b.s - a.v.x*b.v.x - a.v.y*b.v.y - a.v.z*b.v.z,
            Vector3T<T> (
                a.s*b.v.x + a.v.x*b.s + a.v.y*b.v.z - a.v.z*b.v.y,
                a.s*b.v.y + a.v.y*b.s + a.v.z*b.v.x - a.v.x*b.v.z,
                a.s*b.v.z + a.v.z*b.s + a.v.x*b.v.y - a.v.y*b.v.x
            )
        );
    }

    constexpr Vector3T<T> Rotate(Vector3T<T> point) const {
        const QuaternionT<T> q2 (this->s, -this->v);
        const QuaternionT<T> p (0, std::move(point));
        return QuaternionT<T>(*this * p * q2).v;
    }

	friend std::ostream& operator<<(std::ostream& o, const QuaternionT<T> &q) {
        return o << q.s << ' ' << q.v;
    }
};

typedef QuaternionT<VEC_REAL_T> Quaternion;

