#include <SFGUI/Widgets.hpp>
#include "../Bullet/Bullet.hpp"
#include "../Bullet/BulletFactory.hpp"
#include "Tower.hpp"

void Tower::update(BulletFactory & bulletFactory, CreepQueryService & queryService)
{
	targetingComponent_->update(queryService);
	auto target = targetingComponent_->getTargetedCreep();
	if (target) {
		bulletFactory.setTarget(target);
		shootingComponent_->update(bulletFactory);
	}
}

void Tower::render(sf::RenderTarget & target)
{
	displayComponent_->render(target);
}

sf::Vector2f Tower::getPosition() const
{
	return position_;
}

bool Tower::isHit(sf::Vector2f point) const
{
	const sf::FloatRect rect = {
		position_.x - 0.5f, position_.y - 0.5f,
		1.f, 1.f
	};
	return rect.contains(point);
}

sfg::Widget::Ptr Tower::getPanel()
{
	return sfg::Label::Create("Tower #" + std::to_string((intptr_t)this));
}
