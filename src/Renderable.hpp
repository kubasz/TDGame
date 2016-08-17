#pragma once

#ifndef TDF_RENDERABLE_HPP
#define TDF_RENDERABLE_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

//! A base class for a component which can be rendered, and hit-tested.
class Renderable
{
public:
	virtual ~Renderable() {}
	virtual void render(sf::RenderTarget & target) = 0;
	virtual bool isHit(sf::Vector2f /*point*/) const
	{
		return false;
	}
};

//! A Renderable which is composed of other renderables.
template<typename T /* : public Renderable */>
class CompositeRenderable : public T
{
private:
	std::vector<std::unique_ptr<T>> children_;

public:
	CompositeRenderable()
	{}

	void addChild(std::unique_ptr<T> child)
	{
		children_.emplace_back(std::move(child));
	}

	virtual void render(sf::RenderTarget & target) override
	{
		for (auto & child : children_)
			child->render(target);
	}

	virtual bool isHit(sf::Vector2f point) const override
	{
		for (const auto & child : children_) {
			if (child->isHit(point))
				return true;
		}

		return false;
	}
};

#endif // TDF_RENDERABLE_HPP
