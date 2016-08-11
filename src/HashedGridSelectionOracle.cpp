#include "SelectionOracle.hpp"
#include "Selectable.hpp"

void HashedGridSelectionOracle::addObject(const std::shared_ptr<Selectable> & object)
{
	const auto rect = object->getBoundingRect();
	int startX = (int)floor(rect.left), startY = (int)floor(rect.top);
	int endX = (int)ceil(rect.left + rect.width);
	int endY = (int)ceil(rect.top + rect.height);

	for (int y = startY; y < endY; y++) {
		for (int x = startX; x < endX; x++) {
			hashMap_.insert({ {x, y}, object });
		}
	}
}

std::vector<std::shared_ptr<Selectable>>
	HashedGridSelectionOracle::getCollidingObjects(sf::Vector2f point) const
{
	std::vector<std::shared_ptr<Selectable>> ret;
	const auto itPair = hashMap_.equal_range(
		{ (int)floor(point.x), (int)floor(point.y) });

	for (auto it = itPair.first; it != itPair.second; ++it) {
		const auto rect = it->second->getBoundingRect();
		if (rect.contains(point) && it->second->isHit(point))
			ret.emplace_back(std::move(it->second));
	}

	return ret;
}
