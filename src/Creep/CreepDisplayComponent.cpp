#include "CreepDisplayComponent.hpp"
#include "CreepWalkComponent.hpp"

CreepDotDisplayComponent::CreepDotDisplayComponent(
	const CreepWalkComponent & walkComponent,
	float radius)
	: circle_(radius)
	, radius_(radius)
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

bool CreepDotDisplayComponent::isHit(sf::Vector2f point) const
{
	const auto diff = point - walkComponent_.getPosition();
	return diff.x * diff.x + diff.y * diff.y <= radius_ * radius_;
}
