#pragma once

#include <vector>
#include <map>
#include <stdlib.h>

// base class to define a zero-sum game with two opponents
// actions are defined as int's
class Game {
public:

	enum State {
		UNKNOWN,
		PLAYER_ONE,
		PLAYER_TWO,
		DRAW,
	};

	Game() :
		m_currentPlayer(PLAYER_ONE),
		m_currentOpponent(PLAYER_TWO) {

	}
	virtual ~Game() {}

	// returns wether someone is winning or not
	virtual State	getCurrentGameState() const = 0;

	// these switch whenever an action is made
	State			getCurrentPlayer() const	{ return m_currentPlayer;	}
	State			getCurrentOpponent() const	{ return m_currentOpponent; }

	// get a list of valid actions for current player
	virtual void	getValidActions(std::vector<int>& actions) const = 0;
	virtual bool	isActionValid(int action) const = 0;

	// performs an action for the current player, and switches current player
	virtual void	performAction(int action) = 0;

	// draw the game
	virtual void	draw() const = 0;

	// clones the current game state
	virtual Game*	clone() const = 0;

protected:

	State	m_currentPlayer;
	State	m_currentOpponent;
};

// base class for an A.I. opponent
class AI {
public:

	AI() {}
	virtual ~AI() {}

	// makes a decision for the current game
	virtual int	makeDecision(const Game& game) = 0;
};

// an A.I. opponent that chooses purely random actions to perform
class RandomAI : public AI {
public:

	RandomAI() {}
	virtual ~RandomAI() {}

	// randomly choose an action to perform
	virtual int	makeDecision(const Game& game) {
		std::vector<int> actions;
		game.getValidActions(actions);
		if (actions.size() > 0)
			return actions[rand() % actions.size()];
		else
			return -1;
	}
};

class MCTS : public AI
{
public:
	MCTS(int playouts) :
		m_playouts(playouts) {}

	virtual ~MCTS() {}

	virtual int makeDecision(const Game& game)
	{
		std::vector<int> actions; //create a vector to hold the actions
		game.getValidActions(actions); //put the actions in the vector
		std::map<float, std::vector<int>> scores;

		if (actions.size() == 0)
			return -1;


		for (int actionIndex = 0; actionIndex < (int)actions.size(); ++actionIndex) // for each action
		{
			int choiceValue = 0; //set the return value to 0

			for (int playIndex = 0; playIndex < m_playouts; ++playIndex) //for each playout
			{
				//if the player went here, would it win? If so, play here

				Game* gameClone = game.clone(); //clone the current game so we can modify it
				gameClone->performAction(actions[actionIndex]); //perform the action we want to take on the clone

				//simulate random choices until we find a winner
				std::vector<int> cloneChoices; //list of choices made
				while (gameClone->getCurrentGameState() == Game::UNKNOWN) //this will change when we have a winner
				{
					gameClone->getValidActions(cloneChoices);//find how many choices we can make at this time
					gameClone->performAction(cloneChoices[rand() % cloneChoices.size()]); //pick one at random
				}

				if (gameClone->getCurrentGameState() == Game::PLAYER_ONE)
				{
					--choiceValue;
				}
				else if (gameClone->getCurrentGameState() == Game::PLAYER_TWO)
				{
					++choiceValue;
				}

				delete gameClone; //delete the clone we made
			}
			//give the action a score based on value / playouts;
			scores[(int)(choiceValue / m_playouts)].push_back(actions[actionIndex]);
		}

		int bestScore = INT_MIN;
		for (auto key : scores)
		{
			if (bestScore < key.first)
			{
				bestScore = key.first;
			}
		}

		return scores[bestScore][rand() % scores[bestScore].size()];
	}

private:
	// the number of times it will simulate a game
	int m_playouts;
};