#include "BulletDisplayComponent.hpp"
#include "BulletMovementComponent.hpp"

BulletSimpleDisplayComponent::BulletSimpleDisplayComponent(
	float radius,
	BulletMovementComponent & movementComponent)
	: circleShape_(radius)
	, movementComponent_(movementComponent)
{
	circleShape_.setOrigin(radius, radius);
	circleShape_.setFillColor(sf::Color::Red);
}

void BulletSimpleDisplayComponent::render(sf::RenderTarget & target)
{
	circleShape_.setPosition(movementComponent_.getPosition());
	target.draw(circleShape_);
}
