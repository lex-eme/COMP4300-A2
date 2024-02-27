#pragma once

#include "Components.h"
#include <memory>
#include <string>

class Entity
{
	friend class EntityManager;

	bool m_Active = true;
	size_t m_ID = 0;
	const std::string m_Tag = "default";

	Entity(const size_t id, const std::string& tag);

public:
	std::shared_ptr<CTransform>		cTransform;
	std::shared_ptr<CShape>			cShape;
	std::shared_ptr<CCollision>		cCollision;
	std::shared_ptr<CInput>			cInput;
	std::shared_ptr<CScore>			cScore;
	std::shared_ptr<CLifeSpan>		cLifeSpan;
	std::shared_ptr<CSpecial>		cSpecial;

	bool isActive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();
};

