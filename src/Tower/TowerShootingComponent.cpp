#include "../Bullet/BulletFactory.hpp"
#include "../Tower/TowerShootingComponent.hpp"

TowerLinearShootingComponent::TowerLinearShootingComponent(float shotsPerSecond, std::string bulletType)
	: charge_(0.f)
	, maxCharge_(shotsPerSecond)
	, bulletType_(bulletType)
{}

void TowerLinearShootingComponent::update(sf::Time dt, BulletFactory & bulletFactory)
{
	charge_ -= dt.asSeconds();

	if (charge_ <= 0.f) {
		bulletFactory.shoot(bulletType_);
		charge_ += maxCharge_ + (rand()%1000)/4000.f;
	}
}
