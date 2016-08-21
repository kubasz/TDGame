#pragma once

#ifndef TDF_TOWER_HPP
#define TDF_TOWER_HPP

#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "../Selectable.hpp"
#include "../Renderable.hpp"
#include "TowerDisplayComponent.hpp"
#include "TowerShootingComponent.hpp"
#include "TowerTargetingComponent.hpp"

class Upgradeable
{
public:
	virtual ~Upgradeable() {}
	virtual unsigned int maxUpgradeState() const = 0;
	virtual unsigned int currentUpgradeState() const = 0;
	virtual unsigned int currentUpgradeCost() const = 0;
	virtual void upgrade() = 0;
};

class BulletFactory;
class CreepQueryService;

//! Represents a Tower placed on the grid.
class Tower final : public Selectable, public Renderable
{
private:
	sf::Vector2f position_;
	int sellCost_;
	std::unique_ptr<TowerTargetingComponent> targetingComponent_;
	std::unique_ptr<TowerShootingComponent> shootingComponent_;
	std::unique_ptr<TowerDisplayComponent> displayComponent_;
public:
	Tower(
		sf::Vector2f position,
		int sellCost,
		std::unique_ptr<TowerTargetingComponent> targeting,
		std::unique_ptr<TowerShootingComponent> shooting,
		std::unique_ptr<TowerDisplayComponent> display)
		: position_(position)
		, sellCost_(sellCost)
		, targetingComponent_(std::move(targeting))
		, shootingComponent_(std::move(shooting))
		, displayComponent_(std::move(display))
	{}

	void update(sf::Time dt, BulletFactory & bulletFactory, CreepQueryService & queryService);

	virtual void render(sf::RenderTarget & target) override;
	sf::Vector2f getPosition() const;

	virtual bool isHit(sf::Vector2f point) const override;
	virtual sfg::Widget::Ptr getPanel(std::shared_ptr<LevelInstance> levelInstance) override;

	int getSellCost()
	{
		return sellCost_;
	}
};

#endif // TDF_TOWER_HPP
