#pragma once

#ifndef TDF_CREEP_WALK_COMPONENT_HPP
#define TDF_CREEP_WALK_COMPONENT_HPP

#include <array>
#include <vector>
#include <SFML/System.hpp>

template<typename P> class NavigationProvider;

//! Component of Creep implementing walking logic.
class CreepWalkComponent
{
public:
	virtual ~CreepWalkComponent() {}

	//! Updates position of the Creep.
	virtual void update(sf::Time dt, NavigationProvider<sf::Vector2i> & navigation) = 0;

	virtual sf::Vector2f getPosition() const = 0;
	virtual sf::Vector2f getFacingDirection() const = 0;

	//! Returns which Turret positions are occupied by this Creep.
	virtual std::vector<sf::Vector2i> getOccupiedTurretPositions() const = 0;
	virtual bool hasReachedGoal() const = 0;
};

//! Component implementing a linear walk through the level grid.
class CreepGridWalkComponent final : public CreepWalkComponent
{
private:
	struct creepGroundPosition_t
	{
		std::array<sf::Vector2i, 2> points;
		float progress;
	};

	creepGroundPosition_t gridPosition_;
	sf::Vector2f worldPosition_;
	sf::Vector2f direction_;

public:
	CreepGridWalkComponent(sf::Vector2i initialPosition);
	virtual void update(sf::Time dt, NavigationProvider<sf::Vector2i> & navigation) override;
	virtual sf::Vector2f getPosition() const override;
	virtual sf::Vector2f getFacingDirection() const override;
	virtual std::vector<sf::Vector2i> getOccupiedTurretPositions() const override;
	virtual bool hasReachedGoal() const override;
};

#endif // TDF_CREEP_WALK_COMPONENT_HPP
