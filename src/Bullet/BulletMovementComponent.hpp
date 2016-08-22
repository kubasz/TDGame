#pragma once

#ifndef TDF_BULLET_MOVEMENT_COMPONENT_HPP
#define TDF_BULLET_MOVEMENT_COMPONENT_HPP

#include <memory>
#include <SFML/System.hpp>

class Creep;
class BulletDamageComponent;

//! A component for Bullet implementing movement logic.
class BulletMovementComponent
{
public:
	virtual ~BulletMovementComponent() {}

	//! Updates position of the Bullet.
	virtual void update(sf::Time dt) = 0;

	virtual sf::Vector2f getPosition() const = 0;
	virtual sf::Vector2f getTargetPosition() const = 0;
	virtual bool isAlive() const = 0;
};

//! Implements a bullet which always hits the target after a given delay.
class BulletTimedMovementComponent final : public BulletMovementComponent
{
private:
	std::unique_ptr<BulletDamageComponent> damageComponent_;
	sf::Vector2f position_;
	float timeToHit_;
	std::weak_ptr<Creep> target_;

public:
	BulletTimedMovementComponent(
		std::unique_ptr<BulletDamageComponent> damageComponent,
		float time,
		const std::shared_ptr<Creep> & target,
		sf::Vector2f startingPosition);
	void update(sf::Time dt) override;
	virtual sf::Vector2f getPosition() const override;
	virtual sf::Vector2f getTargetPosition() const override;
	virtual bool isAlive() const override;
};


//! Implements a bullet which does not move and hits the target after a given delay.
class BulletLaserMovementComponent final : public BulletMovementComponent
{
private:
	std::unique_ptr<BulletDamageComponent> damageComponent_;
	sf::Vector2f position_;
	float timeToHit_;
	std::weak_ptr<Creep> target_;

public:
	BulletLaserMovementComponent(
		std::unique_ptr<BulletDamageComponent> damageComponent,
		float time,
		const std::shared_ptr<Creep> & target,
		sf::Vector2f startingPosition);
	void update(sf::Time dt) override;
	virtual sf::Vector2f getPosition() const override;
	virtual sf::Vector2f getTargetPosition() const override;
	virtual bool isAlive() const override;
};

#endif // TDF_BULLET_MOVEMENT_COMPONENT_HPP
