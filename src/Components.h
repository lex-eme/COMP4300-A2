#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

struct CTransform
{
	Vec2 pos = { 0.0f, 0.0f };
	Vec2 velocity = { 0.0f, 0.0f };
	float angle = 0.0f;

	CTransform(const Vec2& p, const Vec2& v, float a)
		: pos(p), velocity(v), angle(a) {}
};

struct CShape
{
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

struct CCollision
{
	float radius = 0.0f;

	CCollision(float r)
		: radius(r) {}
};

struct CScore
{
	int score = 0;

	CScore(int s)
		: score(s) {}
};

struct CLifeSpan
{
	int remaining = 0;
	int total = 0;

	CLifeSpan(int total)
		: remaining(total), total(total) {}
};

struct CInput
{
	bool up		= false;
	bool left	= false;
	bool right	= false;
	bool down	= false;

	CInput() {}
};

struct CSpecial
{
	Vec2 target;
	float strength;

	CSpecial(const Vec2& tar, float s)
		: target(tar), strength(s) {}
};