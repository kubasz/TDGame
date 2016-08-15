#include <memory>
#include <json.hpp>
#include "Tower.hpp"
#include "TowerDisplayComponent.hpp"
#include "TowerShootingComponent.hpp"
#include "TowerTargetingComponent.hpp"
#include "TowerFactory.hpp"

using json = nlohmann::json;

static std::vector<std::pair<std::string, towerTypeInfo_t>> typeInfos = []()
{
	std::vector<std::pair<std::string, towerTypeInfo_t>> ret;

	ret.push_back({ "GenericTower", {
		35, // Cost
		3.f, // Range (in game units)
		[](sf::Vector2f position) -> std::shared_ptr<Tower> {
			auto targeting = std::unique_ptr<TowerClosestTargetingComponent>(
				new TowerClosestTargetingComponent(position));
			auto shooting = std::unique_ptr<TowerLinearShootingComponent>(
				new TowerLinearShootingComponent(1.f));
			auto display = std::unique_ptr<TowerSimpleDisplayComponent>(
				new TowerSimpleDisplayComponent(*targeting.get(), position));
			return std::make_shared<Tower>(
				position,
				std::move(targeting),
				std::move(shooting),
				std::move(display));
		}
	}});

	return ret;
}();

towerTypeInfo_t TowerFactory::getTowerTypeInfo(const std::string & typeName)
{
	for (const auto & p : typeInfos) {
		if (p.first == typeName)
			return p.second;
	}

	throw std::runtime_error("Unknown Tower type: " + typeName);
}

const std::vector<std::pair<std::string, towerTypeInfo_t>> & TowerFactory::getAllTowerTypeInfos()
{
	return typeInfos;
}
