#pragma once

#ifndef TDF_CREEP_DISPLAY_COMPONENT_HPP
#define TDF_CREEP_DISPLAY_COMPONENT_HPP

#include <SFML/Graphics.hpp>

#include <Thor/Animations.hpp>

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
	float radius_;
	const CreepWalkComponent & walkComponent_;
	thor::FrameAnimation animation_;
	sf::Sprite sprite_;
	sf::Time animation_time_;
	
	static const sf::Time animation_duration_;

public:
	CreepDotDisplayComponent(const CreepWalkComponent & walkComponent, 
		float radius, const sf::Texture &texture, const thor::FrameAnimation& animation);
	virtual void render(sf::RenderTarget & target) override;
	virtual bool isHit(sf::Vector2f point) const override;
	virtual void update(const sf::Time &dt) override;
};

#endif // TDF_CREEP_DISPLAY_COMPONENT_HPP
