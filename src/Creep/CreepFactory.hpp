#pragma once

#ifndef TDF_CREEP_FACTORY_HPP
#define TDF_CREEP_FACTORY_HPP

//! Its sole function is to create creeps.
class CreepFactory
{
public:
	std::shared_ptr<Creep> createCreep(
		const std::string & typeName,
		int32_t life, int32_t bounty,
		sf::Vector2i position);
};

#endif // TDF_CREEP_FACTORY_HPP
