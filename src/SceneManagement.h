#ifndef _SCENE_MANAGEMENTH_H_
#define _SCENE_MANAGEMENTH_H_

#include "Application.h"
#include "Camera.h"

#include "Vertex.h"
#include <vector>
using std::vector;

#include "Collisions.h"

class SceneManagement : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void getFrustumPlanes(const glm::mat4& transform, glm::vec4* planes);

	FlyCamera* m_camera;
	float m_timer;
	unsigned int m_programID;

	BoundingSphere* sphere;
	AABB* cube;
	vec4 plane;
};

#endif // !_SCENE_MANAGEMENTH_H_
