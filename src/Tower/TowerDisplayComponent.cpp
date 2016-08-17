// For MSVC's <cmath> to work
#define _USE_MATH_DEFINES
#include <cmath>
#include "../Creep/Creep.hpp"
#include "TowerDisplayComponent.hpp"
#include "TowerTargetingComponent.hpp"

TowerSimpleDisplayComponent::TowerSimpleDisplayComponent(
	TowerTargetingComponent & targeting,
	sf::Vector2f position)
	: targetingComponent_(targeting)
	, angle_(0.f)
	, position_(position)
{
	baseShape_.setRadius(0.25f);
	baseShape_.setFillColor(sf::Color::Black);
	baseShape_.setPosition(position);
	baseShape_.setOrigin(0.25f, 0.25f);
	barrelShape_.setPosition(0.f, -0.125f);
	barrelShape_.setSize({ 0.5f, 0.25f });
	barrelShape_.setFillColor(sf::Color::Black);
}

void TowerSimpleDisplayComponent::render(sf::RenderTarget & target)
{
	auto targeted = targetingComponent_.getTargetedCreep();
	if (targeted) {
		const auto difference = targeted->getPosition() - position_;
		angle_ = (float)(atan2(difference.y, difference.x) * (180.f / M_PI));
	}

	sf::Transform transform;
	transform.translate(position_).rotate(angle_);

	target.draw(baseShape_);
	target.draw(barrelShape_, transform);
}
