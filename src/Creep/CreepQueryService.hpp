#pragma once

#ifndef TDF_CREEP_QUERY_SERVICE_HPP
#define TDF_CREEP_QUERY_SERVICE_HPP

#include <memory>
#include <vector>
#include <limits>
#include "Creep.hpp"

//! \class CreepQueryService
//! \brief Provides Towers with information about closest Creeps.
class CreepQueryService
{
public:
	virtual ~CreepQueryService() {}

	//! Returns the closest Creep to the given position, in given range.
	virtual std::shared_ptr<Creep> getClosestCreep(
		sf::Vector2f center,
		float maxRange = std::numeric_limits<float>::infinity()) = 0;
};

//! \class CreepVectorQueryService
//! \brief A trivial implementation which checks all Creeps during every query.
class CreepVectorQueryService final : public CreepQueryService
{
private:
	const std::vector<std::shared_ptr<Creep>> & creeps_;

public:
	CreepVectorQueryService(const std::vector<std::shared_ptr<Creep>> & creeps);
	virtual std::shared_ptr<Creep> getClosestCreep(
		sf::Vector2f center,
		float maxRange = std::numeric_limits<float>::infinity()) override;
};

#endif // TDF_CREEP_QUERY_SERVICE_HPP
