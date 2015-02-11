#ifndef RENDERING_GEO_H_
#define RENDERING_GEO_H_

#include "Application.h"
#include "CameraAndProjections.h"
#include "Vertex.h"

class RenderingGeometry : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	void genernateGrid(unsigned int rows, unsigned int cols);

	void generateShader();
	unsigned int m_programID;
	unsigned int m_index_count;

	FlyCamera *m_camera;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	float m_dt;
	
};


#endif //_RENDERING_GEO_H_