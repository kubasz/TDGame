#include <memory>
#include <string>
#include "../MakeUnique.hpp"
#include "Creep.hpp"
#include "CreepFactory.hpp"
#include "CreepLifeDisplayComponent.hpp"

std::shared_ptr<Creep> CreepFactory::createCreep(
	const std::string & typeName,
	int32_t life, int32_t bounty,
	sf::Vector2i position, Game &game)
{
	if (typeName == "GenericCreep") {
		auto walk = std::make_unique<CreepGridWalkComponent>(position);
		auto dotDisplay = std::make_unique<CreepDotDisplayComponent>(*walk.get(), 0.125f,
			game.getTexture("Creep"));

		auto lifeDisplay = std::make_unique<CreepLifeDisplayComponent>(
			sf::Vector2f(0.f, 0.5f), sf::Vector2f(0.8f, 0.2f), true);
		auto lifeDisplayDirect = lifeDisplay.get();

		auto compositeDisplay = std::make_unique<CreepCompositeDisplayComponent>();
		compositeDisplay->addChild(std::move(dotDisplay));
		compositeDisplay->addChild(std::move(lifeDisplay));

		auto ret = std::make_shared<Creep>(life, bounty, std::move(walk), std::move(compositeDisplay));
		lifeDisplayDirect->setOwner(ret.get());
		return ret;
	}

	throw std::runtime_error("Unknown Creep type: " + typeName);
}
