#include "Bullet.hpp"
#include "BulletDisplayComponent.hpp"
#include "BulletMovementComponent.hpp"
#include "BulletFactory.hpp"
#include "../Level.hpp"

std::shared_ptr<Bullet> BulletFactory::innerCreateBullet(
	const std::string & bulletName) const
{
	if (bulletName == "GenericBullet") {
		auto movement = std::unique_ptr<BulletTimedMovementComponent>(new BulletTimedMovementComponent(
			1.f, target_, 20, position_));
		auto display = std::unique_ptr<BulletSimpleDisplayComponent>(
			new BulletSimpleDisplayComponent(0.0625f, *movement.get()));
		return std::make_shared<Bullet>(std::move(movement), std::move(display));
	}

	throw std::runtime_error("Unknown Bullet type: " + bulletName);
}

BulletFactory::BulletFactory(std::shared_ptr<LevelInstance> levelInstance, sf::Vector2f position)
	: levelInstance_(levelInstance)
	, position_(position)
{}

void BulletFactory::createBullet(
	const std::string & bulletName)
{
	levelInstance_->registerBullet(innerCreateBullet(bulletName));
}
