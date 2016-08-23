#include "CreepDisplayComponent.hpp"
#include "CreepWalkComponent.hpp"

const sf::Time CreepDotDisplayComponent::animation_duration_ = sf::milliseconds(1000);

CreepDotDisplayComponent::CreepDotDisplayComponent(
	const CreepWalkComponent & walkComponent,
	float radius, const sf::Texture& texture, const thor::FrameAnimation& animation)
	: radius_(radius)
	, walkComponent_(walkComponent)
	, animation_(animation)
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

	animation_(sprite_, (float)animation_time_.asMilliseconds() / (float)animation_duration_.asMilliseconds());
	sprite_.setPosition(pos);
	target.draw(sprite_);
}

bool CreepDotDisplayComponent::isHit(sf::Vector2f point) const
{
	const auto diff = point - walkComponent_.getPosition();
	return diff.x * diff.x + diff.y * diff.y <= radius_ * radius_;
}

void CreepDotDisplayComponent::update(const sf::Time &dt)
{
	animation_time_ += dt;

	while(animation_time_ > animation_duration_)
		animation_time_ -= animation_duration_;
}