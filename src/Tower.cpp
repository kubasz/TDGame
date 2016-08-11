#include <cmath>
#include "Creep.hpp"
#include "Tower.hpp"
#include "Bullet.hpp"

#ifndef M_PI
#define M_PI 3.1415
#endif

static const float FRAME_TIME = 1.f / 60.f;

void TowerClosestTargetingComponent::update(CreepQueryService & service)
{
	targetedCreep_ = service.getClosestCreep(position_);
}

TowerLinearShootingComponent::TowerLinearShootingComponent(float shotsPerSecond)
	: charge_(0.f)
	, maxCharge_(shotsPerSecond)
{}

void TowerLinearShootingComponent::update(BulletFactory & bulletFactory)
{
	charge_ -= FRAME_TIME;

	if (charge_ <= 0.f) {
		bulletFactory.shoot("GenericBullet");
		charge_ += maxCharge_;
	}
}

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

void Tower::update(BulletFactory & bulletFactory, CreepQueryService & queryService)
{
	targetingComponent_->update(queryService);
	auto target = targetingComponent_->getTargetedCreep();
	if (target) {
		bulletFactory.setTarget(target);
		shootingComponent_->update(bulletFactory);
	}
}

sf::FloatRect Tower::getBoundingRect() const
{
	return{
		position_.x - 0.5f, position_.y - 0.5f,
		1.f, 1.f
	};
}

bool Tower::isHit(sf::Vector2f point) const
{
	return getBoundingRect().contains(point);
}

sfg::Widget::Ptr Tower::getPanel()
{
	// TOOD
	return nullptr;
}
