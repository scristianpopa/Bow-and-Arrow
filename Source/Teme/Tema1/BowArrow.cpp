#include "BowArrow.h"

#include <Core/Engine.h>
#include "Transform2D.h"
#include <iostream>
using namespace std;

BowArrow::BowArrow() :
	score(0),
	lives(3),
	bowHeight(window->GetResolution().y / 2),
	bowAngle(0.f),
	hideArrow(0.f),
	gravity(3.5f),
	baloonTimer(0.f),
	shurikenTimer(5.f),
	endColorTimer(0.f),
	noGBoost(0.f),
	timeSlow(0.f),
	speedUp(1.f),
	speedUpStep(5)
{
}

BowArrow::~BowArrow()
{
	delete baloon;
	delete shuriken;
	delete arrow;
	delete bow;
	delete heart;
	delete Text;
}

void BowArrow::Init()
{
	srand((unsigned int)time(NULL));

	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	baloon = new Baloon();
	shuriken = new Shuriken();
	arrow = new Arrow();
	bow = new Bow();
	heart = new Heart();
	Text = new TextRenderer(resolution.x, resolution.y);
	Text->Load("Source/TextRenderer/Fonts/Arial.ttf", 18);
}

// displays the legend of baloons and the score
void BowArrow::drawScoreLegend(const glm::ivec2 &resolution)
{
	Text->RenderText("Score: " + to_string(score), 40.0f, resolution.y - 40.0f, 1.7f, glm::vec3(1.f, 0.3f, 1.f));
	Text->RenderText("       GAME RULES        ", 20.f, 15.f, 1.f, glm::vec3(0.1f, 0.1f, 0.1f));
	Text->RenderText("Red baloon       +1 score", 20.f, 35.f, 1.f, glm::vec3(0.2f, 0.2f, 0.2f));
	Text->RenderText("Yellow baloon    -1 score", 20.f, 55.f, 1.f, glm::vec3(0.2f, 0.2f, 0.2f));
	Text->RenderText("Blue baloon      no arrow gravity", 20.f, 75.f, 1.f, glm::vec3(0.2f, 0.2f, 0.2f));
	Text->RenderText("Pink baloon      slow time", 20.f, 95.f, 1.f, glm::vec3(0.2f, 0.2f, 0.2f));
	Text->RenderText("Shuriken         boo-boo", 20.f, 115.f, 1.f, glm::vec3(0.2f, 0.2f, 0.2f));
}

void BowArrow::drawEndScreen(const glm::ivec2& resolution, const float deltaTimeSeconds)
{
	if (endColorTimer < 0) {
		endR = static_cast<float> (rand()) / static_cast <float> (RAND_MAX);
		endG = static_cast<float> (rand()) / static_cast <float> (RAND_MAX);
		endB = static_cast<float> (rand()) / static_cast <float> (RAND_MAX);
		endColorTimer = 1.f;
	}
	endColorTimer -= deltaTimeSeconds;
	Text->RenderText("GAME OVER", resolution.x / 2 - 165.f, resolution.y / 2 - 45.f, 3.f, glm::vec3(endR, endG, endB));
	if (score < 5) {
		Text->RenderText("UR A NOOB", resolution.x / 2 - 100.f, resolution.y / 2 + 30.f, 2.f, glm::vec3(endR, endG, endB));
	}
	else {
		if (score < 15) {
			Text->RenderText("ACCEPTABLE", resolution.x / 2 - 127.f, resolution.y / 2 + 30.f, 2.1f, glm::vec3(endR, endG, endB));
		}
		else {
			Text->RenderText("UR A PRO", resolution.x / 2 - 95.f, resolution.y / 2 + 30.f, 2.3f, glm::vec3(endR, endG, endB));
		}
	}
}

// generates new objects at random locations
void BowArrow::spawn(const float deltaTimeSeconds) {
	glm::ivec2 resolution = window->GetResolution();
	baloonTimer -= deltaTimeSeconds;
	if (baloonTimer <= 0 && lives) {
		int tmp = rand() % 1000;
		if (tmp > 900) {
			baloons.emplace_back(500 + rand() % ((int)resolution.x - 650), -60, "pink", 110.f);
		}
		else {
			if (tmp > 750) {
				baloons.emplace_back(500 + rand() % ((int)resolution.x - 650), -60, "blue", 80.f);
			}
			else {
				if(tmp > 500) {
					baloons.emplace_back(500 + rand() % ((int)resolution.x - 650), -60, "yellow", 50.f);
				}
				else {
					baloons.emplace_back(500 + rand() % ((int)resolution.x - 650), -60, "red", 60.f);
				}
			}
		}
		baloonTimer = rand() % 3 + 0.5f;
	}

	shurikenTimer -= deltaTimeSeconds;
	if (shurikenTimer <= 0 && lives) {
		auto tmp = 0.75f + 0.5f * static_cast<float> (rand()) / static_cast <float> (RAND_MAX);
		shurikens.emplace_back(resolution.x + 50, 130 + rand() % ((int)resolution.y - 305), 0.f, 150.f * (2 - tmp), 1.3f * tmp);
		shurikenTimer = std::max(1.5f, rand() % 6 + 4 - 2 * speedUp);
	}
}

