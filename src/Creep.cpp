#include <functional>
#include "Creep.hpp"
#include "Level.hpp"

// TODO
static const float WALK_SPEED = 1.f / 60.f;

void CreepGridWalkComponent::update(NavigationProvider<sf::Vector2i> & navigation)
{
	sf::Vector2i &p0 = gridPosition_.points[0], &p1 = gridPosition_.points[1];
	float & progress = gridPosition_.progress;

	progress += WALK_SPEED;
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

bool CreepGridWalkComponent::hasReachedGoal() const
{
	return gridPosition_.points[0] == gridPosition_.points[1];
}

CreepDotDisplayComponent::CreepDotDisplayComponent(
	const CreepWalkComponent & walkComponent,
	float radius)
	: circle_(radius)
	, walkComponent_(walkComponent)
{
	circle_.setFillColor(sf::Color::Black);
	circle_.setOrigin(radius, radius);
}

void CreepDotDisplayComponent::render(sf::RenderTarget & target)
{
	circle_.setPosition(walkComponent_.getPosition());
	target.draw(circle_);
}

void CreepAnimationDisplayComponent::render(sf::RenderTarget & target)
{

}

std::shared_ptr<Creep> CreepFactory::createCreep(
	const std::string & typeName,
	int32_t life, int32_t bounty,
	sf::Vector2i position)
{
	if (typeName == "GenericCreep") {
		auto walk = std::make_unique<CreepGridWalkComponent>(position);
		auto display = std::make_unique<CreepDotDisplayComponent>(*walk.get(), 0.125f);
		return std::make_shared<Creep>(life, bounty, std::move(walk), std::move(display));
	}

	throw std::runtime_error("Unknown Creep type: " + typeName);
}

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
