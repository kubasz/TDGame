#include <cassert>
#include "Creep.hpp"
#include "CreepLifeDisplayComponent.hpp"

CreepLifeDisplayComponent::CreepLifeDisplayComponent(
	sf::Vector2f offset, sf::Vector2f size, bool hideOnFull)
	: size_(size)
	, hideOnFull_(hideOnFull)
{
	owner_ = nullptr;
	shape_.setOrigin(0.5f * size - offset);
	shape_.setFillColor(sf::Color::Red);
}

void CreepLifeDisplayComponent::setOwner(Creep * owner)
{
	owner_ = owner;
}

void CreepLifeDisplayComponent::render(sf::RenderTarget & target)
{
	assert(owner_ != nullptr);

	const auto life = owner_->getLife();
	const auto maxLife = owner_->getMaxLife();

	if (hideOnFull_ && (life == maxLife))
		return;

	const float percentage = (float)life / (float)maxLife;

	shape_.setPosition(owner_->getPosition()
		+ sf::Vector2f(0.5f * size_.x * percentage, 0.f));
	shape_.setSize({ size_.x * percentage, size_.y });
	target.draw(shape_);
}
