#define _USE_MATH_DEFINES
#include <cmath>
#include "Decoration.hpp"

static const float ROTATION_SPEED = 45.f;
static const float PULSE_SPEED = 2.f;
static const float MAX_RADIUS = 0.45f;
static const float MIN_RADIUS = 0.1f;

CreepSourceDecoration::CreepSourceDecoration(sf::Vector2f position)
{
	rectangleShape_.setPosition(position);
	rectangleShape_.setSize({ 0.5f, 0.5f });
	rectangleShape_.setOrigin({ 0.25f, 0.25f });
	rectangleShape_.setFillColor(sf::Color(255, 127, 0));
}

void CreepSourceDecoration::update(sf::Time dt)
{
	rectangleShape_.rotate(dt.asSeconds() * ROTATION_SPEED);
}

void CreepSourceDecoration::render(sf::RenderTarget & target)
{
	target.draw(rectangleShape_);
}

GoalDecoration::GoalDecoration(sf::Vector2f position)
	: phase_(0.f)
{
	circleShape_.setPosition(position);
	circleShape_.setFillColor(sf::Color::Cyan);
}

void GoalDecoration::update(sf::Time dt)
{
	phase_ += dt.asSeconds() * PULSE_SPEED;
	if (phase_ > M_PI)
		phase_ -= 2.f * M_PI;
}

void GoalDecoration::render(sf::RenderTarget & target)
{
	const float radius = 
		(MAX_RADIUS + MIN_RADIUS) * 0.5f
		+ sin(phase_) * (MAX_RADIUS - MIN_RADIUS) * 0.5f;
	circleShape_.setRadius(radius);
	circleShape_.setOrigin({ radius, radius });
	target.draw(circleShape_);
}
