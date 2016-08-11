#pragma once

#ifndef TDF_TOWER_HPP
#define TDF_TOWER_HPP

#include <memory>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <json.hpp>

#include "Creep.hpp"
#include "Bullet.hpp"
#include "Selectable.hpp"

class CreepQueryService;

class TowerTargetingComponent
{
public:
	virtual ~TowerTargetingComponent() {}
	virtual void update(CreepQueryService & service) = 0;
	virtual std::shared_ptr<Creep> getTargetedCreep() const = 0;
};

// TODO: Move somewhere else
class Upgradeable
{
public:
	virtual ~Upgradeable() {}
	virtual unsigned int maxUpgradeState() const = 0;
	virtual unsigned int currentUpgradeState() const = 0;
	virtual unsigned int currentUpgradeCost() const = 0;
	virtual void upgrade() = 0;
};

// TODO: The practice of creep caching has little sense here... I created
// TowerTargetingLockOnComponent class for this purpose, but here
// it should be abandoned in favor of one method which chooses
// and returns a targeted creep
class TowerClosestTargetingComponent final : public TowerTargetingComponent
{
private:
	sf::Vector2f position_;
	std::shared_ptr<Creep> targetedCreep_;
public:
	TowerClosestTargetingComponent(sf::Vector2f position)
		: position_(position)
	{}
	virtual void update(CreepQueryService & service) override;
	virtual std::shared_ptr<Creep> getTargetedCreep() const override
	{
		return targetedCreep_;
	}
};

//! A helper targeting component which uses another component to choose a targeted
//! Creep, then returns it until the Creep dies.
class TowerTargetingLockOnComponent final : public TowerTargetingComponent
{
private:
	std::shared_ptr<TowerTargetingComponent> base_;
	std::shared_ptr<Creep> lockedCreep_;
public:
	TowerTargetingLockOnComponent(std::shared_ptr<TowerTargetingComponent> base)
		: base_(base)
	{}

	virtual void update(CreepQueryService & service) override
	{
		if (!lockedCreep_ || !lockedCreep_->isAlive()) {
			base_->update(service);
			lockedCreep_ = base_->getTargetedCreep();
		}
		else {
			lockedCreep_ = nullptr;
		}
	}

	virtual std::shared_ptr<Creep> getTargetedCreep() const override
	{
		return lockedCreep_;
	}
};

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

//! A Tower component implementing the look of the tower.
class TowerDisplayComponent
{
public:
	virtual ~TowerDisplayComponent() {}
	virtual void render(sf::RenderTarget & target) = 0;
};

//! A simple look for the tower.
class TowerSimpleDisplayComponent final : public TowerDisplayComponent
{
private:
	sf::CircleShape baseShape_;
	sf::RectangleShape barrelShape_;
	TowerTargetingComponent & targetingComponent_;
	float angle_;
	sf::Vector2f position_;

public:
	TowerSimpleDisplayComponent(
		TowerTargetingComponent & targeting,
		sf::Vector2f position);
	virtual void render(sf::RenderTarget & target) override;
};

//! Represents a Tower placed on the grid.
class Tower final : public Selectable
{
private:
	sf::Vector2f position_;
	std::unique_ptr<TowerTargetingComponent> targetingComponent_;
	std::unique_ptr<TowerShootingComponent> shootingComponent_;
	std::unique_ptr<TowerDisplayComponent> displayComponent_;
public:
	Tower(
		sf::Vector2f position,
		std::unique_ptr<TowerTargetingComponent> targeting,
		std::unique_ptr<TowerShootingComponent> shooting,
		std::unique_ptr<TowerDisplayComponent> display)
		: position_(position)
		, targetingComponent_(std::move(targeting))
		, shootingComponent_(std::move(shooting))
		, displayComponent_(std::move(display))
	{}

	void update(BulletFactory & bulletFactory, CreepQueryService & queryService);

	inline void render(sf::RenderTarget & target)
	{
		displayComponent_->render(target);
	}

	inline sf::Vector2f getPosition() const
	{
		return position_;
	}

	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isHit(sf::Vector2f point) const;
	virtual sfg::Widget::Ptr getPanel();
};

struct towerTypeInfo_t
{
	int32_t cost;
	float range;
	std::function<std::shared_ptr<Tower>(sf::Vector2f position)> construct;
};

//! Its sole purpose is to create Turrets.
class TowerFactory
{
public:
	static towerTypeInfo_t getTowerTypeInfo(const std::string & typeName);
	static const std::vector<std::pair<std::string, towerTypeInfo_t>> & getAllTowerTypeInfos();
};

#endif // TDF_TOWER_HPP