void BowArrow::drawLives() {
	for (int i = 0; i < lives; ++i) {
		heart->draw(Transform2D::Translate(60 + i * 45, 65));
	}
}

void BowArrow::drawBow(const float deltaTimeSeconds) {
	bow->draw(Transform2D::Translate(100, bowHeight), Transform2D::Rotate(bowAngle));
	// you shouldn't hide the new arrow, so display it at the middle of the string
	if (!hideArrow) {
		glm::mat3 modelMatrix = Transform2D::Translate(100, bowHeight);
		modelMatrix *= Transform2D::Rotate(bowAngle);
		modelMatrix *= Transform2D::Translate(-bow->getStringPull(), 0);
		arrow->draw(modelMatrix);
	}
	else {
		if (lives) {
			hideArrow = std::max(hideArrow - deltaTimeSeconds, 0.f);
		}
	}
}

void BowArrow::drawArrows(const float deltaTimeSeconds) {
	glm::ivec2 resolution = window->GetResolution();
	auto prop = arrows.begin();
	while (prop != arrows.end()) {
		arrow->draw(prop->matrix);
		if (lives) {
			prop->translateX += deltaTimeSeconds * prop->speed * cos(prop->angle);
			prop->translateY += deltaTimeSeconds * prop->speed * sin(prop->angle) - deltaTimeSeconds * gravity;
			// if the no gravity buff is active, don't change the angle of the arrows
			if (!noGBoost) {
				prop->angle = std::max(prop->angle - deltaTimeSeconds * prop->gravityAffect / 300, (float)-M_PI_2);
				prop->gravityAffect += gravity;
			}
			prop->updateMatrix();
			glm::vec3 pos = prop->matrix * glm::vec3(0.f, 0.f, 1.f);
			// erase arrows if out of viewport
			if (pos[0] > resolution.x || pos[0] < 0 || pos[1] > resolution.y || pos[1] < 0) {
				prop = arrows.erase(prop);
			}
			else {
				++prop;
			}
		}
		else {
			// if we're out of lives, still display the last scene
			++prop;
		}
	}
}

void BowArrow::drawBaloons(const float deltaTimeSeconds) {
	glm::ivec2 resolution = window->GetResolution();
	auto prop = baloons.begin();
	while (prop != baloons.end()) {
		baloon->draw(prop->matrix, prop->color, prop->alive, prop->scale, deltaTimeSeconds);
		if (lives) {
			// baloon goes up when all good and healthy
			if (prop->alive) {
				// different modifiers when the time slow buff is active
				if (!timeSlow) {
					prop->translateY += deltaTimeSeconds * prop->speed * speedUp;
					prop->angle += deltaTimeSeconds * speedUp;
				}
				else {
					prop->translateY += deltaTimeSeconds * prop->speed * speedUp * 0.3f;
					prop->angle += deltaTimeSeconds * speedUp * 0.3f;
				}
			}
			else {
				// baloon goes down when broken and sad
				if (!timeSlow) {
					prop->translateY -= deltaTimeSeconds * prop->speed * speedUp;
					prop->speed += gravity;
				}
				else {
					prop->translateY -= deltaTimeSeconds * prop->speed * speedUp * 0.3f;
					prop->speed += gravity * 0.3f;
				}
			}
			prop->updateMatrix();
			if (prop->alive) {
				for (auto& arrowProp : arrows) {
					// check for arrow collision
					if (baloon->hitBy(prop->matrix, arrow->getTip(arrowProp.matrix))) {
						prop->alive = false;
						if (prop->color == "red") {
							++score;
						}
						else {
							if (prop->color == "blue") {
								noGBoost = NOGRAVITY;
							}
							else {
								if (prop->color == "yellow") {
									--score;
								}
								else {
									timeSlow = FREEZE;
								}
							}
						}
						break;
					}
				}
			}
			glm::vec3 pos = prop->matrix * glm::vec3(0.f, 0.f, 1.f);
			// erase the baloon when out of viewport
			if ((prop->alive && pos[1] > resolution.y) || (pos[1] < 0 && !prop->alive)) {
				prop = baloons.erase(prop);
			}
			else {
				++prop;
			}
		}
		else {
			// if we're out of lives, still display the last scene
			++prop;
		}
	}
}

