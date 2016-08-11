#include "Bullet.hpp"

static const float FRAME_TIME = 1.f / 60.f;

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

	const float nextTime = std::max(timeToHit_ - FRAME_TIME, 0.f);
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

std::shared_ptr<Bullet> BulletFactory::innerCreateBullet(
	const std::string & bulletName) const
{
	if (bulletName == "GenericBullet") {
		auto movement = std::make_unique<BulletTimedMovementComponent>(
			1.f, target_, 20, position_);
		auto display = std::make_unique<BulletSimpleDisplayComponent>(0.0625f, *movement.get());
		return std::make_shared<Bullet>(std::move(movement), std::move(display));
	}

	throw std::runtime_error("Unknown Bullet type: " + bulletName);
}

void BulletFactory::createBullet(
	const std::string & bulletName)
{
	container_.emplace_back(std::move(innerCreateBullet(bulletName)));
}
