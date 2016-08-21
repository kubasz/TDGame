#include <SFGUI/Widgets.hpp>
#include <cmath>
#include "Creep.hpp"

Creep::Creep(
	int32_t maxLife, int32_t bounty,
	std::unique_ptr<CreepWalkComponent> walkComponent,
	std::unique_ptr<CreepDisplayComponent> displayComponent, Game &game)
	: walkComponent_(std::move(walkComponent))
	, displayComponent_(std::move(displayComponent))
	, life_(maxLife), maxLife_(maxLife)
	, bounty_(bounty)
{
	sound_.setBuffer(game.getSound("Creep"));
}

bool Creep::isHit(sf::Vector2f point) const
{
	return displayComponent_->isHit(point);
}

sfg::Widget::Ptr Creep::getPanel()
{
	return sfg::Label::Create("Creep #" + std::to_string((intptr_t)this));
}

void Creep::inflictDamage(int32_t damage)
{
	float vuln = queryBuff(CreepBuff::Type::BUFF_VULNERABILITY)/100.f + 1.f;
	life_ -= int(round(damage*vuln));
	if(life_<0)
	{
		life_ = 0;
	}
	sound_.play();
}
