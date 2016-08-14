#pragma once

#ifndef TDF_BULLET_FACTORY_HPP
#define TDF_BULLET_FACTORY_HPP

#include <memory>
#include <vector>
#include <SFML/System.hpp>

class Bullet;
class Creep;
class LevelInstance;

//! Its sole purpose is to create bullets.
class BulletFactory
{
private:
	std::shared_ptr<LevelInstance> levelInstance_;
	std::shared_ptr<Creep> target_;
	const sf::Vector2f position_;

	std::shared_ptr<Bullet> innerCreateBullet(
		const std::string & bulletName) const;

public:
	BulletFactory(
		std::shared_ptr<LevelInstance> levelInstance,
		sf::Vector2f position);

	//! Sets the Creep which will be targeted by created Bullets.
	inline void setTarget(const std::shared_ptr<Creep> & target)
	{
		target_ = target;
	}

	//! Creates bullet directed at target_.
	void createBullet(const std::string & bulletName);

	//! Alternative for BulletFactory::createBullet.
	inline void shoot(const std::string & bulletName)
	{
		return createBullet(bulletName);
	}
};

#endif // TDF_BULLET_FACTORY_HPP
