#pragma once

#ifndef TDF_BULLET_DAMAGE_COMPONENT_HPP
#define TDF_BULLET_DAMAGE_COMPONENT_HPP

#include <memory>
#include "../Creep/Buff.hpp"

class Creep;

//! A component for Bullet implementing the damage
class BulletDamageComponent {
public:
	virtual void damage(std::shared_ptr<Creep> creep) const = 0;
};

//! Bullet which does normal damage
class BulletSimpleDamageComponent final : public BulletDamageComponent
{
private:
	int32_t damage_;

public:
	BulletSimpleDamageComponent(int32_t damage);
	virtual void damage(std::shared_ptr<Creep> creep) const override;
};

//! Bullet which gives buffs
class BulletBuffDamageComponent final : public BulletDamageComponent
{
private:
	CreepBuff buff_;

public:
	BulletBuffDamageComponent(CreepBuff buff);
	virtual void damage(std::shared_ptr<Creep> creep) const override;
};

#endif // TDF_BULLET_DAMAGE_COMPONENT_HPP
