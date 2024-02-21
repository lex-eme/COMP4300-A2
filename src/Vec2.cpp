#include "Vec2.h"

#include <math.h>

Vec2::Vec2()
{
}

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin)
{
}

Vec2::Vec2(int xin, int yin)
	: x((float)xin), y((float)yin)
{
}

bool Vec2::operator==(const Vec2& rhs) const
{
	return false;
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return false;
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2();
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2();
}

Vec2 Vec2::operator*(const float val) const
{
	return Vec2();
}

Vec2 Vec2::operator/(const float val) const
{
	return Vec2();
}

void Vec2::operator+=(const Vec2& rhs)
{
}

void Vec2::operator-=(const Vec2& rhs)
{
}

void Vec2::operator*=(const float val)
{
}

void Vec2::operator/=(const float val)
{
}

float Vec2::dist(const Vec2& rhs) const
{
	return 0.0f;
}
