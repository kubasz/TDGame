#pragma once

#ifndef TDF_LEVEL_GAME_STATE_HPP
#define TDF_LEVEL_GAME_STATE_HPP

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFGUI/Widgets.hpp>

#include "GameState.hpp"
#include "../Level.hpp"

// TODO: Separate tower placing logic from this class

class Game;

class LevelGameState final : public GameState
{
private:
	std::shared_ptr<Level> level_;
	std::shared_ptr<LevelInstance> levelInstance_;
	sfg::Label::Ptr guiCashLabel_;
	sfg::Box::Ptr guiInfoPanelLocation_;
	int64_t oldCash_;
	sfg::Window::Ptr guiMainWindow_;
	sfg::Desktop guiDesktop_;
	sf::View levelView_;
	sf::Vector2i windowSize_;
	sf::Vector2i lastMousePosition_;
	sf::Vector2f lastMouseLevelPosition_;
	sf::Vector2i hoveredTile_;

	std::shared_ptr<Selectable> selectedObject_;

	std::string placedTowerTypeName_;
	bool isPlacingTower_;

	void createTowerCreationButtons(const sfg::Box::Ptr & layout);
	void centerView();

	// TODO: Convert these handlers to Thor dispatcher ?

	void handleResize(int width, int height);
	void handleClick(sf::Vector2i position);
	void handleKeyPress(sf::Keyboard::Key key);

public:
	LevelGameState(Game & game, std::istream & source);

	virtual void update() override;
	virtual void render(sf::RenderTarget & target) override;
	virtual void handleEvent(const sf::Event & evt) override;
};

#endif // TDF_LEVEL_GAME_STATE_HPP
