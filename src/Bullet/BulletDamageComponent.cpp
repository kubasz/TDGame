#include "BulletDamageComponent.hpp"
#include "../Creep/Creep.hpp"

BulletSimpleDamageComponent::BulletSimpleDamageComponent(int32_t damage)
	: damage_(damage)
{
}

void BulletSimpleDamageComponent::damage(std::shared_ptr<Creep> creep) const
{
	creep->inflictDamage(damage_);
}

BulletBuffDamageComponent::BulletBuffDamageComponent(CreepBuff buff)
	: buff_(buff)
{
}

void BulletBuffDamageComponent::damage(std::shared_ptr<Creep> creep) const
{
	creep->applyBuff(buff_);
}
