#include "Shuriken.h"

Shuriken::Shuriken() :
	size(25.f)
{
	Init();
}

Shuriken::~Shuriken()
{
}

void Shuriken::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	{
		Mesh* triangle = new Mesh("triangle");

		std::vector<VertexFormat> vertices;

		vertices.emplace_back(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(-size, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(-size, size, 0.f), glm::vec3(0.f, 0.f, 0.f));

		std::vector<unsigned short> indices = {
			0, 1, 2,
		};

		triangle->InitFromData(vertices, indices);
		triangle->SetDrawMode(GL_TRIANGLES);
		meshes["triangle"] = triangle;
	}

	// edges of a shuriken should be SHINY 
	{
		Mesh* shiny = new Mesh("shiny");

		std::vector<VertexFormat> vertices;

		vertices.emplace_back(glm::vec3(0.f, -0.1f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		vertices.emplace_back(glm::vec3(-0.1f - size, -0.1f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		vertices.emplace_back(glm::vec3(-0.1f - size, 0.1f + size, 0.f), glm::vec3(1.f, 1.f, 1.f));

		std::vector<unsigned short> indices = {
			0, 1, 2,
		};

		shiny->InitFromData(vertices, indices);
		shiny->SetDrawMode(GL_LINE_STRIP);
		meshes["edge"] = shiny;
	}
}

void Shuriken::setSize(const float s) {
	size = s;
}

void Shuriken::draw(glm::mat3 &matrix, const bool alive) {
	if (alive) {
		// just rotate the triangle 3 times
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes["edge"], shaders["VertexColor"], matrix);

		matrix *= Transform2D::Rotate(M_PI_2);
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes["edge"], shaders["VertexColor"], matrix);

		matrix *= Transform2D::Rotate(M_PI_2);
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes["edge"], shaders["VertexColor"], matrix);

		matrix *= Transform2D::Rotate(M_PI_2);
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes["edge"], shaders["VertexColor"], matrix);
	}
	else {
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes["edge"], shaders["VertexColor"], matrix);
	
		matrix *= Transform2D::Rotate(M_PI_2 * 0.4f);
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes["edge"], shaders["VertexColor"], matrix);

		matrix *= Transform2D::Rotate(M_PI_2 * 1.85f);
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes["edge"], shaders["VertexColor"], matrix);

		matrix *= Transform2D::Rotate(M_PI_2 * 0.9f);
		RenderMesh2D(meshes["triangle"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes["edge"], shaders["VertexColor"], matrix);
	}
}

// checks to see if the shuriken is destroyed by an arrow
// point - circle collision
bool Shuriken::hitBy(const glm::mat3& matrix, const glm::vec3& arrowTip) {
	glm::vec3 tmp = matrix * glm::vec3(0.f, 0.f, 1.f);
	float cx = (arrowTip[0] - tmp[0]) / getHitboxRadius();
	float cy = (arrowTip[1] - tmp[1]) / getHitboxRadius();
	if (cx * cx + cy * cy <= 1.f) {
		return true;
	}
	return false;
}

float Shuriken::getHitboxRadius() {
	return sqrt(2.f) * size;
}

ShurikenProperties::ShurikenProperties(const float translateX, const float translateY, const float angle, const float speed, const float scale)
	: translateX(translateX)
	, translateY(translateY)
	, angle(angle)
	, speed(speed)
	, scale(scale)
	, alive(true)
{
	updateMatrix();
}

void ShurikenProperties::updateMatrix() {
	matrix = Transform2D::Translate(translateX, translateY) * Transform2D::Rotate(angle) * Transform2D::Scale(scale, scale);
}
