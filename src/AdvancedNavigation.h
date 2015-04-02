#ifndef _ADV_NAVI_H_
#define _ADV_NAVI_H_

#include "Application.h"
#include "Camera.h"
#include "Vertex.h"
#include <vector>

struct NavMeshNode
{
	vec3 position; //center of the polygon node
	vec3 corners[3];
	NavMeshNode* edges[3];

	//A* data
	unsigned int flags;
	float edge_costs[3];
};


class AdvancedNavigation : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void buildNavMesh();

	OpenGLData m_sponza_mesh;
	std::vector<NavMeshNode> m_nav_nodes;

	FlyCamera* m_camera;
	float m_timer;
	unsigned int m_programID;

};


#endif // !_ADV_NAVI_H_
