#pragma once

#include <iostream>

struct Vec2
{
	float x = 0.0f;
	float y = 0.0f;

	Vec2();
	Vec2(float xin, float yin);
	Vec2(int xin, int yin);

	bool operator==(const Vec2& rhs) const;
	bool operator!=(const Vec2& rhs) const;

	Vec2 operator+(const Vec2& rhs) const;
	Vec2 operator-(const Vec2& rhs) const;
	Vec2 operator*(const float val) const;
	Vec2 operator/(const float val) const;

	Vec2& operator+=(const Vec2& rhs);
	Vec2& operator-=(const Vec2& rhs);
	Vec2& operator*=(const float val);
	Vec2& operator/=(const float val);

	float dist(const Vec2& rhs) const;

	friend std::ostream& operator<<(std::ostream& os, const Vec2& dt);
};

