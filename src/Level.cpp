#include <algorithm>
#include <functional>
#include <utility>
#include <set>
#include <stack>
#include <json.hpp>
#include "Bullet/BulletFactory.hpp"
#include "Creep/CreepFactory.hpp"
#include "Creep/CreepQueryService.hpp"
#include "Decoration.hpp"
#include "Tower/TowerFactory.hpp"
#include "Level.hpp"

using json = nlohmann::json;

NavigationProvider<sf::Vector2i> & LevelInstance::getGoalNavigationProvider()
{
	return gridNavigation_;
}

std::shared_ptr<Tower> LevelInstance::getTowerAt(sf::Vector2i position)
{
	if (level_->pointLiesOnGrid(position))
		return towerMap_[position.y * level_->getWidth() + position.x];

	return nullptr;
}

InvasionManager::InvasionManager(const json & data)
	: creepsRemaining_(0)
	, currentWave_(-1)
{
	auto cmpPoints = [](const sf::Vector2i & a, const sf::Vector2i & b) -> bool {
		if (a.x == b.x)
			return a.y < b.y;
		return a.x < b.x;
	};

	std::set<sf::Vector2i, decltype(cmpPoints)> knownSpawnPoints(cmpPoints);

	// We are interested in the "waves" table
	for (const auto & wave : data["waves"]) {
		wave_t waveData;
		waveData.startMoment = sf::seconds((double)wave["start-time"]);

		// Calculate spawn moments for the given description
		for (const auto & creepDesc : wave["creeps"]) {
			std::vector<sf::Time> moments;
			const std::string type = creepDesc["type"];
			const int32_t hp = creepDesc["hp"];
			const int32_t bounty = creepDesc["bounty"];
			const sf::Vector2i spawnAt = {
				creepDesc["spawn-at"][0], creepDesc["spawn-at"][1]
			};
			knownSpawnPoints.insert(spawnAt);

			auto addInfo = [&](sf::Time waveTime) {
				waveData.invasionSchedule.push_back({
					waveTime, type, spawnAt, hp, bounty
				});
				++creepsRemaining_;
			};

			const auto & spawnTime = creepDesc["spawn-time"];
			if (spawnTime.is_object()) {
				// We have a range of spawn moments
				const auto start = sf::seconds((double)spawnTime["start"]);
				const auto interval = sf::seconds((double)spawnTime["interval"]);
				const int64_t count = spawnTime["count"];
				for (int64_t i = 0; i < count; ++i)
					addInfo(start + (sf::Int64)i * interval);
			}
			else if (spawnTime.is_array()) {
				for (const auto & time : spawnTime)
					addInfo(sf::seconds((double)time));
			}
			else if (spawnTime.is_number_float()) {
				addInfo(sf::seconds((double)spawnTime));
			}
			else {
				throw std::runtime_error("Invalid type of spawnTime");
			}
		}

		std::sort(waveData.invasionSchedule.begin(), waveData.invasionSchedule.end());
		waves_.emplace_back(std::move(waveData));
	}

	std::copy(knownSpawnPoints.begin(), knownSpawnPoints.end(),
		std::back_inserter(spawnPoints_));
}

void InvasionManager::spawn(std::shared_ptr<LevelInstance> levelInstance, sf::Time dt)
{
	// Damn you, c++
	creationInfo_t start, end;

	int32_t waveID = 0;
	for (const auto & wave : waves_) {
		if (wave.startMoment > moment_ + dt)
			break; // We can omit later waves

		currentWave_ = std::max(currentWave_, waveID);
		start.moment = moment_ - wave.startMoment;
		end.moment = moment_ + dt - wave.startMoment;

		const auto from = std::lower_bound(wave.invasionSchedule.begin(), wave.invasionSchedule.end(), start);
		const auto to = std::lower_bound(wave.invasionSchedule.begin(), wave.invasionSchedule.end(), end);

		for (auto it = from; it != to; ++it) {
			levelInstance->createCreepAt(it->creepName, it->life, it->bounty, it->position);
			--creepsRemaining_;
		}

		++waveID;
	}

	moment_ += dt;
}

bool InvasionManager::invasionEnded() const
{
	return creepsRemaining_ == 0;
}

const std::vector<sf::Vector2i> & InvasionManager::getSpawnPoints() const
{
	return spawnPoints_;
}

void InvasionManager::sendNextWave()
{
	const auto nextWave = currentWave_ + 1;
	if (nextWave >= (int32_t)waves_.size())
		return;
	
	const sf::Time delta = waves_[nextWave].startMoment - moment_;
	for (uint32_t i = nextWave; i < waves_.size(); ++i)
		waves_[i].startMoment -= delta;

	currentWave_ = nextWave;
}

