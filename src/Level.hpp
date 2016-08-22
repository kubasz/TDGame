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
#include "LevelServices.hpp"

class Level;
class LevelInstance;
class Game;

//! A factory creating Creeps and adding them to the world at appropriate times.
class InvasionManager
{
private:
	struct creationInfo_t
	{
		sf::Time moment;
		std::string creepName;
		sf::Vector2i position;
		int32_t life;
		int32_t bounty;

		inline bool operator<(const creationInfo_t & other) const
		{
			return moment < other.moment;
		}
	};

	struct wave_t
	{
		sf::Time startMoment;
		std::vector<creationInfo_t> invasionSchedule;
	};

	std::vector<wave_t> waves_;
	std::vector<sf::Vector2i> spawnPoints_;
	int32_t creepsRemaining_;
	int32_t currentWave_;
	sf::Time moment_;

public:
	InvasionManager(const nlohmann::json & data);
	InvasionManager(const InvasionManager & other) = default;

	//! Spawns creeps for the given moment of time.
	void spawn(std::shared_ptr<LevelInstance> levelInstance, sf::Time dt);

	//! Returns if all creeps were already spawned before this moment.
	bool invasionEnded() const;

	//! Returns all points on which Creeps can spawn.
	const std::vector<sf::Vector2i> & getSpawnPoints() const;

	//! Immediately sends next wave.
	void sendNextWave();

	int32_t getWaveNumber() const;
};

//! Contains static information about a level.
class Level
{
private:
	int32_t width_, height_;
	std::unique_ptr<InvasionManager> invasionManager_;
	sf::Vector2i goal_;
	int64_t startingMoney_;
	int64_t startingLives_;
	Game& game_;

public:
	Level(std::istream & source, Game &game);
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
	int64_t getStartingLives() const
	{
		return startingLives_;
	}
	const InvasionManager & cloneInvasionManager() const
	{
		return *invasionManager_.get();
	}
	bool pointLiesOnGrid(sf::Vector2i position) const
	{
		return (position.x >= 0) && (position.x < width_)
			&& (position.y >= 0) && (position.y < height_);
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
	InvasionManager invasionManager_;
	GridNavigationProvider gridNavigation_;
	GridTowerPlacementOracle gridTowerPlacement_;
	sf::Time currentTime_;
	bool wavesRunning_;
	int64_t money_;
	int64_t lives_;
	Game& game_;

public:
	LevelInstance(std::shared_ptr<Level> level, Game &game);
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

	void cheatAddMoney(int64_t amount)
	{
		money_ += amount;
	}

	int64_t getLives() const
	{
		return lives_;
	}

	void resume()
	{
		wavesRunning_ = true;
	}

	const std::vector<std::shared_ptr<Creep>> & getCreeps() const
	{
		return creeps_;
	}

	bool hasWon() const
	{
		return creeps_.empty() && invasionManager_.invasionEnded();
	}

	bool hasLost() const
	{
		return getLives() <= 0;
	}

	InvasionManager & getInvasionManager()
	{
		return invasionManager_;
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
	void sellTower(Tower* tower);

	void update(sf::Time dt);
	void render(sf::RenderTarget & target);

private:
	void renderBackground(sf::RenderTarget & target);
};

#endif // TDF_LEVEL_HPP
