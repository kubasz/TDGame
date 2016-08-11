#pragma once

#ifndef TDF_BULLET_HPP
#define TDF_BULLET_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include "Creep.hpp"

//! A component for Bullet implementing movement logic.
class BulletMovementComponent
{
public:
	virtual ~BulletMovementComponent() {}

	//! Updates position of the Bullet.
	virtual void update() = 0;

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
	void update() override;
	virtual sf::Vector2f getPosition() const override;
	virtual bool isAlive() const override;
};

//! A component for Bullet implementing the look of a Bullet.
class BulletDisplayComponent
{
public:
	virtual ~BulletDisplayComponent() {}
	virtual void render(sf::RenderTarget & target) = 0;
};

//! A very simple look for Bullet.
class BulletSimpleDisplayComponent final : public BulletDisplayComponent
{
private:
	sf::CircleShape circleShape_;
	BulletMovementComponent & movementComponent_;

public:
	BulletSimpleDisplayComponent(
		float radius,
		BulletMovementComponent & movementComponent);
	virtual void render(sf::RenderTarget & target) override;
};

//! Represents a Bullet shot from a Tower.
class Bullet
{
private:
	std::unique_ptr<BulletMovementComponent> movementComponent_;
	std::unique_ptr<BulletDisplayComponent> displayComponent_;

public:
	Bullet(
		std::unique_ptr<BulletMovementComponent> movementComponent,
		std::unique_ptr<BulletDisplayComponent> displayComponent)
		: movementComponent_(std::move(movementComponent))
		, displayComponent_(std::move(displayComponent))
	{}

	inline void update()
	{
		movementComponent_->update();
	}

	inline void render(sf::RenderTarget & target)
	{
		displayComponent_->render(target);
	}

	inline sf::Vector2f getPosition() const
	{
		return movementComponent_->getPosition();
	}

	inline bool isAlive() const
	{
		return movementComponent_->isAlive();
	}
};

//! Its sole purpose is to create bullets.
class BulletFactory
{
private:
	std::vector<std::shared_ptr<Bullet>> & container_;
	std::shared_ptr<Creep> target_;
	const sf::Vector2f position_;

	std::shared_ptr<Bullet> innerCreateBullet(
		const std::string & bulletName) const;

public:
	BulletFactory(
		std::vector<std::shared_ptr<Bullet>> & container,
		sf::Vector2f position)
		: container_(container)
		, position_(position)
	{}

	//! \brief Sets the Creep which will be targeted by created Bullets.
	inline void setTarget(const std::shared_ptr<Creep> & target)
	{
		target_ = target;
	}

	//! \brief Creates bullet directed at target_.
	void createBullet(const std::string & bulletName);

	//! \brief Alternative for BulletFactory::createBullet.
	inline void shoot(const std::string & bulletName)
	{
		return createBullet(bulletName);
	}
};

#endif // TDF_BULLET_HPP
