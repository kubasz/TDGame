#pragma once

#ifndef TDF_TOWER_DISPLAY_COMPONENT_HPP
#define TDF_TOWER_DISPLAY_COMPONENT_HPP

#include <SFML/System.hpp>

#include "../Renderable.hpp"
#include "../Game.hpp"

class TowerTargetingComponent;
class Game;

//! A Tower component implementing the look of the tower.
class TowerDisplayComponent : public Renderable {};

//! A simple look for the tower.
class TowerSimpleDisplayComponent final : public TowerDisplayComponent
{
private:
	sf::Sprite sprite_;
	sf::RectangleShape barrelShape_;
	TowerTargetingComponent & targetingComponent_;
	float angle_;
	sf::Vector2f position_;

public:
	TowerSimpleDisplayComponent(
		TowerTargetingComponent & targeting,
		sf::Vector2f position, const sf::Texture& texture);
	virtual void render(sf::RenderTarget & target) override;
};

#endif // TDF_TOWER_DISPLAY_COMPONENT_HPP
