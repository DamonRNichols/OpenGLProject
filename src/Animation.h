#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "Application.h"
#include "Camera.h"
#include "FBXFile.h"
#include "Vertex.h"

class Animation : public Application
{
public:
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();

	void generateGLMeshes(FBXFile* fbx);

	void EvaluateSkeleton(FBXAnimation* anim, FBXSkeleton* skeleton, float timer);
	void UpdateBones(FBXSkeleton* skeleton);

	unsigned int m_programID;

	std::vector<OpenGLData> m_meshes;
	FBXFile* m_file;

	FlyCamera *m_camera;
	float m_timer;
};


#endif // !_ANIMATION_H_