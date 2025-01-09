/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include <glm/glm.hpp>

namespace bubble
{
	inline static float distancia3(const glm::vec3& a, const glm::vec3& b)
	{
		float dx = b.x - a.x;
		float dy = b.y - a.y;
		float dz = b.z - a.z;

		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}
	inline static float distancia2(const bubble::vetor2<double>& a, const bubble::vetor2<double>& b)
	{
		float dx = b.x - a.x;
		float dy = b.y - a.y;

		return std::sqrt(dx * dx + dy * dy );
	}
	inline static glm::vec3 telaParaMundo(bubble::vetor2<double> v, float zDepth, bubble::camera& cam)
	{
		float ndcX = (2.f * v.x) / cam.viewport_ptr->w - 1.f;
		float ndcY = 1.f - (2.f * v.y) / cam.viewport_ptr->h;
		float ndcZ = 2.f * zDepth - 1.f;
		
		glm::vec4 ndcCoords(ndcX, ndcY, ndcZ, 1.f);
		glm::mat4 inverseVP = glm::inverse(cam.obtProjectionMatrix() * cam.obtViewMatrix());
		glm::vec4 worldCoords = inverseVP * ndcCoords;
		worldCoords /= worldCoords.w;

		return glm::vec3(worldCoords);
	}
}
inline glm::vec3 operator*(const glm::vec3& lhs, const glm::vec3& rhs)
{
	return glm::vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}