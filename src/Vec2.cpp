#include "Vec2.h"

#include <math.h>

#define PI 3.14159265

Vec2::Vec2() {}

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin) {}

Vec2::Vec2(int xin, int yin)
	: x((float)xin), y((float)yin) {}

bool Vec2::operator==(const Vec2& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return x != rhs.x || y != rhs.y;
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float val) const
{
	return Vec2(x * val, y * val);
}

Vec2 Vec2::operator/(const float val) const
{
	return Vec2(x / val, y /val);
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vec2& Vec2::operator*=(const float val)
{
	x *= val;
	y *= val;
	return *this;
}

Vec2& Vec2::operator/=(const float val)
{
	x /= val;
	y /= val;
	return *this;
}

float Vec2::dist(const Vec2& rhs) const
{
	float dx = rhs.x - x;
	float dy = rhs.y - y;
	return sqrtf(dx * dx + dy * dy);
}

float Vec2::mag() const
{
	return sqrt(x * x + y * y);
}

Vec2& Vec2::normalize()
{
	float m = mag();

	if (m != 0.0f && m != 1.0f) {
		*this /= m;
	}

	return *this;
}

Vec2& Vec2::setMag(float len) {
	normalize();
	*this *= len;
	return *this;
}

Vec2& Vec2::rotate(float theta)
{
	float t = x;
	float angle = theta * PI / 180.0f;
	x = x * cos(angle) - y * sin(angle);
	y = t * sin(angle) + y * cos(angle);
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Vec2& dt) {
	os << '(' << dt.x << ',' << dt.y << ')';
	return os;
}