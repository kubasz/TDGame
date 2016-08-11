#pragma once

#ifndef TDF_SELECTABLE_HPP
#define TDF_SELECTABLE_HPP

#include <SFML/System.hpp>
#include <SFGUI/Widget.hpp>

//! \class Selectable
//! \brief A selectable entity interface.
//! Subclassed and registered entities can be selected by clicking on
//! them during gameplay. For example, this allows for tower upgrading
//! and viewing enemy stats.
class Selectable
{
public:
	virtual ~Selectable() {}

	//! \brief Returns a rectangle for broad-phase collision checking.
	virtual sf::FloatRect getBoundingRect() const = 0;

	//! \brief Precise mouse collision check.
	//! \param point Position of the mouse, in absolute coordinates.
	virtual bool isHit(sf::Vector2f point) const = 0;

	//! \brief Returns a panel with settings for the given object.
	virtual sfg::Widget::Ptr getPanel() = 0;
};

#endif // TDF_SELECTABLE_HPP
