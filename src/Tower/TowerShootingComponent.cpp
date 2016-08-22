#include <SFML/Audio/SoundBuffer.hpp>
#include "../Bullet/BulletFactory.hpp"
#include "../Tower/TowerShootingComponent.hpp"

TowerLinearShootingComponent::TowerLinearShootingComponent(float shotsPerSecond, std::string bulletType,
														   const sf::SoundBuffer &sound)
	: charge_(0.f)
	, maxCharge_(shotsPerSecond)
	, bulletType_(bulletType)
	, sound_(sound)
{
}

void TowerLinearShootingComponent::update(sf::Time dt, BulletFactory & bulletFactory)
{
	charge_ -= dt.asSeconds();

	if (charge_ <= 0.f) {
		sound_.play();
		bulletFactory.shoot(bulletType_);
		charge_ += maxCharge_ + (rand()%1000)/4000.f;
	}
}
