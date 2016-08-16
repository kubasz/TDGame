#pragma once

#ifndef TDF_MENU_GAME_STATE_HPP
#define TDF_MENU_GAME_STATE_HPP

#include <SFGUI/Widgets.hpp>
#include "GameState.hpp"

class Game;

class MenuGameState : public GameState
{
private:
	Game & game_;
	sfg::Desktop guiDesktop_;

public:
	MenuGameState(Game & game);
	virtual ~MenuGameState() {};

	virtual void update(sf::Time dt) override final;
	virtual void render(sf::RenderTarget & target) override final;
	virtual void handleEvent(const sf::Event & evt) override final;
};

#endif // TDF_MENU_GAME_STATE_HPP
