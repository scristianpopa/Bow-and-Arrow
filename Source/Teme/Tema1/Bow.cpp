#include "Bow.h"

Bow::Bow() :
	numTriangles(30),
	radius(200.f),
	length(80.f),
	pull(0.f),
	maxPull(80.f),
	pullStages(10)
{
	Init();
}

Bow::~Bow()
{

}

void Bow::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	float tipsRadius = 5.f;
	centreX = -sqrt(radius * radius - length * length);
	{
		Mesh* bow = new Mesh("bow");

		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;

		float angle = M_PI_2, increment = M_PI / numTriangles;
		// semicircle for the bow tips
		for (int i = 0; i < numTriangles; ++i)
		{
			angle += increment;

			vertices.emplace_back(
				glm::vec3(cos(angle) * tipsRadius / 2, (1 + sin(angle)) * tipsRadius + length, 0.f),
				glm::vec3(0.f, 0.f, 0.f));
			indices.push_back(i);
		}

		vertices.emplace_back(glm::vec3(0.f, length, 0.f), glm::vec3(0.f, 0.f, 0.f));
		indices.push_back(numTriangles);

		angle = glm::atan(length / radius);
		increment = 2 * angle / numTriangles;
		
		// circle section
		for (int i = 0; i < numTriangles; ++i)
		{
			angle -= increment;

			vertices.emplace_back(
				glm::vec3(cos(angle) * radius + centreX, sin(angle) * radius, 0.f),
				glm::vec3(0.f, 0.f, 0.f));
			indices.push_back(i + numTriangles + 1);
		}

		vertices.emplace_back(glm::vec3(0.f, -length, 0.f), glm::vec3(0.f, 0.f, 0.f));
		indices.push_back(2 * numTriangles + 1);

		angle = 3 * M_PI_2;
		increment = M_PI / numTriangles;

		// other tip
		for (int i = 0; i < numTriangles; ++i)
		{
			angle -= increment;

			vertices.emplace_back(
				glm::vec3(cos(angle) * tipsRadius / 2, (1 + sin(angle)) * -tipsRadius - length, 0.f),
				glm::vec3(0.f, 0.f, 0.f));
			indices.push_back(i + 2 * numTriangles + 2);
		}
		
		bow->InitFromData(vertices, indices);
		bow->SetDrawMode(GL_LINE_STRIP);
		meshes["bow"] = bow;
	}

	// untouched string
	{
		Mesh* bowString = new Mesh("bowString0");

		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices = { 0, 1 };

		vertices.emplace_back(glm::vec3(0.f, -length, 0.f), glm::vec3(1.f, 1.f, 1.f));
		vertices.emplace_back(glm::vec3(0.f, length, 0.f), glm::vec3(1.f, 1.f, 1.f));

		bowString->InitFromData(vertices, indices);
		bowString->SetDrawMode(GL_LINE_STRIP);
		meshes["bowString0"] = bowString;
	}

	for (int i = 1; i <= pullStages; ++i) {
		createString(i);
	}

	// different pull bars, rectangles which scale
	
	createPullBar("", glm::vec3(0.f, 0.f, 0.f));
	createPullBar("Blue", glm::vec3(0.f, 0.f, 1.f));
	createPullBar("Pink", glm::vec3(1.f, 20.f / 255.f, 147.f / 255.f));

}

