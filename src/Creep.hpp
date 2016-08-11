#pragma once

#ifndef TDF_CREEP_HPP
#define TDF_CREEP_HPP

#include <array>
#include <memory>
#include <vector>
#include <limits>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Thor/Animations.hpp>

template<typename P> class NavigationProvider;

//! Component of Creep implementing walking logic.
class CreepWalkComponent
{
public:
	virtual ~CreepWalkComponent() {}

	//! Updates position of the Creep.
	virtual void update(NavigationProvider<sf::Vector2i> & navigation) = 0;

	virtual sf::Vector2f getPosition() const = 0;
	virtual sf::Vector2f getFacingDirection() const = 0;
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
	CreepGridWalkComponent(sf::Vector2i initialPosition)
	{
		gridPosition_.points[1] = initialPosition;
		gridPosition_.progress = 1.f;
	}
	virtual void update(NavigationProvider<sf::Vector2i> & navigation) override;
	virtual sf::Vector2f getPosition() const override;
	virtual sf::Vector2f getFacingDirection() const override;
	virtual bool hasReachedGoal() const override;
};

//! Component of the Creep implementing its look.
class CreepDisplayComponent
{
public:
	virtual ~CreepDisplayComponent() {}
	virtual void render(sf::RenderTarget & target) = 0;
};

//! A very simple look for the creep.
class CreepDotDisplayComponent final : public CreepDisplayComponent
{
private:
	sf::CircleShape circle_;
	const CreepWalkComponent & walkComponent_;

public:
	CreepDotDisplayComponent(const CreepWalkComponent & walkComponent, float radius);
	virtual void render(sf::RenderTarget & target) override;
};

// TODO: Remove?
class CreepAnimationDisplayComponent final : public CreepDisplayComponent
{
private:
	sf::Sprite sprite_;
	const CreepWalkComponent & walkComponent_;

public:
	CreepAnimationDisplayComponent(sf::Sprite sprite,
									const CreepWalkComponent & walkComponent)
		: sprite_(sprite)
		, walkComponent_(walkComponent)
	{}
	virtual void render(sf::RenderTarget & target) override;
};

//! \brief Represents an enemy in the level.
//! A Creep is composed of two components: walking and display.
//! Besides forwarding some methods to the components, it also keeps
//! track of the Creep's life.
class Creep
{
private:
	std::unique_ptr<CreepWalkComponent> walkComponent_;
	std::unique_ptr<CreepDisplayComponent> displayComponent_;
	int32_t life_, maxLife_;
	int32_t bounty_;

public:
	Creep(int32_t maxLife, int32_t bounty,
		std::unique_ptr<CreepWalkComponent> walkComponent,
		std::unique_ptr<CreepDisplayComponent> displayComponent)
		: walkComponent_(std::move(walkComponent))
		, displayComponent_(std::move(displayComponent))
		, life_(maxLife), maxLife_(maxLife)
		, bounty_(bounty)
	{}

	inline void update(NavigationProvider<sf::Vector2i> & navigation)
	{
		walkComponent_->update(navigation);
	}

	inline void render(sf::RenderTarget & target)
	{
		displayComponent_->render(target);
	}

	inline sf::Vector2f getPosition() const
	{
		return walkComponent_->getPosition();
	}

	inline bool isAlive() const
	{
		return life_ > 0;
	}

	inline bool hasReachedGoal() const
	{
		return walkComponent_->hasReachedGoal();
	}

	inline void inflictDamage(int32_t damage)
	{
		life_ -= damage;
	}

	inline int32_t getBounty() const
	{
		return bounty_;
	}
};

//! Its sole function is to create creeps.
class CreepFactory
{
public:
	std::shared_ptr<Creep> createCreep(
		const std::string & typeName,
		int32_t life, int32_t bounty,
		sf::Vector2i position);
};

//! \class CreepQueryService
//! \brief Provides Towers with information about closest Creeps.
class CreepQueryService
{
public:
	virtual ~CreepQueryService() {}

	//! Returns the closest Creep to the given position, in given range.
	virtual std::shared_ptr<Creep> getClosestCreep(
		sf::Vector2f center,
		float maxRange = std::numeric_limits<float>::infinity()) = 0;
};

//! \class CreepVectorQueryService
//! \brief A trivial implementation which checks all Creeps during every query.
class CreepVectorQueryService final : public CreepQueryService
{
private:
	const std::vector<std::shared_ptr<Creep>> & creeps_;

public:
	CreepVectorQueryService(const std::vector<std::shared_ptr<Creep>> & creeps)
		: creeps_(creeps)
	{}
	virtual std::shared_ptr<Creep> getClosestCreep(
		sf::Vector2f center,
		float maxRange = std::numeric_limits<float>::infinity()) override;
};

#endif // TDF_CREEP_HPP
