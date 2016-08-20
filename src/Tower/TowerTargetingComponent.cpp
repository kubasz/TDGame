#include "../Creep/Creep.hpp"
#include "../Creep/CreepQueryService.hpp"
#include "TowerTargetingComponent.hpp"

TowerClosestTargetingComponent::TowerClosestTargetingComponent(sf::Vector2f position, float range)
	: position_(position)
	, range_(range)
{}

std::shared_ptr<Creep> TowerClosestTargetingComponent::chooseCreep(CreepQueryService & service)
{
	return service.getClosestCreep(position_, range_);
}

std::shared_ptr<Creep> TowerTargetingLockOnComponent::chooseCreep(CreepQueryService & service)
{
	auto creep = base_->getTargetedCreep();
	if (!creep || !creep->isAlive()) {
		base_->update(service);
		return base_->getTargetedCreep();
	}
	else {
		return nullptr;
	}
}

TowerTargetingLockOnComponent::TowerTargetingLockOnComponent(std::shared_ptr<TowerTargetingComponent> base)
	: base_(base)
{}
