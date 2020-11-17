#include "Arrow.h"

Arrow::Arrow()
{
	Init();
}

Arrow::~Arrow()
{

}

void Arrow::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	// rectangle
	{
		Mesh* body = new Mesh("body");

		std::vector<VertexFormat> vertices;

		vertices.emplace_back(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(115.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(115.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));

		std::vector<unsigned short> indices = {
			0, 1, 3,
			3, 1, 2
		};

		body->InitFromData(vertices, indices);
		body->SetDrawMode(GL_TRIANGLES);
		meshes["body"] = body;
	}

	// triangle
	{
		Mesh* tip = new Mesh("tip");

		std::vector<VertexFormat> vertices;

		vertices.emplace_back(glm::vec3(138.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		vertices.emplace_back(glm::vec3(115.f, 3.7f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		vertices.emplace_back(glm::vec3(115.f, -3.7f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		
		std::vector<unsigned short> indices = {
			0, 1, 2
		};

		tip->InitFromData(vertices, indices);
		tip->SetDrawMode(GL_TRIANGLES);
		meshes["tip"] = tip;
	}
}

void Arrow::draw(glm::mat3 &matrix) {
	RenderMesh2D(meshes["tip"], shaders["VertexColor"], matrix);
	RenderMesh2D(meshes["body"], shaders["VertexColor"], matrix);
}


// returns the tip of the arrow in viewport coordinates
glm::vec3 Arrow::getTip(glm::mat3 &matrix) {
	return matrix * glm::vec3(138.f, 0.f, 1.f);
}


ArrowProperties::ArrowProperties(const float translateX, const float translateY, const float displacementX, const float displacementY,
	const float angle = 0.f, const float speed = 10.f)
	: translateX(translateX)
	, translateY(translateY)
	, displacementX(displacementX)
	, displacementY(displacementY)
	, angle(angle)
	, speed(speed)
	, gravityAffect(0.f)
{
	updateMatrix();
}

void ArrowProperties::updateMatrix() {
	matrix = Transform2D::Translate(translateX, translateY) * Transform2D::Rotate(angle) * Transform2D::Translate(displacementX, displacementY);
}
