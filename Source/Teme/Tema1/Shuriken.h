#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include "Transform2D.h"
#include <vector>

struct ShurikenProperties {
	float translateX, translateY;
	float angle;
	float speed;
	float scale;
	bool alive;
	glm::mat3 matrix;

	ShurikenProperties(const float translateX, const float translateY, const float angle, const float speed, const float scale);

	void updateMatrix();
};

class Shuriken : public SimpleScene
{
public:
	Shuriken();
	~Shuriken();

	void Init() override;
	void draw(glm::mat3 &matrix, const bool alive);
	void setSize(const float size);
	bool hitBy(const glm::mat3& matrix, const glm::vec3& arrowTip);
	float getHitboxRadius();

protected:
	float size;
};
#pragma once
