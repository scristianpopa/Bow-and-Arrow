#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include "Transform2D.h"
#include <vector>

struct ArrowProperties {
	float translateX, translateY;
	float displacementX, displacementY; // original displacement when launching the arrow
	float angle; 
	float speed;
	float gravityAffect;
	glm::mat3 matrix;

	ArrowProperties(const float translateX, const float translateY, const float displacementX, const float displacementY,
		const float angle, const float speed);

	void updateMatrix();
};

class Arrow : public SimpleScene
{
public:
	Arrow();
	~Arrow();

	void Init() override;
	void draw(glm::mat3 &matrix);
	glm::vec3 getTip(glm::mat3 &matrix);
};
#pragma once
