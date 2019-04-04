#pragma once

class Vector3 final
{
public:
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Right;
	static const Vector3 Left;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Forward;
	static const Vector3 Back;
	static const Vector3 Infinity;
	static const Vector3 NegInfinity;

public:
	static const float Distance(const Vector3& lhs, const Vector3& rhs);
	static const float DistanceSq(const Vector3& lhs, const Vector3& rhs);
	static const float Dot(const Vector3& lhs, const Vector3& rhs);

	static const Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	static const Vector3 Transform(const Vector3& lhs, const class Matrix& rhs);
	static const Vector3 TransformCoord(const Vector3& lhs, const class Matrix& rhs);
	static const Vector3 TransformNormal(const Vector3& lhs, const class Matrix& rhs);


public:
	Vector3()
		: x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(const float& x, const float& y, const float& z)
		: x(x), y(y), z(z) {}
	Vector3(const float& rhs)
		: x(rhs), y(rhs), z(rhs) {}
	Vector3(const Vector3& rhs)
		: x(rhs.x), y(rhs.y), z(rhs.z) {}
	Vector3(const class Vector2 &rhs);
	~Vector3() {}

	const float Length() const { return sqrt(x * x + y * y + z * z); }
	const float LengthSq() const { return x * x + y * y + z * z; }
	const float Volume() const { return x * y * z; }
	const float Dot(const Vector3& rhs) const { return Dot(*this, rhs); }

	const Vector3 Cross(const Vector3& rhs) const { return Cross(*this, rhs); }
	const Vector3 Absolute() const { return Vector3(fabs(x), fabs(y), fabs(z)); }
	const Vector3 Normalize() const;
	void Normalize();

	operator float*() { return &x; }
	operator const float*() const { return &x; }

	const Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	const Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	const Vector3 operator*(const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
	const Vector3 operator/(const Vector3& rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }

	const Vector3 operator+(const float& rhs) const { return Vector3(x + rhs, y + rhs, z + rhs); }
	const Vector3 operator-(const float& rhs) const { return Vector3(x - rhs, y - rhs, z - rhs); }
	const Vector3 operator*(const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	const Vector3 operator/(const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }

	void operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; }
	void operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; }
	void operator*=(const Vector3& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; }
	void operator/=(const Vector3& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; }

	void operator+=(const float& rhs) { x += rhs; y += rhs; z += rhs; }
	void operator-=(const float& rhs) { x -= rhs; y -= rhs; z -= rhs; }
	void operator*=(const float& rhs) { x *= rhs; y *= rhs; z *= rhs; }
	void operator/=(const float& rhs) { x /= rhs; y /= rhs; z /= rhs; }

	const bool operator==(const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	const bool operator!=(const Vector3& rhs) const { return !(*this == rhs); }

public:
	float x;
	float y;
	float z;
};