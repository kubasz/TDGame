#pragma once

#ifndef TDF_RENDERABLE_HPP
#define TDF_RENDERABLE_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

//! A base class for a component which can be rendered.
class Renderable
{
public:
	virtual ~Renderable() {}
	virtual void render(sf::RenderTarget & target) = 0;
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
};

#endif // TDF_RENDERABLE_HPP
