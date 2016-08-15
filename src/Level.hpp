#pragma once

#ifndef TDF_LEVEL_HPP
#define TDF_LEVEL_HPP

#include <cstdint>
#include <istream>
#include <memory>
#include <vector>

#include <json.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Renderable.hpp"
#include "Bullet/Bullet.hpp"
#include "Creep/Creep.hpp"
#include "Decoration.hpp"
#include "Tower/Tower.hpp"

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
	std::unique_ptr<bool[]> validTurretPlaces_;
	std::unique_ptr<int32_t[]> parents_;
	std::unique_ptr<int32_t[]> pre_;
	std::unique_ptr<int32_t[]> low_;

public:
	GridTowerPlacementOracle(LevelInstance & levelInstance);
	virtual bool canPlaceTowerHere(const sf::Vector2i & at) const override;

	//! Draws places suitable for Tower placement.
	void render(sf::RenderTarget & target);

	//! Updates tower placement info.
	void update();
};

//! A factory creating Creeps and adding them to the world at appropriate times.
class InvasionManager
{
private:
	struct creationInfo_t
	{
		int64_t moment;
		std::string creepName;
		sf::Vector2i position;
		int32_t life;
		int32_t bounty;

		inline bool operator<(const creationInfo_t & other) const
		{
			return moment < other.moment;
		}
	};

	std::vector<creationInfo_t> invasionPlan_;
	std::vector<sf::Vector2i> spawnPoints_;

public:
	InvasionManager(const nlohmann::json & data);

	//! Spawns creeps for the given moment of time.
	void spawn(std::shared_ptr<LevelInstance> levelInstance, int64_t moment);

	//! Returns if all creeps were already spawned before this moment.
	bool invasionEnded(int64_t moment) const;

	//! Returns all points on which Creeps can spawn.
	const std::vector<sf::Vector2i> & getSpawnPoints() const;
};

//! Contains static information about a level.
class Level
{
private:
	int32_t width_, height_;
	std::unique_ptr<InvasionManager> invasionManager_;
	sf::Vector2i goal_;
	int64_t startingMoney_;

public:
	Level(std::istream & source);
	int32_t getWidth() const
	{
		return width_;
	}
	int32_t getHeight() const
	{
		return height_;
	}
	sf::Vector2i getGoal() const
	{
		return goal_;
	}
	int64_t getStartingMoney() const
	{
		return startingMoney_;
	}
	InvasionManager & getInvasionManager()
	{
		return *invasionManager_.get();
	}
};

class Creep;

//! A class responsible for keeping and updating simulation state
//! of a level and its entities.
class LevelInstance : public std::enable_shared_from_this<LevelInstance>
{
private:
	std::shared_ptr<Level> level_;
	std::unique_ptr<std::shared_ptr<Tower>[]> towerMap_;
	std::vector<std::shared_ptr<Bullet>> bullets_;
	std::vector<std::shared_ptr<Creep>> creeps_;
	std::vector<std::shared_ptr<Decoration>> decorations_;
	std::vector<std::shared_ptr<Tower>> towers_;
	std::vector<std::weak_ptr<Renderable>> renderables_;
	GridNavigationProvider gridNavigation_;
	GridTowerPlacementOracle gridTowerPlacement_;
	int64_t currentFrame_;
	bool wavesRunning_;
	int64_t money_;

public:
	LevelInstance(std::shared_ptr<Level> level);
	NavigationProvider<sf::Vector2i> & getGoalNavigationProvider();
	std::shared_ptr<Level> getLevel() const
	{
		return level_;
	}
	std::shared_ptr<Tower> getTowerAt(sf::Vector2i position);
	std::shared_ptr<Tower> getTowerAt(int x, int y)
	{
		return getTowerAt({ x, y });
	}

	bool canPlaceTowerHere(const sf::Vector2i & at) const
	{
		return gridTowerPlacement_.canPlaceTowerHere(at);
	}

	int64_t getMoney() const
	{
		return money_;
	}

	void startWaves()
	{
		wavesRunning_ = true;
	}

	//! Returns a control widget for an object selected by mouse position.
	std::shared_ptr<Selectable> selectAt(sf::Vector2f position);

	//! \brief Creates a tower at given position.
	//! Returns if there was enough cash to instantiate the tower.
	bool createTowerAt(const std::string & name, sf::Vector2i position);
	void createCreepAt(
		const std::string & name,
		int32_t life, int32_t bounty,
		sf::Vector2i position);
	void registerBullet(std::shared_ptr<Bullet> bullet);

	void update();
	void render(sf::RenderTarget & target);
};

#endif // TDF_LEVEL_HPP
