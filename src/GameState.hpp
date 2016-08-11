#pragma once

#ifndef TDF_GAME_STATE_HPP
#define TDF_GAME_STATE_HPP

#include <cstdint>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class GameState
{
public:
	// enum class Identifier : uint32_t { MENU, LEVEL_SELECT, LEVEL };

	GameState();
	virtual ~GameState() {}

	virtual void update() {};
	virtual void render(sf::RenderTarget & target) {};
	virtual void handleEvent(const sf::Event & evt) {};

	// virtual Identifier getIdentifier() const = 0;
};

#endif // TDF_GAME_STATE_HPP