int32_t InvasionManager::getWaveNumber() const
{
	return currentWave_;
}

Level::Level(std::istream & source, Game &game) : game_(game)
{
	json levelDescription;
	source >> levelDescription;

	width_ = levelDescription["grid-size"][0];
	height_ = levelDescription["grid-size"][1];
	goal_ = { levelDescription["goal"][0], levelDescription["goal"][1] };
	startingMoney_ = levelDescription["starting-money"];
	startingLives_ = levelDescription["starting-lives"];

	invasionManager_.reset(new InvasionManager(levelDescription));
}

LevelInstance::LevelInstance(std::shared_ptr<Level> level, Game & game)
	: level_(level)
	, towerMap_(new std::shared_ptr<Tower>[level->getWidth() * level->getHeight()])
	, invasionManager_(level_->cloneInvasionManager())
	, gridNavigation_(*this, level->getGoal())
	, gridTowerPlacement_(*this)
	, wavesRunning_(false)
	, money_(level->getStartingMoney())
	, lives_(level->getStartingLives())
	, game_(game)
{
	for (auto source : invasionManager_.getSpawnPoints())
		decorations_.push_back(std::make_shared<CreepSourceDecoration>(sf::Vector2f(source)));
	decorations_.push_back(std::make_shared<GoalDecoration>(sf::Vector2f(level->getGoal())));

	for (const auto & decoration : decorations_)
		renderables_.push_back(decoration);
}

std::shared_ptr<Selectable> LevelInstance::selectAt(sf::Vector2f position)
{
	for (auto & tower : towers_) {
		if (tower->isHit(position))
			return tower;
	}

	for (auto & creep : creeps_) {
		if (creep->isHit(position))
			return creep;
	}

	return nullptr;
}

bool LevelInstance::createTowerAt(const std::string & name, sf::Vector2i position)
{
	if (!level_->pointLiesOnGrid(position))
		return false;
	
	const auto typeInfo = TowerFactory::getTowerTypeInfo(name);
	if (typeInfo.cost > money_)
		return false;
	
	auto tower = typeInfo.construct({ (float)position.x, (float)position.y }, game_);

	towers_.push_back(tower);
	renderables_.push_back(tower);
	towerMap_[position.y * level_->getWidth() + position.x] = tower;

	gridNavigation_.update();
	gridTowerPlacement_.updateTowerRestrictions();
	money_ -= typeInfo.cost;

	return true;
}

void LevelInstance::createCreepAt(const std::string & name, int32_t life, int32_t bounty, sf::Vector2i position)
{
	auto creep = CreepFactory().createCreep(name, life, bounty, position, game_);
	creeps_.push_back(creep);
	renderables_.push_back(creep);
}

void LevelInstance::registerBullet(std::shared_ptr<Bullet> bullet)
{
	bullets_.push_back(bullet);
	renderables_.push_back(bullet);
}

// TODO: Move somewhere else?
template<typename T, typename F>
static void removeFromVectorIf(std::vector<T> & v, F f)
{
	auto it = std::remove_if(v.begin(), v.end(), f);
	v.resize(std::distance(v.begin(), it));
}

void LevelInstance::update(sf::Time dt)
{
	if (wavesRunning_)
		invasionManager_.spawn(shared_from_this(), dt);

	for (auto & decoration : decorations_)
		decoration->update(dt);

	CreepVectorQueryService queryService(creeps_);

	for (auto & tower : towers_) {
		BulletFactory factory(shared_from_this(), tower->getPosition());
		tower->update(dt, factory, queryService);
	}

	for (auto & bullet : bullets_)
		bullet->update(dt);
	removeFromVectorIf(bullets_, [&](const std::shared_ptr<Bullet> & b) {
		return !b->isAlive();
	});

	for (auto & creep : creeps_) {
		creep->update(dt, gridNavigation_);
		if (!creep->isAlive())
			money_ += creep->getBounty();
		else if (creep->hasReachedGoal())
			lives_--;
	}
	removeFromVectorIf(creeps_, [&](const std::shared_ptr<Creep> & c) {
		return !c->isAlive() || c->hasReachedGoal();
	});

	if (wavesRunning_)
		currentTime_ += dt;
	
	gridTowerPlacement_.updateCreepRestrictions();
}

void LevelInstance::render(sf::RenderTarget & target)
{
	gridTowerPlacement_.render(target);

	// Remove all expired weak_ptrs
	removeFromVectorIf(renderables_, std::mem_fn(&std::weak_ptr<Renderable>::expired));
	for (auto & renderable : renderables_)
		renderable.lock()->render(target);
}
