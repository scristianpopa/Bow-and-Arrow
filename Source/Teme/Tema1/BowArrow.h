#pragma once

#include <Component/SimpleScene.h>
#include "../../TextRenderer/TextRenderer.h"
#include "Transform2D.h"
#include "Heart.h"
#include "Baloon.h"
#include "Shuriken.h"
#include "Arrow.h"
#include "Bow.h"
#include <unordered_set>
#include <algorithm>
#include <Core/Engine.h>

#define FREEZE 3.f
#define NOGRAVITY 7.f

class BowArrow : public SimpleScene
{
public:
	BowArrow();
	~BowArrow();

	void Init() override;

private:
	void drawScoreLegend(const glm::ivec2 &resolution);
	void drawEndScreen(const glm::ivec2& resolution, const float deltaTimeSeconds);
	void spawn(const float deltaTimeSeconds);
	void drawLives();
	void drawBow(const float deltaTimeSeconds);
	void drawArrows(const float deltaTimeSeconds);
	void drawBaloons(const float deltaTimeSeconds);
	void drawShurikens(const float deltaTimeSeconds);
	void drawBuffs();

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	Baloon* baloon;
	Shuriken* shuriken;
	Arrow* arrow;
	Bow* bow;
	Heart* heart;
	TextRenderer* Text;
	int score;
	int lives;
	int speedUpStep; // speed up the game when the score increments by speedUpStep;
	float speedUp;
	float gravity; // how strongly arrows and broken ballons are attracted to the bottom of the viewport
	float noGBoost; // timer for the gravity disabling buff
	float timeSlow; // timer for the time slow buff
	float hideArrow; // timer after an arrow is launched to know when we can launch again
	float bowHeight, bowAngle; 
	float shurikenTimer, baloonTimer; // timers for object generation
	float endR, endG, endB; // color values for the end game screen
	float endColorTimer; // timer for changing said colors
	// vectors with properties of objects
	std::vector<ShurikenProperties> shurikens;
	std::vector<BaloonProperties> baloons;
	std::vector<ArrowProperties> arrows;
};
#pragma once
