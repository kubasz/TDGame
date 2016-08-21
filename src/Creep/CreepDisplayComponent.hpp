#pragma once

#ifndef TDF_CREEP_DISPLAY_COMPONENT_HPP
#define TDF_CREEP_DISPLAY_COMPONENT_HPP

#include <SFML/Graphics.hpp>

#include "../Renderable.hpp"

class CreepWalkComponent;

//! Component of the Creep implementing its look.
class CreepDisplayComponent : public Renderable {};

class CreepCompositeDisplayComponent final
	: public CompositeRenderable<CreepDisplayComponent> {};

//! A very simple look for the creep.
class CreepDotDisplayComponent final : public CreepDisplayComponent
{
private:
	sf::Sprite sprite_;
	float radius_;
	const CreepWalkComponent & walkComponent_;

public:
	CreepDotDisplayComponent(const CreepWalkComponent & walkComponent, float radius, const sf::Texture &texture);
	virtual void render(sf::RenderTarget & target) override;
	virtual bool isHit(sf::Vector2f point) const override;
};

#endif // TDF_CREEP_DISPLAY_COMPONENT_HPP
