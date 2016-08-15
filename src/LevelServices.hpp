#pragma once

#ifndef TDF_LEVEL_SERVICES_HPP
#define TDF_LEVEL_SERVICES_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Level;
class LevelInstance;

//! Provides Creeps with path finding information.
template<typename P>
class NavigationProvider
{
public:
	virtual ~NavigationProvider() {}

	// Returns the goal to which the creep walks.
	virtual P getGoal() const = 0;

	// Returns next point on the grid, following the given point.
	virtual P getNextStep(const P & point) const = 0;
};

class GridNavigationProvider final : public NavigationProvider<sf::Vector2i>
{
private:
	LevelInstance & levelInstance_;
	std::unique_ptr<int32_t[]> path_;
	sf::Vector2i goal_;

public:
	GridNavigationProvider(LevelInstance & levelInstance, sf::Vector2i goal);
	virtual sf::Vector2i getGoal() const override;
	virtual sf::Vector2i getNextStep(const sf::Vector2i & point) const override;

	//! Updates navigation info.
	void update();
};

//! Tells if a tower can be placed at given position
template<typename P>
class TowerPlacementOracle
{
public:
	virtual ~TowerPlacementOracle() {}
	virtual bool canPlaceTowerHere(const P & at) const = 0;
};

class GridTowerPlacementOracle final : public TowerPlacementOracle<sf::Vector2i>
{
private:
	LevelInstance & levelInstance_;
	std::unique_ptr<bool[]> permanentlyOccupied_;
	std::unique_ptr<bool[]> validTurretPlaces_;
	std::unique_ptr<bool[]> occupiedByCreeps_;
	std::unique_ptr<int32_t[]> parents_;
	std::unique_ptr<int32_t[]> pre_;
	std::unique_ptr<int32_t[]> low_;

public:
	GridTowerPlacementOracle(LevelInstance & levelInstance);
	virtual bool canPlaceTowerHere(const sf::Vector2i & at) const override;

	//! Draws places suitable for Tower placement.
	void render(sf::RenderTarget & target);

	//! Updates information about positions restricted by Towers.
	void updateTowerRestrictions();

	//! Updates information about positions restricted by Creeps.
	void updateCreepRestrictions();
};

#endif // TDF_LEVEL_SERVICES_HPP
