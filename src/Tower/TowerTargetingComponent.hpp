#pragma once

#ifndef TDF_TOWER_TARGETING_COMPONENT_HPP
#define TDF_TOWER_TARGETING_COMPONENT_HPP

#include <memory>

#include <SFML/System.hpp>

class Creep;
class CreepQueryService;

//! Chooses the Creep for Tower to shoot at.
class TowerTargetingComponent
{
private:
	std::shared_ptr<Creep> targetedCreep_;

protected:
	virtual std::shared_ptr<Creep> chooseCreep(CreepQueryService & /*service*/)
	{
		return nullptr;
	}

public:
	virtual ~TowerTargetingComponent() {}
	inline void update(CreepQueryService & service)
	{
		targetedCreep_ = chooseCreep(service);
	}
	inline std::shared_ptr<Creep> getTargetedCreep() const
	{
		return targetedCreep_;
	}
};

//! Chooses the closest Creep.
class TowerClosestTargetingComponent final : public TowerTargetingComponent
{
private:
	sf::Vector2f position_;
	float range_;

protected:
	virtual std::shared_ptr<Creep> chooseCreep(CreepQueryService & service) override;

public:
	TowerClosestTargetingComponent(sf::Vector2f position, float range = std::numeric_limits<float>::infinity());
};

//! A helper targeting component which uses another component to choose a targeted
//! Creep, then returns it until the Creep dies.
class TowerTargetingLockOnComponent final : public TowerTargetingComponent
{
private:
	std::shared_ptr<TowerTargetingComponent> base_;

protected:
	virtual std::shared_ptr<Creep> chooseCreep(CreepQueryService & service) override;

public:
	TowerTargetingLockOnComponent(std::shared_ptr<TowerTargetingComponent> base);
};

#endif // TDF_TOWER_TARGETING_COMPONENT_HPP
