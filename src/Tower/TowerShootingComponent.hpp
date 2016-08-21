#pragma once

#ifndef TDF_TOWER_SHOOTING_COMPONENT
#define TDF_TOWER_SHOOTING_COMPONENT

#include <SFML/System.hpp>

class BulletFactory;

//! A Tower component which decides when to shoot, and how.
class TowerShootingComponent
{
public:
	virtual ~TowerShootingComponent() {}
	virtual void update(sf::Time dt, BulletFactory & bulletFactory) = 0;
};

//! Shoots a bullet every n seconds.
class TowerLinearShootingComponent final : public TowerShootingComponent
{
private:
	float charge_, maxCharge_;
	std::string bulletType_;

public:
	TowerLinearShootingComponent(float shotsPerSecond, std::string bulletType);
	virtual void update(sf::Time dt, BulletFactory & bulletFactory) override;
};

#endif // TDF_TOWER_SHOOTING_COMPONENT
