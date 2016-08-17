#pragma once

#ifndef TDF_CREEP_LIFE_DISPLAY_COMPONENT
#define TDF_CREEP_LIFE_DISPLAY_COMPONENT

#include "CreepDisplayComponent.hpp"

class Creep;
class CreepWalkComponent;

class CreepLifeDisplayComponent final : public CreepDisplayComponent
{
private:
	Creep * owner_;
	sf::RectangleShape shape_;
	sf::Vector2f size_;
	bool hideOnFull_;

public:
	CreepLifeDisplayComponent(sf::Vector2f offset, sf::Vector2f size, bool hideOnFull);
	void setOwner(Creep * owner);
	virtual void render(sf::RenderTarget & target) override;
};

#endif // TDF_CREEP_LIFE_DISPLAY_COMPONENT
