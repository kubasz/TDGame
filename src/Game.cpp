#include <exception>
#include <iostream>
#include <SFGUI/Renderers.hpp>

#include "MakeUnique.hpp"
#include "Game.hpp"
#include "GameStates/GameState.hpp"
#include "GameStates/MenuGameState.hpp"

#ifdef _MSC_VER
#include <conio.h>
#define WAIT_ANY_KEY _getch()
#else
#include <cstdio>
#define WAIT_ANY_KEY getchar()
#endif

Game::Game(int argc, char ** argv)
{
	srand((unsigned int)time(nullptr));
	sfg::Renderer::Set(sfg::VertexArrayRenderer::Create());

	const sf::Uint32 style = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;
	window_.create(sf::VideoMode(640, 480), "TDFramework", style);
	if (!window_.isOpen())
		throw std::runtime_error("Failed to create window");

	window_.resetGLStates();
	window_.setFramerateLimit(60);
	window_.setActive();

	setNextState(std::make_unique<MenuGameState>(*this));
}

Game::~Game()
{}

void Game::setNextState(std::unique_ptr<GameState> gameState)
{
	nextState_ = std::move(gameState);
}

int Game::run()
{
	while (window_.isOpen()) {
		if (nextState_)
			currentState_ = std::move(nextState_);

		sf::Event evt;
		if (window_.pollEvent(evt)) {
			switch (evt.type) {
			case sf::Event::Closed:
				window_.close();
				break;
			default:
				if (currentState_)
					currentState_->handleEvent(evt);
				break;
			}
		}
		else {
			if (currentState_)
				currentState_->update();
			if (currentState_)
				currentState_->render(window_);
			sfgui_.Display(window_);
			window_.display();
		}
	}
	
	return 0;
}

void Game::quitNextFrame()
{
	window_.close();
}

int Game::getWidth() const
{
	return window_.getSize().x;
}

int Game::getHeight() const
{
	return window_.getSize().y;
}

int main(int argc, char ** argv)
{
	try {
		return Game(argc, argv).run();
	}
	catch (std::runtime_error & err) {
		std::cout << "Runtime error: " << err.what() << std::endl;
		WAIT_ANY_KEY;
		return 1;
	}
}
