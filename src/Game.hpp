#pragma once

#ifndef TDF_GAME_HPP
#define TDF_GAME_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class GameState;

class Game
{
private:
	sf::RenderWindow window_;
	sfg::SFGUI sfgui_;

	std::unique_ptr<GameState> currentState_, nextState_;

public:
	Game(int argc, char ** argv);
	~Game();

	void setNextState(std::unique_ptr<GameState> gameState);
	int run();
	void quitNextFrame();

	int getWidth() const;
	int getHeight() const;
};

#endif // TDF_GAME_HPP
