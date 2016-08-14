#include "../Constants.hpp"
#include "../Bullet/BulletFactory.hpp"
#include "../Tower/TowerShootingComponent.hpp"

TowerLinearShootingComponent::TowerLinearShootingComponent(float shotsPerSecond)
	: charge_(0.f)
	, maxCharge_(shotsPerSecond)
{}

void TowerLinearShootingComponent::update(BulletFactory & bulletFactory)
{
	charge_ -= Constants::FPS;

	if (charge_ <= 0.f) {
		bulletFactory.shoot("GenericBullet");
		charge_ += maxCharge_;
	}
}
