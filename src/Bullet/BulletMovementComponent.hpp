#pragma once

#ifndef TDF_BULLET_MOVEMENT_COMPONENT_HPP
#define TDF_BULLET_MOVEMENT_COMPONENT_HPP

#include <memory>
#include <SFML/System.hpp>

class Creep;

//! A component for Bullet implementing movement logic.
class BulletMovementComponent
{
public:
	virtual ~BulletMovementComponent() {}

	//! Updates position of the Bullet.
	virtual void update(sf::Time dt) = 0;

	virtual sf::Vector2f getPosition() const = 0;
	virtual bool isAlive() const = 0;
};

//! Implements a bullet which always hits the target after a given delay.
class BulletTimedMovementComponent final : public BulletMovementComponent
{
private:
	sf::Vector2f position_;
	float timeToHit_;
	std::weak_ptr<Creep> target_;
	int32_t damage_;

public:
	BulletTimedMovementComponent(
		float time,
		const std::shared_ptr<Creep> & target,
		int32_t damage,
		sf::Vector2f startingPosition);
	void update(sf::Time dt) override;
	virtual sf::Vector2f getPosition() const override;
	virtual bool isAlive() const override;
};

#endif // TDF_BULLET_MOVEMENT_COMPONENT_HPP
