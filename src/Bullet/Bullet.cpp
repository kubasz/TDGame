#include "Bullet.hpp"

Bullet::Bullet(
	std::unique_ptr<BulletMovementComponent> movementComponent,
	std::unique_ptr<BulletDisplayComponent> displayComponent)
	: movementComponent_(std::move(movementComponent))
	, displayComponent_(std::move(displayComponent))
{}
