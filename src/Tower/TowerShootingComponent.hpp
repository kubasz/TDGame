#pragma once

#ifndef TDF_TOWER_SHOOTING_COMPONENT
#define TDF_TOWER_SHOOTING_COMPONENT

class BulletFactory;

//! A Tower component which decides when to shoot, and how.
class TowerShootingComponent
{
public:
	virtual ~TowerShootingComponent() {}
	virtual void update(BulletFactory & bulletFactory) = 0;
};

//! Shoots a bullet every n seconds.
class TowerLinearShootingComponent final : public TowerShootingComponent
{
private:
	float charge_, maxCharge_;

public:
	TowerLinearShootingComponent(float shotsPerSecond);
	virtual void update(BulletFactory & bulletFactory) override;
};

#endif // TDF_TOWER_SHOOTING_COMPONENT
