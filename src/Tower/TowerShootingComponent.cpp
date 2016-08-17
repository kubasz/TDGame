#include "../Bullet/BulletFactory.hpp"
#include "../Tower/TowerShootingComponent.hpp"

TowerLinearShootingComponent::TowerLinearShootingComponent(float shotsPerSecond)
	: charge_(0.f)
	, maxCharge_(shotsPerSecond)
{}

void TowerLinearShootingComponent::update(sf::Time dt, BulletFactory & bulletFactory)
{
	charge_ -= dt.asSeconds();

	if (charge_ <= 0.f) {
		bulletFactory.shoot("GenericBullet");
		charge_ += maxCharge_;
	}
}
