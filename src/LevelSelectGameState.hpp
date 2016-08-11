#pragma once

#ifndef TDF_LEVELSELECTGAMESTATE_HPP
#define TDF_LEVELSELECTGAMESTATE_HPP

#include <SFGUI/Widgets.hpp>
#include "GameState.hpp"

class Game;

class LevelSelectGameState : public GameState
{
private:
	Game & game_;

	//! Layout with buttons to set during next update. This is needed
	//! because we can't delete widgets in a signal handler.
	sfg::ScrolledWindow::Ptr guiNextLayout_;
	sfg::Window::Ptr guiMainWindow_;
	sfg::Desktop guiDesktop_;

	std::weak_ptr<sfg::Window> guiErrorPopup_;

public:
	LevelSelectGameState(Game & game);

	void setPath(const std::string & path);

	virtual void update() override;
	virtual void render(sf::RenderTarget & target) override;
	virtual void handleEvent(const sf::Event & evt) override;

	void handleResize(int width, int height);
	void popupLevelReadError();
};

#endif // TDF_LEVELSELECTGAMESTATE_HPP
