#include "Heart.h"

Heart::Heart()
{
	Init();
}

Heart::~Heart()
{

}

void Heart::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	{
		Mesh* heart = new Mesh("heart");

		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;
		
		vertices.emplace_back(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
		indices.push_back(0);

		// solutions to eq (x ^ 2 + y ^ 2 - 1) ^ 3 - x ^ 2 * y ^ 3 = 0
		for (float x = -1.139; x <= 1.139; x += 0.002)
		{
			float cubeRoot = cbrt(x * x);
			float delta = cubeRoot * cubeRoot - 4 * x * x + 4;
			float y = (cubeRoot + sqrt(delta)) / 2;
			vertices.emplace_back(glm::vec3(15.f * x, 15.f * y, 0.f), glm::vec3(1.f, 0.f, 0.f));
			indices.push_back(indices.back() + 1);
		}

		for (float x = 1.139; x >= -1.139; x -= 0.002)
		{
			float cubeRoot = cbrt(x * x);
			float delta = cubeRoot * cubeRoot - 4 * x * x + 4;
			float y = (cubeRoot - sqrt(delta)) / 2;
			vertices.emplace_back(glm::vec3(15.f * x, 15.f * y, 0.f), glm::vec3(1.f, 0.f, 0.f));
			indices.push_back(indices.back() + 1);
		}

		indices.push_back(1);

		heart->InitFromData(vertices, indices);
		heart->SetDrawMode(GL_TRIANGLE_FAN);
		meshes["heart"] = heart;
	}
}

void Heart::draw(glm::mat3& matrix) {
	RenderMesh2D(meshes["heart"], shaders["VertexColor"], matrix);
}
