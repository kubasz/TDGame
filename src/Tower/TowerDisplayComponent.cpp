// For MSVC's <cmath> to work
#define _USE_MATH_DEFINES
#include <cmath>
#include "../Creep/Creep.hpp"
#include "TowerDisplayComponent.hpp"
#include "TowerTargetingComponent.hpp"

TowerSimpleDisplayComponent::TowerSimpleDisplayComponent(
		sf::Vector2f position,
		const sf::Texture & texture)
		: position_(position)
{
	position_.x -= 0.5;
	position_.y -= 0.5;

	sprite_.setTexture(texture);
	sprite_.setPosition(position_);
	sprite_.setScale(1.f/32.f, 1.f/32.f);
}

void TowerSimpleDisplayComponent::render(sf::RenderTarget & target)
{
	target.draw(sprite_);
}

TowerTargettingDisplayComponent::TowerTargettingDisplayComponent(
	TowerTargetingComponent & targeting,
	sf::Vector2f position,
	const sf::Texture & texture,
	const sf::Texture& textureHead)
	: targetingComponent_(targeting)
	, angle_(0.f)
	, position_(position)
{
	position_.x -= 0.5;
	position_.y -= 0.5;

	sprite_.setTexture(texture);
	sprite_.setPosition(position_);
	sprite_.setScale(1.f/32.f, 1.f/32.f);

	barrelShape_.setPosition(0.f, 0.f);
	barrelShape_.setSize({ 1.f, 1.f });
	barrelShape_.setFillColor(sf::Color::White);
	barrelShape_.setTexture(&textureHead);
	barrelShape_.setOrigin(0.5f, 0.5f);
}

void TowerTargettingDisplayComponent::render(sf::RenderTarget & target)
{
	auto targeted = targetingComponent_.getTargetedCreep();

	if (targeted) {
		const auto difference = targeted->getPosition() - position_;
		angle_ = (float)(atan2(difference.y, difference.x) * (180.f / M_PI));
	}

	sf::Transform transform;
	transform.translate(position_ + sf::Vector2f(0.5f, 0.5f)).rotate(angle_);

	target.draw(sprite_);
	target.draw(barrelShape_, transform);
}
