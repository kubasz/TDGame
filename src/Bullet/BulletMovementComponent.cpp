#include <algorithm>
#include <memory>
#include <SFML/System.hpp>
#include "BulletMovementComponent.hpp"
#include "../Creep/Creep.hpp"
#include "../Constants.hpp"

BulletTimedMovementComponent::BulletTimedMovementComponent(
	float time,
	const std::shared_ptr<Creep> & target,
	int32_t damage,
	sf::Vector2f startingPosition)
	: position_(startingPosition)
	, timeToHit_(time)
	, target_(target)
	, damage_(damage)
{}

void BulletTimedMovementComponent::update()
{
	auto lockedTarget = target_.lock();
	if (!lockedTarget)
		return;

	const float nextTime = std::max(timeToHit_ - Constants::FPS, 0.f);
	const float factor = (nextTime == 0.f) ? 0.f : (nextTime / timeToHit_);
	const auto creepPosition = lockedTarget->getPosition();

	position_ = creepPosition + factor * (position_ - creepPosition);
	timeToHit_ = nextTime;

	if (nextTime == 0.f) {
		lockedTarget->inflictDamage(damage_);
		target_.reset();
	}
}

sf::Vector2f BulletTimedMovementComponent::getPosition() const
{
	return position_;
}

bool BulletTimedMovementComponent::isAlive() const
{
	if (timeToHit_ <= 0.f)
		return false;

	auto lockedTarget = target_.lock();
	return lockedTarget && lockedTarget->isAlive();
}
