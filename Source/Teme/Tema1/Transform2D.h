#pragma once

#include <include/glm.h>

namespace Transform2D
{
	// Translate matrix
	inline glm::mat3 Translate(float translateX, float translateY)
	{
		// TODO implement translate matrix
		return glm::transpose(
			glm::mat3(1.f, 0.f, translateX,
					  0.f, 1.f, translateY,
					  0.f, 0.f, 1.f)
		);
	}

	// Scale matrix
	inline glm::mat3 Scale(float scaleX, float scaleY)
	{
		// TODO implement scale matrix
		return glm::transpose(
			glm::mat3(scaleX, 0.f, 0.f,
						0.f, scaleY, 0.f,
						0.f, 0.f, 1.f)
		);
	}

	// Rotate matrix
	inline glm::mat3 Rotate(float radians)
	{
		// TODO implement rotate matrix
		return glm::transpose(
			glm::mat3(cos(radians), -sin(radians), 0.f,
						sin(radians), cos(radians), 0.f,
						0.f, 0.f, 1.f)
		);
	}
}
