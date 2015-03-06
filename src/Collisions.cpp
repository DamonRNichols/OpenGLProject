#include "Collisions.h"

void AABB::reset()
{
	vec3 min(1e37f), max(-1e37f); //defining the largest min and max space
}

void AABB::fit(const std::vector<glm::vec3>& points) 
{
	for (auto& p : points) 
	{
		if (p.x < m_min.x) m_min.x = p.x;
		if (p.y < m_min.y) m_min.y = p.y;
		if (p.z < m_min.z) m_min.z = p.z;
		if (p.x > m_max.x) m_max.x = p.x;
		if (p.y > m_max.y) m_max.y = p.y;
		if (p.z > m_max.z) m_max.z = p.z;
	}
}

int AABB::CheckCollision(vec4 a_plane)
{
	vec3 v1, v2;

	vec4 plane = a_plane;

	glm::normalize(plane);

	for (int i = 0; i < 3; ++i)
	{
		if (plane[i] >= 0)
		{
			v1[i] = m_min[i];
			v2[i] = m_max[i];
		}
		else
		{
			v1[i] = m_max[i];
			v2[i] = m_min[i];
		}
	}
	// Get the Extense vector
	vec3 E = (m_max - m_min) / 2.0f;

	// Get the center of the Box
	vec3 center = m_min + E;
	
	vec4 N = plane;
	glm::normalize(N);
	
	float fRadius = abs(plane.x*E.x) + abs(plane.y*E.y) + abs(plane.z*E.z);

	BoundingSphere sphere;
	sphere.centre = center;
	sphere.radius = fRadius;

	return sphere.CheckCollision(plane);
}

void AABB::fixCenter(vec3 a_min, vec3 a_max)
{
	m_center = vec3((a_min.x + a_max.x) / 2, (a_min.y + a_max.y) / 2, (a_min.z + a_max.z) / 2);
}

void BoundingSphere::fit(const std::vector<glm::vec3>& points) {
	glm::vec3 min(1e37f), max(-1e37f); //defining the largest min and max space
	for (auto& p : points) { //shrinking the sphere to fit the object best
		if (p.x < min.x) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;
		if (p.x > max.x) max.x = p.x;
		if (p.y > max.y) max.y = p.y;
		if (p.z > max.z) max.z = p.z;
	}
	centre = (min + max) * 0.5f;
	radius = glm::distance(min, centre);
}

int BoundingSphere::CheckCollision(vec4 a_plane)
{
	float d = glm::dot(vec3(a_plane), this->centre) + a_plane.w;

	if (d > this->radius)
	{
		//printf("Front\n");
		return 1;
	}
	else if (d < -this->radius)
	{
		//printf("Back\n");
		return -1;
	}
	else
	{
		//printf("On the plane\n");
		return 0;
	}
}