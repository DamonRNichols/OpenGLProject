#include "GameTreeSearch.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

bool GameTreeSearch::startup()
{
	if (Application::startup() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();

	m_camera = new FlyCamera();

	m_game = new ConnectFour();
	//m_ai = new RandomAI();
	m_ai = new MCTS(1000);

	return true;
}

void GameTreeSearch::shutdown()
{
	Gizmos::destroy();

	delete m_ai;
	delete m_game;

	Application::shutdown();
}

bool GameTreeSearch::update()
{
	if (Application::update() == false)
	{
		return false;
	}

	Gizmos::clear();

	float dt = (float)glfwGetTime();
	glfwSetTime(0.0f);
	m_timer += dt;

	m_camera->update(dt);
	/*
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i <= 20; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? white : black);
		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? white : black);
	}*/

	// kepp track of if the mouse has been clicked
	static bool moveMade = false;
	// if the game isn't over...
	if (m_game->getCurrentGameState() == Game::UNKNOWN) 
	{
		// if it is the user's turn (player 1)
		if (m_game->getCurrentPlayer() == Game::PLAYER_ONE) 
		{
			// if the mouse button is down...
			if (glfwGetMouseButton(m_window, 0) == GLFW_PRESS) 
			{
				// if the mouse JUST went down then make a choice
				if (moveMade == false) 
				{
					moveMade = true;
					// get the mouse position within the game grid
					double x = 0, y = 0;
					glfwGetCursorPos(m_window, &x, &y);
					m_pickPosition = m_camera->pickAgainstPlane(
						(float)x, (float)y, glm::vec4(0, 1, 0, 0));
					// determine which column was chosen
					int column = (int)((m_pickPosition.z +
						ConnectFour::COLUMNS) / 2);
					// if we clicked in the area for a
					// valid column then perform that action
					if (m_game->isActionValid(column))
						m_game->performAction(column);
				}
			}
			else
				moveMade = false;
		}
		else 
		{
			// it is the opponent's turn (player 2)
			// use the A.I. to make a decision
			m_game->performAction(m_ai->makeDecision(*m_game));
		}
	}



	return true;
}

void GameTreeSearch::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_game->draw();
	
	Gizmos::draw(m_camera->m_view_proj);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}