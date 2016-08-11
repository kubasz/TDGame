#pragma once

#ifndef TDF_SELECTION_ORACLE_HPP
#define TDF_SELECTION_ORACLE_HPP

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <SFML/System.hpp>

class Selectable;

//! \class SelectionOracle
//! \brief Class for answering selection queries
class SelectionOracle
{
public:
	virtual ~SelectionOracle() {}

	//! \brief Adds an object to the structure.
	//! \param object The object to add.
	virtual void addObject(const std::shared_ptr<Selectable> & object) = 0;

	//! \brief Returns all objects with object rectangles colliding with given point.
	//! \param point Position the returned objects collide with.
	virtual std::vector<std::shared_ptr<Selectable>>
		getCollidingObjects(sf::Vector2f point) const = 0;
};

//! \class HashedGridSelectionOracle
//! \brief Implementation based on a hash map. Probably could better with a grid.
class HashedGridSelectionOracle final : public SelectionOracle
{
private:
	// TODO: Move to some utilities
	template<typename A, typename B>
	struct pair_hash
	{
		// TODO: Insert a large prime here
		inline size_t operator()(const std::pair<A, B> & p) const
		{
			return 0x412F53AB * std::hash<A>()(p.first) + std::hash<B>()(p.second);
		}
	};

	std::unordered_multimap<
		std::pair<int, int>,
		std::shared_ptr<Selectable>,
		pair_hash<int, int>
	> hashMap_;

public:
	virtual void addObject(const std::shared_ptr<Selectable> & object) override;
	virtual std::vector<std::shared_ptr<Selectable>>
		getCollidingObjects(sf::Vector2f point) const override;
};

#endif // TDF_SELECTION_ORACLE_HPP
