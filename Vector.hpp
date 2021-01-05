#pragma once
#include <cmath>
#include <array>

#ifndef VEC_REAL_T
typedef float VEC_REAL_T;
#endif

template <typename T>
struct Vector3T {
	Vector3T() = default;
	constexpr Vector3T(T x, T y, T z) : x(x), y(y), z(z) { }
	explicit constexpr Vector3T(T v) : x(v), y(v), z(v) { }
	explicit constexpr Vector3T(const std::array<T, 3>& v) : x(v[0]), y(v[1]), z(v[2]) { }
	constexpr Vector3T(const Vector3T<T>& v) : x(v.x), y(v.y), z(v.z) { }
	Vector3T<T>& operator=(const Vector3T<T>& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	explicit constexpr operator bool() const {
		return x || y || z;
	}
	constexpr bool operator==(const Vector3T<T>& other) const {
		return x == other.x && y == other.y && z == other.z;
	}
	constexpr bool operator!=(const Vector3T<T>& other) const {
		return !(*this == other);
	}
	constexpr T& operator[](int i) {
		return ((T*) this)[i];
	}
	constexpr T operator[](int i) const {
		return ((T*) this)[i];
	}
	constexpr Vector3T<T>& operator+=(const Vector3T<T>& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	constexpr Vector3T<T>& operator-=(const Vector3T<T>& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	constexpr Vector3T<T>& operator*=(T v) {
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}
	constexpr Vector3T<T>& operator/=(T v) {
		x /= v;
		y /= v;
		z /= v;
		return *this;
	}
	friend constexpr Vector3T<T> operator*(const Vector3T<T>& v, T s) {
	    return Vector3T<T> {v.x * s, v.y * s, v.z * s};
	}
	friend constexpr Vector3T<T> operator*(T s, const Vector3T<T>& v) {
	    return Vector3T<T> {v.x * s, v.y * s, v.z * s};
	}
	constexpr Vector3T<T> operator/(T v) const {
	    return Vector3T<T> {x / v, y / v, z / v};
	}
	constexpr Vector3T<T> operator+(const Vector3T<T>& v) const {
		return Vector3T {x + v.x, y + v.y, z + v.z};
	}
	constexpr Vector3T<T> operator-(const Vector3T<T>& v) const {
		return Vector3T {x - v.x, y - v.y, z - v.z};
	}
	constexpr Vector3T<T> operator-() const {
		return Vector3T {-x, -y, -z};
	}
	constexpr VEC_REAL_T Magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}
	constexpr T MagnitudeSqr() const {
		return (x * x + y * y + z * z);
	}
	static constexpr Vector3T<T> Rotate(const Vector3T<T>& vPoint, Vector3T<T> vAxis, VEC_REAL_T angle) {
		vAxis.Normalize();
		const VEC_REAL_T half_ang = angle / 2;
		const VEC_REAL_T sin_half_ang = sin(half_ang);
		const VEC_REAL_T s1 = cos(half_ang);
		const Vector3T<T> v1 {sin_half_ang * vAxis.x, sin_half_ang * vAxis.y, sin_half_ang * vAxis.z};
		const Vector3T<T> v3 = -v1;
		const VEC_REAL_T s12 = -Vector3T<T>::Dot(v1, vPoint);
		const Vector3T<T> v12 = vPoint * s1 + Vector3T<T>::Cross(v1, vPoint);
		return v3 * s12 + v12 * s1 + Vector3T<T>::Cross(v12, v3);
	}
	///Remember to check if magnitude is zero
	constexpr Vector3T<T> Normalized() const {
		return *this / Magnitude();
	}
	///Remember to check if magnitude is zero
	constexpr void Normalize() {
		*this /= Magnitude();
	}
	///Remember to check if magnitude is zero
	constexpr void SetMagnitude(VEC_REAL_T mag) {
		*this *= mag / Magnitude();
	}
	///Remember to check if magnitude is zero
	constexpr void ClampMagnitude(VEC_REAL_T mag) {
		if (Magnitude() > mag) { *this *= mag / Magnitude(); }
	}
	///Remember to check if magnitude is zero
	constexpr void SetMax(VEC_REAL_T mag) {
		const VEC_REAL_T fmax = std::max(std::max(abs(x), abs(y)), abs(z));
		*this *= mag / fmax;
	}
	constexpr T Max() const {
		return std::max(std::max(abs(x), abs(y)), abs(z));
	}
	///Remember to check if magnitude is zero
	static constexpr VEC_REAL_T AngleBetween(const Vector3T<T>& v1, const Vector3T<T>& v2) {
		return acos(AngleBetweenCos(v1, v2));
	}
	///Remember to check if magnitude is zero
	static constexpr VEC_REAL_T AngleBetweenCos(const Vector3T<T>& v1, const Vector3T<T>& v2) {
		const VEC_REAL_T lenlen = sqrt(v1.MagnitudeSqr() * v2.MagnitudeSqr());
		return Dot(v1, v2) / lenlen;
	}
	///Can be negative
	static constexpr VEC_REAL_T ProjectionLength(const Vector3T<T>& v, const Vector3T<T>& vProjectOn) {
		return v.Magnitude() * AngleBetweenCos(v, vProjectOn);
	}
	static constexpr Vector3T<T> Projection(const Vector3T<T>& v, const Vector3T<T>& vProjectOn) {
		const VEC_REAL_T magsqr = vProjectOn.MagnitudeSqr();
		return vProjectOn * (Dot(v, vProjectOn) / magsqr);
	}
	///Aka perpendicular projection
	static constexpr Vector3T<T> ProjectionOnPlane(const Vector3T<T>& v, const Vector3T<T>& planeNormal) {
		const T magsqr = planeNormal.MagnitudeSqr();
		return v - planeNormal * (Dot(planeNormal, v) / magsqr);
	}
	static constexpr T Dot(const Vector3T<T>& v1, const Vector3T<T>& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	static constexpr Vector3T<T> Cross(const Vector3T<T>& v1, const Vector3T<T>& v2) {
		return Vector3T<T> {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
	}
	static constexpr Vector3T<T> Lerp(const Vector3T<T>& from, const Vector3T<T>& to, VEC_REAL_T t) {
		return Vector3T<T> {from.x + (to.x - from.x) * t, from.y + (to.y - from.y) * t, from.z + (to.z - from.z) * t};
	}
	static constexpr VEC_REAL_T Distance(const Vector3T<T>& a, const Vector3T<T>& b) {
	    return (b - a).Magnitude();
	}
	friend std::ostream& operator<<(std::ostream& o, const Vector3T<T> &v) {
        return o << '{' << v.x << ", " << v.y << ", " << v.z << '}';
    }
	T x, y, z;
};


template <typename T>
struct Vector2T {
	Vector2T() = default;
	constexpr Vector2T(T x, T y) : x(x), y(y) { }
	explicit constexpr Vector2T(T v) : x(v), y(v) { }
	explicit constexpr Vector2T(const std::array<T, 2>& v) : x(v[0]), y(v[1]) { }
	constexpr Vector2T(const Vector2T<T>& v) : x(v.x), y(v.y) { }
	Vector2T<T>& operator=(const Vector2T<T>& v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	explicit constexpr operator bool() const {
		return x || y;
	}
	constexpr bool operator==(const Vector2T<T>& other) const {
		return x == other.x && y == other.y;
	}
	constexpr bool operator!=(const Vector2T<T>& other) const {
		return !(*this == other);
	}
	T& operator[](int i) {
		return ((T*) this)[i];
	}
	constexpr T operator[](int i) const {
		return ((T*) this)[i];
	}
	constexpr Vector2T<T>& operator+=(const Vector2T<T>& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	constexpr Vector2T<T>& operator-=(const Vector2T<T>& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	constexpr Vector2T<T>& operator*=(T v) {
		x *= v;
		y *= v;
		return *this;
	}
	constexpr Vector2T<T>& operator/=(T v) {
		x /= v;
		y /= v;
		return *this;
	}
	friend constexpr Vector3T<T> operator*(const Vector2T<T>& v, T s) {
	    return Vector2T<T> {v.x * s, v.y * s};
	}
	friend constexpr Vector3T<T> operator*(T s, const Vector2T<T>& v) {
	    return Vector2T<T> {v.x * s, v.y * s};
	}
	constexpr Vector2T<T> operator/(T v) const {
	    return Vector2T<T> {x / v, y / v};
	}
	constexpr Vector2T<T> operator+(const Vector2T<T>& v) const {
		return Vector2T {x + v.x, y + v.y};
	}
	constexpr Vector2T<T> operator-(const Vector2T<T>& v) const {
		return Vector2T {x - v.x, y - v.y};
	}
	constexpr Vector2T<T> operator-() const {
		return Vector2T {-x, -y};
	}
	constexpr VEC_REAL_T Magnitude() const {
		return sqrt(x * x + y * y);
	}
	constexpr T MagnitudeSqr() const {
		return (x * x + y * y);
	}
	///TODO
	static constexpr Vector2T<T> Rotate(const Vector2T<T>& vPoint, Vector2T<T> vAxis, VEC_REAL_T angle) = delete;
	///Remember to check if magnitude is zero
	constexpr Vector2T<T> Normalized() const {
		return *this / Magnitude();
	}
	///Remember to check if magnitude is zero
	constexpr void Normalize() {
		*this /= Magnitude();
	}
	///Remember to check if magnitude is zero
	constexpr void SetMagnitude(VEC_REAL_T mag) {
		*this *= mag / Magnitude();
	}
	///Remember to check if magnitude is zero
	constexpr void ClampMagnitude(VEC_REAL_T mag) {
		if (Magnitude() > mag) { *this *= mag / Magnitude(); }
	}
	///Remember to check if magnitude is zero
	constexpr void SetMax(VEC_REAL_T mag) {
		const T fmax = std::max(abs(x), abs(y));
		*this *= mag / fmax;
	}
	constexpr T Max() const {
		return std::max(abs(x), abs(y));
	}
	///Remember to check if magnitude is zero
	static constexpr VEC_REAL_T AngleBetween(const Vector2T<T>& v1, const Vector2T<T>& v2) {
		return acos(AngleBetweenCos(v1, v2));
	}
	///Remember to check if magnitude is zero
	static constexpr VEC_REAL_T AngleBetweenCos(const Vector2T<T>& v1, const Vector2T<T>& v2) {
		const VEC_REAL_T lenlen = sqrt(v1.MagnitudeSqr() * v2.MagnitudeSqr());
		return Dot(v1, v2) / lenlen;
	}
	///Can be negative
	static constexpr VEC_REAL_T ProjectionLength(const Vector2T<T>& v, const Vector2T<T>& vProjectOn) {
		return Dot(v, vProjectOn) / vProjectOn.Magnitude();
	}
	static constexpr Vector2T<T> Projection(const Vector2T<T>& v, const Vector2T<T>& vProjectOn) {
		const T magsqr = vProjectOn.MagnitudeSqr();
		return vProjectOn * (Dot(v, vProjectOn) / magsqr);
	}
	static constexpr T Dot(const Vector2T<T>& v1, const Vector2T<T>& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static constexpr Vector2T<T> Lerp(const Vector2T<T>& from, const Vector2T<T>& to, VEC_REAL_T t) {
		return Vector2T<T> {from.x + (to.x - from.x)* t, from.y + (to.y - from.y)* t};
	}
	static constexpr VEC_REAL_T Distance(const Vector2T<T>& a, const Vector2T<T>& b) {
	    return (b - a).Magnitude();
	}
	friend std::ostream& operator<<(std::ostream& o, const Vector2T<T> &v) {
        return o << '{' << v.x << ", " << v.y << '}';
    }
	T x, y;
};

typedef Vector3T<VEC_REAL_T> Vector3;
typedef Vector2T<VEC_REAL_T> Vector2;
