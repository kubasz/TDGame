#include "CreepWalkComponent.hpp"
#include "../Level.hpp"

CreepGridWalkComponent::CreepGridWalkComponent(sf::Vector2i initialPosition)
{
	gridPosition_.points[0] = initialPosition;
	gridPosition_.points[1] = initialPosition;
	gridPosition_.progress = 1.f;
}

void CreepGridWalkComponent::update(sf::Time dt, NavigationProvider<sf::Vector2i> & navigation, float speedBuff)
{
	if(speedBuff < -59.f)
	{
		speedBuff = -59.f;
	}
	sf::Vector2i &p0 = gridPosition_.points[0], &p1 = gridPosition_.points[1];
	float & progress = gridPosition_.progress;

	progress += dt.asSeconds() + speedBuff*dt.asSeconds()/60.0f;
	while (progress > 1.f) {
		p0 = p1;
		p1 = navigation.getNextStep(p1);
		progress -= 1.f;
	}

	const sf::Vector2f v0 = { (float)p0.x, (float)p0.y };
	const sf::Vector2f v1 = { (float)p1.x, (float)p1.y };

	worldPosition_ = (1.f - progress) * v0 + progress * v1;
	direction_ = v1 - v0;
}

sf::Vector2f CreepGridWalkComponent::getPosition() const
{
	return worldPosition_;
}

sf::Vector2f CreepGridWalkComponent::getFacingDirection() const
{
	return direction_;
}

std::vector<sf::Vector2i> CreepGridWalkComponent::getOccupiedTurretPositions() const
{
	return { gridPosition_.points[0], gridPosition_.points[1] };
}

bool CreepGridWalkComponent::hasReachedGoal() const
{
	return gridPosition_.points[0] == gridPosition_.points[1];
}
