#ifndef _COLLISIONS_H_
#define _COLLISIONS_H_

#include "glm_header.h"

#include <vector>
using std::vector;

class AABB 
{
public:
	AABB() { reset(); }
	~AABB() {}
	void reset();
	void fit(const std::vector<glm::vec3>& points);
	void fixCenter(vec3 a_min, vec3 a_max);

	int CheckCollision(vec4 a_plane);

	glm::vec3 m_min, m_max, m_center;
};

class BoundingSphere 
{
public:
	BoundingSphere() : centre(0), radius(0) { }
	~BoundingSphere() {}
	void fit(const std::vector<glm::vec3>& points);

	int CheckCollision(vec4 a_plane);

	glm::vec3 centre;
	float radius;
};



#endif // !_COLLISIONS_H_
