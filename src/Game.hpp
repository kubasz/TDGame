#pragma once

#ifndef TDF_GAME_HPP
#define TDF_GAME_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <Thor/Resources.hpp>

class GameState;

class Game
{
private:
	sf::RenderWindow window_;
	sfg::SFGUI sfgui_;
	thor::ResourceHolder<sf::Texture, std::string> textures_holder_;
	thor::ResourceHolder<sf::SoundBuffer, std::string> sounds_holder_;

	std::unique_ptr<GameState> currentState_, nextState_;

	//! Load all resources into thor::ResourceHolder
	void loadResources();

	//! Load textures into thor::ResourceHolder
	void loadTextures();
	//! Load sounds into thor::ResourceHolder
	void loadSounds();

public:
	Game(int argc, char ** argv);
	~Game();

	void setNextState(std::unique_ptr<GameState> gameState);
	int run();
	void quitNextFrame();

	int getWidth() const;
	int getHeight() const;
	const sf::Texture& getTexture(const std::string &id) const;
	const sf::SoundBuffer& getSound(const std::string &id) const;
};

#endif // TDF_GAME_HPP
