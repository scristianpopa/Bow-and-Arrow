#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include "Transform2D.h"
#include <algorithm>
#include <string>
#include <vector>
class Bow : public SimpleScene
{
public:
	Bow();
	~Bow();

	void Init() override;
	void draw(glm::mat3 &matrixT, glm::mat3 &matrixR);
	void drawSpecial(glm::mat3& matrixT, const float blue, const float maxBlue, const float pink, const float maxPink);
	bool hitBy(glm::mat3 &myMatrix, glm::mat3 &shurikenMatrix, float shurikenRadius);
	void increasePull(float amount);
	void resetPull();
	void createString(const int i);
	void Bow::createPullBar(const std::string& name, const glm::vec3 color);
	int getStringPull();
	float getPullCharge();

protected:
	int numTriangles;
	int pullStages;	// how many stages the bow string can be pulled
	float radius, length; // radius of the circle from which we section off the bow
	// length is the distance from the middle of the string to the tips of the bow 
	float centreX; // x coordinate of the centre of the above mentioned circle, y coordinate is 0
	float pull, maxPull; // how hard we pull on the string and how much it can go
};
#pragma once
