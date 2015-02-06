#ifndef _CAM_PROJ_H_
#define _CAM_PROJ_H_

#include "Application.h"
#include "Camera.h"

class CameraAndProjections : public Application
{
public:
	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

	FlyCamera *cam;

};


#endif // !_CAM_PROJ_H_
