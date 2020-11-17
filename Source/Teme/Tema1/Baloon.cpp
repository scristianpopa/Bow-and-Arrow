#include "Baloon.h"

Baloon::Baloon() :
	numTriangles(40),
	radiusX(30.f),
	radiusY(40.f)
{
	Init();
}

Baloon::~Baloon()
{

}

void Baloon::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	createBaloon("red", glm::vec3(1.f, 0.f, 0.f));
	createBrokenBaloon("red", glm::vec3(1.f, 0.f, 0.f));
	
	createBaloon("yellow", glm::vec3(1.f, 1.f, 0.f));
	createBrokenBaloon("yellow", glm::vec3(1.f, 1.f, 0.f));

	createBaloon("blue", glm::vec3(0.f, 0.f, 1.f));
	createBrokenBaloon("blue", glm::vec3(0.f, 0.f, 1.f));

	createBaloon("pink", glm::vec3(1.f, 20.f / 255.f, 147.f / 255.f));
	createBrokenBaloon("pink", glm::vec3(1.f, 20.f / 255.f, 147.f / 255.f));

	// polyline
	{
		Mesh *bString = new Mesh("string");

		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5 , 6, 7 };
		vertices.emplace_back(glm::vec3(0.5f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(-3.f, -13.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(-3.5f, -15.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(-3.5f, -17.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(3.f, -28.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(3.5f, -30.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(3.f, -32.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(-0.5f, -45.f, 0.f), glm::vec3(0.f, 0.f, 0.f));

		bString->InitFromData(vertices, indices);
		bString->SetDrawMode(GL_LINE_STRIP);
		meshes["string"] = bString;
	}
}

void Baloon::createBaloon(const std::string& name, const glm::vec3& color) {
	// ellipse
	{
		Mesh* baloon = new Mesh(name + "Baloon");

		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;

		vertices.emplace_back(glm::vec3(0.f, radiusY, 0.f), color);

		float angle;
		for (int i = 0; i < numTriangles; ++i)
		{
			angle = 2.0f * M_PI * i / numTriangles;
			vertices.emplace_back(
				glm::vec3(cos(angle) * radiusX, (1.f + sin(angle)) * radiusY, 0.f),
				color);
			indices.push_back(i);
		}
		indices.push_back(numTriangles);
		indices.push_back(1);

		baloon->InitFromData(vertices, indices);
		baloon->SetDrawMode(GL_TRIANGLE_FAN);
		meshes[name + "Baloon"] = baloon;
	}

	// trapezoid
	{
		Mesh* knot = new Mesh(name + "Knot");

		std::vector<VertexFormat> vertices;

		vertices.emplace_back(glm::vec3(-1.2f, 0.7f, 0.f), color);
		vertices.emplace_back(glm::vec3(1.2f, 0.7f, 0.f), color);
		vertices.emplace_back(glm::vec3(-4.8f, -3.6f, 0.f), color);
		vertices.emplace_back(glm::vec3(4.8f, -3.6f, 0.f), color);

		std::vector<unsigned short> indices = {
			1, 0, 2,
			3, 1, 2
		};

		knot->InitFromData(vertices, indices);
		knot->SetDrawMode(GL_TRIANGLES);
		meshes[name + "Knot"] = knot;
	}
}

void Baloon::createBrokenBaloon(const std::string& name, const glm::vec3& color) {
	{
		Mesh* baloon = new Mesh(name + "BrokenBaloon");

		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;

		vertices.emplace_back(glm::vec3(0.f, radiusY, 0.f), color);

		// ellipse section
		float angle;
		for (int i = 0; i < numTriangles; ++i)
		{
			angle = -M_PI_2 + M_PI * i / numTriangles;
			vertices.emplace_back(
				glm::vec3(cos(angle) * radiusX / 3, (1.f + sin(angle)) * radiusY, 0.f),
				color);
			indices.push_back(i);
		}

		// circle arc
		for (int i = 0; i < numTriangles; ++i)
		{
			angle = M_PI_2 + 0.75f * M_PI * i / numTriangles;
			vertices.emplace_back(
				glm::vec3(cos(angle) * radiusX / 3, (sin(angle) - 1) * radiusX / 3 + 2.f * radiusY, 0.f),
				color);
			indices.push_back(i + numTriangles);
		}
		float cx = -radiusX * sqrt(2.f) / 6, cy = 1.9f * radiusY - radiusX / 3 + cx;
		vertices.emplace_back(glm::vec3(cx, cy, 0.f), color);

		float newRY = 0.8f * radiusY;
		angle = asin((cy - newRY) / newRY);
		float newRX = cx / cos(angle);
		float increment = (3 * M_PI_2 - angle) / numTriangles;

		// ellipse section
		for (int i = 0; i < numTriangles; ++i)
		{
			angle -= increment;
			vertices.emplace_back(
				glm::vec3(cos(angle) * newRX, (1.f + sin(angle)) * newRY, 0.f),
				color);
			indices.push_back(i + 2 * numTriangles);
		}
		
		indices.push_back(3 * numTriangles);
		indices.push_back(3 * numTriangles + 1);
		indices.push_back(1);

		baloon->InitFromData(vertices, indices);
		baloon->SetDrawMode(GL_TRIANGLE_FAN);
		meshes[name + "BrokenBaloon"] = baloon;
	}

	// smaller trapezoid
	{
		Mesh* knot = new Mesh(name + "BrokenKnot");

		std::vector<VertexFormat> vertices;

		vertices.emplace_back(glm::vec3(-1.6f, 0.7f, 0.f), color);
		vertices.emplace_back(glm::vec3(1.6f, 0.7f, 0.f), color);
		vertices.emplace_back(glm::vec3(-3.5f, -2.5f, 0.f), color);
		vertices.emplace_back(glm::vec3(3.5f, -2.5f, 0.f), color);

		std::vector<unsigned short> indices = {
			1, 0, 2,
			3, 1, 2
		};

		knot->InitFromData(vertices, indices);
		knot->SetDrawMode(GL_TRIANGLES);
		meshes[name + "BrokenKnot"] = knot;
	}
}

void Baloon::draw(glm::mat3 &matrix, const std::string &color, const bool alive, float& scale, const float deltaT = 0.f) {
	if (alive) {
		RenderMesh2D(meshes[color + "Baloon"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes[color + "Knot"], shaders["VertexColor"], matrix);
		RenderMesh2D(meshes["string"], shaders["VertexColor"], matrix);
	} else {
		// when the baloon is hit, first scale it down
		if (scale > 0.6f) {
			matrix *= Transform2D::Scale(scale, scale);
			scale -= deltaT / 1.5f;
			RenderMesh2D(meshes[color + "Baloon"], shaders["VertexColor"], matrix);
			RenderMesh2D(meshes[color + "Knot"], shaders["VertexColor"], matrix);
			RenderMesh2D(meshes["string"], shaders["VertexColor"], matrix);
		} else {
			// after a time it will use the broken mesh
			matrix *= Transform2D::Scale(0.6f, 0.6f);
			RenderMesh2D(meshes[color + "BrokenBaloon"], shaders["VertexColor"], matrix);
			RenderMesh2D(meshes[color + "BrokenKnot"], shaders["VertexColor"], matrix);
			RenderMesh2D(meshes["string"], shaders["VertexColor"], matrix);
		}
	}
}

// check whether the tip of an arrow is inside the ellipse(body) of an intact baloon
// point - circle collision
bool Baloon::hitBy(const glm::mat3 &matrix, const glm::vec3 &arrowTip) {
	glm::vec3 tmp = matrix * glm::vec3(0.f, radiusY, 1.f);
	float cx = (arrowTip[0] - tmp[0]) / radiusX;
	float cy = (arrowTip[1] - tmp[1]) / radiusY;
	if (cx * cx + cy * cy <= 1.f) {
		return true;
	}
	return false;
}

BaloonProperties::BaloonProperties(const float translateX, const float translateY, const std::string& color, const float speed = 10.f)
	: translateX(translateX)
	, translateY(translateY)
	, speed(speed)
	, color(color)
	, angle(0.f)
	, scale(1.f)
{
	updateMatrix();
}

void BaloonProperties::updateMatrix() {
	matrix = Transform2D::Translate(translateX + 70.f * sin(angle), translateY);
}