void BowArrow::drawShurikens(const float deltaTimeSeconds) {
	glm::ivec2 resolution = window->GetResolution();
	auto bowMatrix = Transform2D::Translate(100, bowHeight) * Transform2D::Rotate(bowAngle);
	auto prop = shurikens.begin();
	while (prop != shurikens.end()) {
		shuriken->draw(prop->matrix, prop->alive);
		if (lives) {
			// different animations when the shuriken is intact and when it was hit by an arrow
			if (prop->alive) {
				// different modifiers when the time slow buff is active
				if (!timeSlow) {
					prop->translateX -= deltaTimeSeconds * prop->speed * speedUp;
					prop->angle += deltaTimeSeconds * 4.f * speedUp;
				}
				else {
					prop->translateX -= deltaTimeSeconds * prop->speed * speedUp * 0.3f;
					prop->angle += deltaTimeSeconds * 4.f * speedUp * 0.3f;
				}
			}
			else {
				if (!timeSlow) {
					prop->translateX -= deltaTimeSeconds * prop->speed * speedUp;
					prop->translateY -= deltaTimeSeconds * prop->speed * speedUp * 4;
					prop->angle += deltaTimeSeconds * 10.f * speedUp;
				}
				else {
					prop->translateX -= deltaTimeSeconds * prop->speed * speedUp * 0.3f;
					prop->translateY -= deltaTimeSeconds * prop->speed * speedUp * 4 * 0.3f;
					prop->angle += deltaTimeSeconds * 10.f * speedUp * 0.3f;
				}
			}
			prop->updateMatrix();
			glm::vec3 pos = prop->matrix * glm::vec3(0.f, 0.f, 1.f);

			if (prop->alive) {
				for (auto& arrowProp : arrows) {
					// check for collisions with every arrow
					if (shuriken->hitBy(prop->matrix, arrow->getTip(arrowProp.matrix))) {
						prop->alive = false;
						break;
					}
				}
			}
			
			// decrement the number of lives if the shuriken hits the bow
			bool erased = false;
			if (prop->alive && bow->hitBy(bowMatrix, prop->matrix, shuriken->getHitboxRadius() * prop->scale)) {
				--lives;
				erased = true;
			}

			// erase the shuriken when out of viewport
			if (pos[0] < 0 || pos[1] < 0 || erased) {
				prop = shurikens.erase(prop);
			}
			else {
				++prop;
			}
		}
		else {
			// if we're out of lives, still display the last scene
			++prop;
		}
	}
}

void BowArrow::drawBuffs() {
	bow->drawSpecial(Transform2D::Translate(100, bowHeight), noGBoost, NOGRAVITY, timeSlow, FREEZE);
}

void BowArrow::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.f, 0.5f, 0.f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void BowArrow::Update(float deltaTimeSeconds)
{	
	spawn(deltaTimeSeconds);

	drawBow(deltaTimeSeconds);
	
	drawArrows(deltaTimeSeconds);
	
	drawBaloons(deltaTimeSeconds);
	
	drawShurikens(deltaTimeSeconds);

	drawBuffs();

	noGBoost = std::max(noGBoost - deltaTimeSeconds, 0.f);
	timeSlow = std::max(timeSlow - deltaTimeSeconds, 0.f);

	if (score >= speedUpStep) {
		speedUpStep += 5;
		speedUp += 0.5f;
	}

	drawLives();

	glm::ivec2 resolution = window->GetResolution();
	drawScoreLegend(resolution);
	if (!lives) {
		drawEndScreen(resolution, deltaTimeSeconds);
	}
}

void BowArrow::FrameEnd()
{
}

void BowArrow::OnInputUpdate(float deltaTime, int mods)
{
	if (lives) {
		glm::ivec2 resolution = window->GetResolution();
		if (window->KeyHold(GLFW_KEY_W)) {
			bowHeight += deltaTime * 111;
			bowHeight = std::min(bowHeight, resolution.y - 220.f);
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			bowHeight -= deltaTime * 111;
			bowHeight = std::max(bowHeight, 270.f);
		}
		if ((window->KeyHold(GLFW_KEY_SPACE) || window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) && !hideArrow) {
			bow->increasePull(deltaTime * 55);
		}
	}
}

void BowArrow::OnKeyPress(int key, int mods)
{
	// add key press event
}

void BowArrow::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_SPACE && !hideArrow && lives) {
		arrows.emplace_back(100, bowHeight, -bow->getStringPull(), 0, bowAngle, bow->getPullCharge() * 700 + 250);
		bow->resetPull();
		hideArrow = 1.2f;
	}
}

void BowArrow::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (lives) {
		mouseY = window->GetResolution().y - mouseY;
		glm::vec3 stringPoint(0.f, 0.f, 1.f);
		stringPoint = Transform2D::Translate(100, bowHeight) * Transform2D::Rotate(bowAngle) * stringPoint;

		float pi2 = M_PI_2;
		// move the bow only to a certain degree
		// always faces to the right
		if (mouseX < stringPoint[0] + 0.1f) {
			if (mouseY > stringPoint[1]) {
				bowAngle = 0.7f * M_PI_2;
			}
			else {
				bowAngle = -0.7f * M_PI_2;
			}
		}
		else {
			bowAngle = glm::atan((mouseY - stringPoint[1]) / (mouseX - stringPoint[0]));
			if (bowAngle > 0) {
				bowAngle = std::min(bowAngle, 0.7f * pi2);
			}
			else {
				bowAngle = std::max(bowAngle, -0.7f * pi2);
			}
		}
	}
}

void BowArrow::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void BowArrow::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
		if (button == 1 && !hideArrow && lives) {
			arrows.emplace_back(100, bowHeight, -bow->getStringPull(), 0, bowAngle, bow->getPullCharge() * 700 + 250);
			bow->resetPull();
			hideArrow = 1.2f;
		}
}

void BowArrow::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void BowArrow::OnWindowResize(int width, int height)
{
}
