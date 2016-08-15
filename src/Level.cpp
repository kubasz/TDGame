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
{
	auto cmpPoints = [](const sf::Vector2i & a, const sf::Vector2i & b) -> bool {
		if (a.x == b.x)
			return a.y < b.y;
		return a.x < b.x;
	};

	std::set<sf::Vector2i, decltype(cmpPoints)> knownSpawnPoints(cmpPoints);

	// We are interested in the "waves" table
	for (const auto & wave : data["waves"]) {
		const int64_t startFrame = wave["start-frame"];

		// Calculate spawn moments for the given description
		for (const auto & creepDesc : wave["creeps"]) {
			std::vector<int64_t> moments;
			const std::string type = creepDesc["type"];
			const int32_t hp = creepDesc["hp"];
			const int32_t bounty = creepDesc["bounty"];
			const sf::Vector2i spawnAt = {
				creepDesc["spawn-at"][0], creepDesc["spawn-at"][1]
			};
			knownSpawnPoints.insert(spawnAt);

			auto addInfo = [&](int64_t waveTime) {
				invasionPlan_.push_back({
					startFrame + waveTime,
					type, spawnAt, hp, bounty
				});
			};

			const auto & spawnTime = creepDesc["spawn-time"];
			if (spawnTime.is_object()) {
				// We have a range of spawn moments
				const int64_t start = spawnTime["start"];
				const int64_t interval = spawnTime["interval"];
				const int64_t count = spawnTime["count"];
				for (int64_t i = 0; i < count; ++i)
					addInfo(start + i * interval);
			}
			else if (spawnTime.is_array()) {
				for (const auto & time : spawnTime)
					addInfo((int64_t)time);
			}
			else if (spawnTime.is_number_integer()) {
				addInfo((int64_t)spawnTime);
			}
			else {
				throw std::runtime_error("Invalid type of spawnTime");
			}
		}
	}

	std::sort(invasionPlan_.begin(), invasionPlan_.end());
	std::copy(knownSpawnPoints.begin(), knownSpawnPoints.end(),
		std::back_inserter(spawnPoints_));
}

void InvasionManager::spawn(std::shared_ptr<LevelInstance> levelInstance, int64_t moment)
{
	// Damn you, c++
	creationInfo_t dummy;
	dummy.moment = moment;

	const auto its = std::equal_range(invasionPlan_.begin(), invasionPlan_.end(), dummy);
	for (auto it = its.first; it != its.second; ++it) {
		levelInstance->createCreepAt(it->creepName, it->life, it->bounty, it->position);
	}
}

bool InvasionManager::invasionEnded(int64_t moment) const
{
	return invasionPlan_.empty() || (invasionPlan_.back().moment < moment);
}

const std::vector<sf::Vector2i> & InvasionManager::getSpawnPoints() const
{
	return spawnPoints_;
}

Level::Level(std::istream & source)
{
	json levelDescription;
	source >> levelDescription;

	width_ = levelDescription["grid-size"][0];
	height_ = levelDescription["grid-size"][1];
	goal_ = { levelDescription["goal"][0], levelDescription["goal"][1] };
	startingMoney_ = levelDescription["starting-money"];

	invasionManager_.reset(new InvasionManager(levelDescription));
}

LevelInstance::LevelInstance(std::shared_ptr<Level> level)
	: level_(level)
	, towerMap_(new std::shared_ptr<Tower>[level->getWidth() * level->getHeight()])
	, gridNavigation_(*this, level->getGoal())
	, gridTowerPlacement_(*this)
	, currentFrame_(0)
	, wavesRunning_(false)
	, money_(level->getStartingMoney())
{
	for (auto source : level->getInvasionManager().getSpawnPoints())
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

	return nullptr;
}

bool LevelInstance::createTowerAt(const std::string & name, sf::Vector2i position)
{
	if (!level_->pointLiesOnGrid(position))
		return false;
	
	const auto typeInfo = TowerFactory::getTowerTypeInfo(name);
	if (typeInfo.cost > money_)
		return false;

	auto tower = typeInfo.construct({ (float)position.x, (float)position.y });

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
	auto creep = CreepFactory().createCreep(name, life, bounty, position);
	creeps_.push_back(creep);
	renderables_.push_back(creep);
}

void LevelInstance::registerBullet(std::shared_ptr<Bullet> bullet)
{
	bullets_.push_back(bullet);
	renderables_.push_back(bullet);
}

template<typename Entity>
static void removeDeadEntities(std::vector<std::shared_ptr<Entity>> & entities)
{
	auto remover = [&](const std::shared_ptr<Entity> & entity)
	{
		return !entity->isAlive();
	};

	auto it = std::remove_if(entities.begin(), entities.end(), remover);
	entities.resize(std::distance(entities.begin(), it));
}

template<typename Entity>
static void removeDeadEntities(std::vector<std::weak_ptr<Entity>> & entities)
{
	auto it = std::remove_if(entities.begin(), entities.end(),
		std::mem_fn(&std::weak_ptr<Entity>::expired));
	entities.resize(std::distance(entities.begin(), it));
}

void LevelInstance::update()
{
	if (wavesRunning_)
		level_->getInvasionManager().spawn(shared_from_this(), currentFrame_);

	for (auto & decoration : decorations_)
		decoration->update();

	CreepVectorQueryService queryService(creeps_);

	for (auto & tower : towers_) {
		BulletFactory factory(shared_from_this(), tower->getPosition());
		tower->update(factory, queryService);
	}

	for (auto & bullet : bullets_)
		bullet->update();
	removeDeadEntities(bullets_);

	for (auto & creep : creeps_) {
		creep->update(gridNavigation_);
		if (!creep->isAlive())
			money_ += creep->getBounty();
	}
	removeDeadEntities(creeps_);

	if (wavesRunning_)
		++currentFrame_;
	
	gridTowerPlacement_.updateCreepRestrictions();
}

void LevelInstance::render(sf::RenderTarget & target)
{
	gridTowerPlacement_.render(target);

	removeDeadEntities(renderables_);
	for (auto & renderable : renderables_)
		renderable.lock()->render(target);
}
