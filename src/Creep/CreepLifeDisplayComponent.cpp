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
	shape_.setFillColor(sf::Color::Green);
	backgroundShape_.setOrigin(0.5f * size - offset);
	backgroundShape_.setFillColor(sf::Color::Blue);
}

void CreepLifeDisplayComponent::setOwner(Creep * owner)
{
	owner_ = owner;
}

void CreepLifeDisplayComponent::render(sf::RenderTarget & target)
{
	assert(owner_ != nullptr);
	hideOnFull_ = false;

	const auto life = std::max(owner_->getLife(), 0);
	const auto maxLife = owner_->getMaxLife();

	if (hideOnFull_ && (life == maxLife))
		return;

	const float percentage = (float)life / (float)maxLife;

	shape_.setPosition(owner_->getPosition());
	shape_.setSize({ size_.x * percentage, size_.y });
	backgroundShape_.setPosition(owner_->getPosition()
				- sf::Vector2f(0.1f, 0.1f));
	backgroundShape_.setSize({ size_.x + 0.2f , size_.y + 0.2f });
	target.draw(backgroundShape_);
	target.draw(shape_);
}
