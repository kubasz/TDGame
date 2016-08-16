#pragma once

#ifndef TDF_BULLET_HPP
#define TDF_BULLET_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include "../Creep/Creep.hpp"
#include "../Renderable.hpp"
#include "BulletDisplayComponent.hpp"
#include "BulletMovementComponent.hpp"

//! Represents a Bullet shot from a Tower.
class Bullet : public Renderable
{
private:
	std::unique_ptr<BulletMovementComponent> movementComponent_;
	std::unique_ptr<BulletDisplayComponent> displayComponent_;

public:
	Bullet(
		std::unique_ptr<BulletMovementComponent> movementComponent,
		std::unique_ptr<BulletDisplayComponent> displayComponent);

	inline void update(sf::Time dt)
	{
		movementComponent_->update(dt);
	}

	inline virtual void render(sf::RenderTarget & target) override
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

#endif // TDF_BULLET_HPP
