#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <string>
#include "Transform2D.h"
#include <vector>

struct BaloonProperties {
	float translateX, translateY;
	float angle; // used to calculate displacement on Ox
	float speed;
	float scale;
	std::string color;
	bool alive = true;
	glm::mat3 matrix;

	BaloonProperties(const float translateX, const float translateY, const std::string& color, const float speed);

	void updateMatrix();
};

class Baloon : public SimpleScene
{
public:
	Baloon();
	~Baloon();

	void Init() override;
	void draw(glm::mat3 &matrix, const std::string& color, const bool alive, float& scale, const float deltaT);
	bool hitBy(const glm::mat3& matrix, const glm::vec3& arrowTip);
	void createBaloon(const std::string& name, const glm::vec3& color);
	void createBrokenBaloon(const std::string& name, const glm::vec3& color);

protected:
	const int numTriangles;
	const float radiusX, radiusY;
};
#pragma once
