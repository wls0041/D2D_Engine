#pragma once

class Color final
{
public:
	static const Color White;
	static const Color Black;
	static const Color Red;
	static const Color Blue;
	static const Color Green;

public:
	Color()
		: r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
	Color(const float& r, const float& g, const float& b, const float& a)
		: r(r), g(g), b(b), a(a) {}
	Color(const float& val)
		: r(val), g(val), b(val), a(1.0f) {}
	Color(const Color& rhs)
		: r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a) {}
	Color(const uint& rhs);
	~Color() = default;

	operator uint() const;
	operator float*() { return &r; }
	operator const float*() const { return &r; }

	const Color operator+(const Color& rhs) const { return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a); }
	const Color operator-(const Color& rhs) const { return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a); }
	const Color operator*(const Color& rhs) const { return Color(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a); }
	const Color operator/(const Color& rhs) const { return Color(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a); }

	const Color operator+(const float& rhs) const { return Color(r + rhs, g + rhs, b + rhs, a + rhs); }
	const Color operator-(const float& rhs) const { return Color(r - rhs, g - rhs, b - rhs, a - rhs); }
	const Color operator*(const float& rhs) const { return Color(r * rhs, g * rhs, b * rhs, a * rhs); }
	const Color operator/(const float& rhs) const { return Color(r / rhs, g / rhs, b / rhs, a / rhs); }

	void operator+=(const Color& rhs) { r += rhs.r; g += rhs.g; b += rhs.b; a += rhs.a; }
	void operator-=(const Color& rhs) { r -= rhs.r; g -= rhs.g; b -= rhs.b; a -= rhs.a; }
	void operator*=(const Color& rhs) { r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a; }
	void operator/=(const Color& rhs) { r /= rhs.r; g /= rhs.g; b /= rhs.b; a /= rhs.a; }

	void operator+=(const float& rhs) { r += rhs; g += rhs; b += rhs; a += rhs; }
	void operator-=(const float& rhs) { r -= rhs; g -= rhs; b -= rhs; a -= rhs; }
	void operator*=(const float& rhs) { r *= rhs; g *= rhs; b *= rhs; a *= rhs; }
	void operator/=(const float& rhs) { r /= rhs; g /= rhs; b /= rhs; a /= rhs; }

	const bool operator==(const Color& rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }
	const bool operator!=(const Color& rhs) const { return !(*this == rhs); }

public:
	float r;
	float g;
	float b;
	float a;
};