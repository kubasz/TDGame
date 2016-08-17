#pragma once

#ifndef TDF_GAME_STATE_HPP
#define TDF_GAME_STATE_HPP

#include <cstdint>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class GameState
{
public:
	virtual ~GameState() {}

	virtual void update(sf::Time /*dt*/) {};
	virtual void render(sf::RenderTarget & /*target*/) {};
	virtual void handleEvent(const sf::Event & /*evt*/) {};
};

#endif // TDF_GAME_STATE_HPP
