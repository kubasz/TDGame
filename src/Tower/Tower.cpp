#include <SFGUI/Widgets.hpp>
#include "../Bullet/Bullet.hpp"
#include "../Bullet/BulletFactory.hpp"
#include "../Level.hpp"
#include "Tower.hpp"

void Tower::update(sf::Time dt, BulletFactory & bulletFactory, CreepQueryService & queryService)
{
	targetingComponent_->update(queryService);
	auto target = targetingComponent_->getTargetedCreep();
	if (target) {
		bulletFactory.setTarget(target);
		shootingComponent_->update(dt, bulletFactory);
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

sfg::Widget::Ptr Tower::getPanel(std::shared_ptr<LevelInstance> levelInstance)
{
	auto label = sfg::Label::Create("Tower #" + std::to_string((intptr_t)this));

	auto sellButton = sfg::Button::Create("Sell for " + std::to_string(sellCost_));
	sellButton->GetSignal(sfg::Button::OnLeftClick).Connect([this, levelInstance]() {
		levelInstance->sellTower(this);
	});

	auto layout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	layout->PackEnd(label, false);
	layout->PackEnd(sellButton, false);
	return layout;
}
