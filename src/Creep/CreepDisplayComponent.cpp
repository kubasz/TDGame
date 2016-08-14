#include "CreepDisplayComponent.hpp"
#include "CreepWalkComponent.hpp"

CreepDotDisplayComponent::CreepDotDisplayComponent(
	const CreepWalkComponent & walkComponent,
	float radius)
	: circle_(radius)
	, walkComponent_(walkComponent)
{
	circle_.setFillColor(sf::Color::Black);
	circle_.setOrigin(radius, radius);
}

void CreepDotDisplayComponent::render(sf::RenderTarget & target)
{
	circle_.setPosition(walkComponent_.getPosition());
	target.draw(circle_);
}
