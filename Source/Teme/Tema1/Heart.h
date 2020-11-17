#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include "Transform2D.h"
#include <vector>

class Heart : public SimpleScene
{
public:
	Heart();
	~Heart();

	void Init() override;
	void draw(glm::mat3& matrix);
};
#pragma once
