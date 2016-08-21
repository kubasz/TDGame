#include "CreepDisplayComponent.hpp"
#include "CreepWalkComponent.hpp"

CreepDotDisplayComponent::CreepDotDisplayComponent(
	const CreepWalkComponent & walkComponent,
	float radius, const sf::Texture& texture)
	: radius_(radius)
	, walkComponent_(walkComponent)
{
	sprite_.setTexture(texture);
	sprite_.setOrigin(radius, radius);
	sprite_.setScale(1.f/32.f, 1.f/32.f);
}

void CreepDotDisplayComponent::render(sf::RenderTarget & target)
{
	sf::Vector2f pos = walkComponent_.getPosition();
	pos.x -= 0.5f;
	pos.y -= 0.5f;

	sprite_.setPosition(pos);
	target.draw(sprite_);
}

bool CreepDotDisplayComponent::isHit(sf::Vector2f point) const
{
	const auto diff = point - walkComponent_.getPosition();
	return diff.x * diff.x + diff.y * diff.y <= radius_ * radius_;
}