// creates all the pull stages of the string
void Bow::createString(const int i) {
	Mesh* bowString = new Mesh("bowString" + std::to_string(i));

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices = { 0, 1, 2 };

	vertices.emplace_back(glm::vec3(0.f, -length, 0.f), glm::vec3(1.f, 1.f, 1.f));
	vertices.emplace_back(glm::vec3(-maxPull * i / pullStages, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	vertices.emplace_back(glm::vec3(0.f, length, 0.f), glm::vec3(1.f, 1.f, 1.f));

	bowString->InitFromData(vertices, indices);
	bowString->SetDrawMode(GL_LINE_STRIP);
	meshes["bowString" + std::to_string(i)] = bowString;
}

// creates load bars of different colors
void Bow::createPullBar(const std::string& name, const glm::vec3 color) {
	{
		Mesh* pullBar = new Mesh("pullBar" + name);

		std::vector<VertexFormat> vertices;

		vertices.emplace_back(glm::vec3(0.f, -length - 20.f, 0.f), color);
		vertices.emplace_back(glm::vec3(0.f, -length - 35.f, 0.f), color);
		vertices.emplace_back(glm::vec3(80.f, -length - 35.f, 0.f), color);
		vertices.emplace_back(glm::vec3(80.f, -length - 20.f, 0.f), color);

		std::vector<unsigned short> indices = {
			0, 1, 2,
			2, 3, 0
		};
		pullBar->InitFromData(vertices, indices);
		pullBar->SetDrawMode(GL_TRIANGLES);
		meshes["pullBar" + name] = pullBar;
	}

	{
		Mesh* barMargin = new Mesh("barMargin" + name);

		std::vector<VertexFormat> vertices;

		vertices.emplace_back(glm::vec3(0.f, -length - 20.f, 0.f), color);
		vertices.emplace_back(glm::vec3(0.f, -length - 35.f, 0.f), color);
		vertices.emplace_back(glm::vec3(80.f, -length - 35.f, 0.f), color);
		vertices.emplace_back(glm::vec3(80.f, -length - 20.f, 0.f), color);

		std::vector<unsigned short> indices = { 0, 1, 2, 3 };
		barMargin->InitFromData(vertices, indices);
		barMargin->SetDrawMode(GL_LINE_LOOP);
		meshes["barMargin" + name] = barMargin;
	}
}

void Bow::draw(glm::mat3 &matrixT, glm::mat3 &matrixR) {
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], matrixT * matrixR);
	RenderMesh2D(meshes["barMargin"], shaders["VertexColor"], Transform2D::Translate(-40.f, 0.f) * matrixT);
	if (pull)
		RenderMesh2D(meshes["pullBar"], shaders["VertexColor"],
			Transform2D::Translate(-40.f, 0.f) * matrixT * Transform2D::Scale(pull / maxPull, 1.f));

	RenderMesh2D(meshes["bowString" + std::to_string((int)(pull * pullStages / maxPull))], shaders["VertexColor"], matrixT * matrixR);
}

// draws the special bars in case of a power up
void Bow::drawSpecial(glm::mat3& matrixT, const float blue, const float maxBlue, const float pink, const float maxPink) {
	int haveB = 0;
	if (blue) {
		RenderMesh2D(meshes["pullBarBlue"], shaders["VertexColor"],
			Transform2D::Translate(-40.f, -30.f) * matrixT * Transform2D::Scale(blue / maxBlue, 1.f));
		RenderMesh2D(meshes["barMarginBlue"], shaders["VertexColor"], Transform2D::Translate(-40.f, -30.f) * matrixT);
		haveB = -1;
	}
	if (pink) {
		RenderMesh2D(meshes["pullBarPink"], shaders["VertexColor"],
			Transform2D::Translate(-40.f, -30.f + 30.f * haveB) * matrixT * Transform2D::Scale(pink / maxPink, 1.f));
		RenderMesh2D(meshes["barMarginPink"], shaders["VertexColor"], Transform2D::Translate(-40.f, -30.f + 30.f * haveB) * matrixT);
	}
}

// checks collision with the shuriken
// circle - circle collision
bool Bow::hitBy(glm::mat3 &myMatrix, glm::mat3 &shurikenMatrix, float shurikenRadius) {
	glm::vec3 shurikenCentre = shurikenMatrix * glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 lowerTip = myMatrix * glm::vec3(0.f, -length, 1.f);
	if (shurikenCentre[1] + shurikenRadius < lowerTip[1])
		return false;
	glm::vec3 upperTip = myMatrix * glm::vec3(0.f, length, 1.f);
	if (shurikenCentre[1] - shurikenRadius > upperTip[1])
		return false;
	if (shurikenCentre[1] < lowerTip[1] && shurikenCentre[0] + shurikenRadius < lowerTip[0])
		return false;
	if (shurikenCentre[1] > upperTip[1] && shurikenCentre[0] + shurikenRadius < upperTip[0])
		return false;

	glm::vec3 bowCentre = myMatrix * glm::vec3(-radius, 0.f, 1.f);
	float dx = shurikenCentre[0] - bowCentre[0];
	float dy = shurikenCentre[1] - bowCentre[1];
	float d = sqrt(dx * dx + dy * dy);
	if (d <= radius + shurikenRadius)
		return true;
	return false;
}

void Bow::increasePull(float amount) {
	pull = std::min(pull + amount, maxPull);
}

void Bow::resetPull() {
	pull = 0.f;
}


float Bow::getPullCharge() {
	return pull / maxPull;
}

// returns the pull stage so we know how much to stretch the string
int Bow::getStringPull() {
	return (int)(pull * pullStages / maxPull) * maxPull / pullStages;
}
