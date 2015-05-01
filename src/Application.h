#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "glm_header.h"

#include "AntTweakBar.h"
#include "Utility.h" //includes Vertex.h
#include "Gizmos.h"

class CameraAndProjections;

struct GLFWwindow;

class Application
{
public:
	Application();
	virtual ~Application();

	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();

	GLFWwindow* m_window;



};


#endif // !_APPLICATION_H_