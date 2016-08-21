#pragma once

#ifndef TDF_CREEP_HPP
#define TDF_CREEP_HPP

#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../Game.hpp"
#include "../Selectable.hpp"
#include "../Renderable.hpp"
#include "CreepDisplayComponent.hpp"
#include "CreepWalkComponent.hpp"
#include "Buff.hpp"

class Game;

//! \brief Represents an enemy in the level.
//! A Creep is composed of two components: walking and display.
//! Besides forwarding some methods to the components, it also keeps
//! track of the Creep's life.
class Creep : public Selectable, public Renderable
{
private:
	std::unique_ptr<CreepWalkComponent> walkComponent_;
	std::unique_ptr<CreepDisplayComponent> displayComponent_;
	int32_t life_, maxLife_;
	int32_t bounty_;
	sf::Sound sound_;
	std::vector<CreepBuff> buffs_;

public:
	Creep(int32_t maxLife, int32_t bounty,
		std::unique_ptr<CreepWalkComponent> walkComponent,
		std::unique_ptr<CreepDisplayComponent> displayComponent, Game &game);

	inline void applyBuff(CreepBuff buff)
	{
		buffs_.push_back(buff);
	}

	inline float queryBuff(CreepBuff::Type type)
	{
		float accum = 0.0f;
		for(const CreepBuff& buff: buffs_)
		{
			if(buff.type == type)
				accum += buff.strength;
		}
		return accum;
	}

	inline void update(sf::Time dt, NavigationProvider<sf::Vector2i> & navigation)
	{
		// update buffs
		for(CreepBuff& buff: buffs_)
		{
			buff.duration -= dt.asMicroseconds() * 1e-6;
		}
		std::remove_if(buffs_.begin(), buffs_.end(), [](const CreepBuff& buff){return buff.duration <= 0.0;});
		// update walk
		walkComponent_->update(dt, navigation, queryBuff(CreepBuff::Type::BUFF_SPEED));
	}

	inline virtual void render(sf::RenderTarget & target) override
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

	inline int32_t getLife() const
	{
		return life_;
	}

	inline int32_t getMaxLife() const
	{
		return maxLife_;
	}

	inline bool hasReachedGoal() const
	{
		return walkComponent_->hasReachedGoal();
	}

	inline int32_t getBounty() const
	{
		return bounty_;
	}

	inline std::vector<sf::Vector2i> getOccupiedTurretPositions() const
	{
		return walkComponent_->getOccupiedTurretPositions();
	}

	void inflictDamage(int32_t damage);

	virtual bool isHit(sf::Vector2f point) const override;
	virtual sfg::Widget::Ptr getPanel() override;
};

#endif // TDF_CREEP_HPP
