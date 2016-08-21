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

BulletLaserDisplayComponent::BulletLaserDisplayComponent(BulletMovementComponent &movementComponent)
		: movementComponent_(movementComponent)
{
}

void BulletLaserDisplayComponent::render(sf::RenderTarget & target)
{
	sf::Vertex v[] = {
			sf::Vertex(movementComponent_.getPosition(), sf::Color(255, 0, 0)),
			sf::Vertex(movementComponent_.getTargetPosition(), sf::Color(255, 0, 0))
	};
	target.draw(v, 2, sf::PrimitiveType::Lines);
}