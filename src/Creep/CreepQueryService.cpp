#include "CreepQueryService.hpp"

CreepVectorQueryService::CreepVectorQueryService(const std::vector<std::shared_ptr<Creep>> & creeps)
	: creeps_(creeps)
{}

std::shared_ptr<Creep> CreepVectorQueryService::getClosestCreep(
	sf::Vector2f center,
	float maxRange)
{
	if (creeps_.empty())
		return nullptr;

	auto sqDistanceTo = [&](std::shared_ptr<Creep> creep) -> float
	{
		auto d = creep->getPosition() - center;
		return d.x * d.x + d.y * d.y;
	};

	std::shared_ptr<Creep> closest = nullptr;
	float smallestDistance = maxRange * maxRange;

	for (const auto & creep : creeps_) {
		const float newDistance = sqDistanceTo(creep);
		if (smallestDistance > newDistance) {
			smallestDistance = newDistance;
			closest = creep;
		}
	}

	return closest;
}
