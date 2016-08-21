#include <memory>
#include <json.hpp>
#include "../MakeUnique.hpp"
#include "Tower.hpp"
#include "TowerDisplayComponent.hpp"
#include "TowerShootingComponent.hpp"
#include "TowerTargetingComponent.hpp"
#include "TowerFactory.hpp"

using json = nlohmann::json;

static std::vector<std::pair<std::string, towerTypeInfo_t>> typeInfos_ = []()
{
	std::vector<std::pair<std::string, towerTypeInfo_t>> ret;

	ret.push_back({ "Tower", {
		35, // Cost
		[](sf::Vector2f position, Game & game__) -> std::shared_ptr<Tower> {
			auto targeting = std::make_unique<TowerClosestTargetingComponent>(position, 3.f);
			auto shooting = std::make_unique<TowerLinearShootingComponent>(1.f, "GenericBullet");
			auto display = std::make_unique<TowerTargettingDisplayComponent>(*targeting.get(),
				position, game__.getTexture("Tower"));
			return std::make_shared<Tower>(
				position,
				35,
				std::move(targeting),
				std::move(shooting),
				std::move(display));
		}}});

	ret.push_back({ "LongRangeTower", {
			50, // Cost
			[](sf::Vector2f position, Game & game__) -> std::shared_ptr<Tower> {
				auto targeting = std::make_unique<TowerClosestTargetingComponent>(position, 8.f);
				auto shooting = std::make_unique<TowerLinearShootingComponent>(2.f, "GenericBullet");
				auto display = std::make_unique<TowerTargettingDisplayComponent>(*targeting.get(),
                    position, game__.getTexture("Tower"));
				return std::make_shared<Tower>(
						position,
						50,
						std::move(targeting),
						std::move(shooting),
						std::move(display));
			}
	}});

	ret.push_back({ "LaserTower", {
			100, // Cost
			[](sf::Vector2f position, Game & game__) -> std::shared_ptr<Tower> {
				auto targeting = std::make_unique<TowerClosestTargetingComponent>(position, 10.f);
				auto shooting = std::make_unique<TowerLinearShootingComponent>(0.25f, "LaserBullet");
				auto display = std::make_unique<TowerTargettingDisplayComponent>(*targeting.get(),
				    position, game__.getTexture("Tower"));
				return std::make_shared<Tower>(
						position,
						100,
						std::move(targeting),
						std::move(shooting),
						std::move(display));
			}
	}});

	ret.push_back({ "Wall", {
			20, // Cost
			[](sf::Vector2f position, Game & game__) -> std::shared_ptr<Tower> {
				auto targeting = std::make_unique<TowerTargetingComponent>();
				auto shooting = std::make_unique<TowerShootingComponent>();
				auto display = std::make_unique<TowerSimpleDisplayComponent>(position, game__.getTexture("Wall"));
				return std::make_shared<Tower>(
						position,
						20,
						std::move(targeting),
						std::move(shooting),
						std::move(display));
			}
	}});

	return ret;
}();

towerTypeInfo_t TowerFactory::getTowerTypeInfo(const std::string & typeName)
{
	for (const auto & p : typeInfos_) {
		if (p.first == typeName)
			return p.second;
	}

	throw std::runtime_error("Unknown Tower type: " + typeName);
}

const std::vector<std::pair<std::string, towerTypeInfo_t>> & TowerFactory::getAllTowerTypeInfos()
{
	return typeInfos_;
}
