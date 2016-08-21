#pragma once

#ifndef TDF_TOWER_FACTORY_HPP
#define TDF_TOWER_FACTORY_HPP

#include <functional>
#include <memory>
#include <vector>
#include <SFML/System.hpp>

class Tower;
class Game;

struct towerTypeInfo_t
{
	int32_t cost;
	float range;
	std::function<std::shared_ptr<Tower>(sf::Vector2f position, Game &game__)> construct;
};

//! Its sole purpose is to create Turrets.
class TowerFactory
{
public:
	static towerTypeInfo_t getTowerTypeInfo(const std::string & typeName);
	static const std::vector<std::pair<std::string, towerTypeInfo_t>> & getAllTowerTypeInfos();
};

#endif // TDF_TOWER_FACTORY_HPP
