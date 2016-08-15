#include "../MakeUnique.hpp"
#include "../Constants.hpp"
#include "../Game.hpp"
#include "LevelSelectGameState.hpp"
#include "MenuGameState.hpp"

MenuGameState::MenuGameState(Game & game)
	: game_(game)
{
	auto guiChooseLevel = sfg::Button::Create("Choose level");
	guiChooseLevel->GetSignal(sfg::Button::OnLeftClick).Connect([this]() {
		// game_.setNextState(GameState::Identifier::LEVEL);
		game_.setNextState(std::make_unique<LevelSelectGameState>(game_));
	});

	auto guiExit = sfg::Button::Create("Exit");
	guiExit->GetSignal(sfg::Button::OnLeftClick).Connect([&]() {
		game.quitNextFrame();
	});

	auto guiTable = sfg::Table::Create();
	guiTable->Attach(guiChooseLevel, { 0, 0, 1, 1 });
	guiTable->Attach(guiExit, { 0, 1, 1, 1 });

	auto guiWindow = sfg::Window::Create();
	guiWindow->SetTitle(L"Menu");
	guiWindow->Add(guiTable);

	guiDesktop_.Add(guiWindow);
}

void MenuGameState::update()
{
	guiDesktop_.Update(Constants::FPS);
}

void MenuGameState::render(sf::RenderTarget & target)
{
	target.clear();
}

void MenuGameState::handleEvent(const sf::Event & evt)
{
	guiDesktop_.HandleEvent(evt);
}
