#include "../MakeUnique.hpp"
#include "Bullet.hpp"
#include "BulletDisplayComponent.hpp"
#include "BulletMovementComponent.hpp"
#include "BulletDamageComponent.hpp"
#include "BulletFactory.hpp"
#include "../Level.hpp"

std::shared_ptr<Bullet> BulletFactory::innerCreateBullet(
	const std::string & bulletName) const
{
	if (bulletName == "GenericBullet") {
		auto movement = std::make_unique<BulletTimedMovementComponent>(
			std::make_unique<BulletSimpleDamageComponent>(20), 1.f, target_, position_);
		auto display = std::make_unique<BulletSimpleDisplayComponent>(0.0625f, *movement.get());
		return std::make_shared<Bullet>(std::move(movement), std::move(display));
	}

	if (bulletName == "LaserBullet") {
		auto movement = std::make_unique<BulletLaserMovementComponent>(
			std::make_unique<BulletSimpleDamageComponent>(10), 0.1f, target_, position_);
		auto display = std::make_unique<BulletLaserDisplayComponent>(*movement.get());
		return std::make_shared<Bullet>(std::move(movement), std::move(display));
	}

	if (bulletName == "SlownessBullet") {
		auto movement = std::make_unique<BulletTimedMovementComponent>(
				std::make_unique<BulletBuffDamageComponent>(CreepBuff(5, CreepBuff::Type::BUFF_SPEED, -20)), 1.0f, target_, position_);
		auto display = std::make_unique<BulletSimpleDisplayComponent>(0.25f, *movement.get());
		return std::make_shared<Bullet>(std::move(movement), std::move(display));
	}

	if (bulletName == "WeaknessBullet") {
		auto movement = std::make_unique<BulletTimedMovementComponent>(
				std::make_unique<BulletBuffDamageComponent>(CreepBuff(10, CreepBuff::Type::BUFF_VULNERABILITY, 500)), 1.0f, target_, position_);
		auto display = std::make_unique<BulletSimpleDisplayComponent>(0.25f, *movement.get());
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
