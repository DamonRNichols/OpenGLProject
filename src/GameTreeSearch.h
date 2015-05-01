#ifndef _GAME_TREE_SEARCH_H_
#define _GAME_TREE_SEARCH_H_

#include "Application.h"
#include "Camera.h"

#include "ConnectFour.h"

class GameTreeSearch : public Application
{
public:
	bool startup();
	void shutdown();
	bool update();
	void draw();

	FlyCamera* m_camera;
	float m_timer;

	Game* m_game;
	AI* m_ai;

	vec3 m_pickPosition;
};


#endif // !_GAME_TREE_SEARCH_H_