#pragma once

#ifndef TDF_BULLET_DISPLAY_COMPONENT_HPP
#define TDF_BULLET_DISPLAY_COMPONENT_HPP

#include "../Renderable.hpp"

class BulletMovementComponent;

//! A component for Bullet implementing the look of a Bullet.
class BulletDisplayComponent : public Renderable {};

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

//! A very simple look for Bullet that looks like laser.
class BulletLaserDisplayComponent final : public BulletDisplayComponent
{
private:
	BulletMovementComponent & movementComponent_;

public:
	BulletLaserDisplayComponent(BulletMovementComponent &movementComponent);
	virtual void render(sf::RenderTarget & target) override;
};

#endif // TDF_BULLET_DISPLAY_COMPONENT_HPP
