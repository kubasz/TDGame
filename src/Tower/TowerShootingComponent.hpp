#pragma once

#ifndef TDF_TOWER_SHOOTING_COMPONENT
#define TDF_TOWER_SHOOTING_COMPONENT

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

class BulletFactory;

//! A Tower component which decides when to shoot, and how.
class TowerShootingComponent
{
public:
	virtual ~TowerShootingComponent() {}
	virtual void update(sf::Time /*dt*/, BulletFactory & /*bulletFactory*/) {};
};

//! Shoots a bullet every n seconds.
class TowerLinearShootingComponent final : public TowerShootingComponent
{
private:
	float charge_, maxCharge_;
	std::string bulletType_;
	sf::Sound sound_;

public:
	TowerLinearShootingComponent(float shotsPerSecond, std::string bulletType,
								 const sf::SoundBuffer &sound);
	virtual void update(sf::Time dt, BulletFactory & bulletFactory) override;
};

#endif // TDF_TOWER_SHOOTING_COMPONENT
