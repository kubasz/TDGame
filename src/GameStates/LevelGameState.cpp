#include <fstream>
#include "../Constants.hpp"
#include "../Level.hpp"
#include "../Creep/Creep.hpp"
#include "../Tower/Tower.hpp"
#include "../Tower/TowerFactory.hpp"
#include "../Game.hpp"
#include "LevelGameState.hpp"
#include "MenuGameState.hpp"

static const int RIGHT_PANEL_WIDTH = 200;

LevelGameState::LevelGameState(Game & game, std::istream & source)
	: game_(game)
	, level_(std::make_shared<Level>(source))
	, levelInstance_(new LevelInstance(level_))
	, oldCash_(-1)
	, isPlacingTower_(false)
{
	guiCashLabel_ = sfg::Label::Create();
	guiCashLabel_->SetRequisition({ 0.f, 16.f });
	guiCashLabel_->SetAlignment({ 0.f, 0.f });

	guiGameStartButton_ = sfg::Button::Create("Send creeps");
	guiGameStartButton_->GetSignal(sfg::Button::OnLeftClick).Connect([this]() {
		levelInstance_->startWaves();
		guiGameStartButton_->SetState(sfg::Widget::State::INSENSITIVE);
	});

	guiInfoPanelLocation_ = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	auto guiMainLayout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	guiMainLayout->PackEnd(guiCashLabel_, false);
	createTowerCreationButtons(guiMainLayout);
	guiMainLayout->PackEnd(guiGameStartButton_, false);
	guiMainLayout->PackEnd(guiInfoPanelLocation_, false);

	guiMainWindow_ = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
	guiMainWindow_->Add(guiMainLayout);
	guiDesktop_.Add(guiMainWindow_);

	handleResize(game.getWidth(), game.getHeight());
	centerView();
}

void LevelGameState::createTowerCreationButtons(const sfg::Box::Ptr & layout)
{
	for (const auto & p : TowerFactory::getAllTowerTypeInfos()) {
		const auto & towerName = p.first;
		const auto & typeInfo = p.second;
		const std::string buttonText = "Create " + towerName
			+ " (" + std::to_string(typeInfo.cost) + ")";

		auto button = sfg::Button::Create(buttonText);
		button->GetSignal(sfg::Button::OnLeftClick).Connect([this, towerName]() {
			isPlacingTower_ = true;
			placedTowerTypeName_ = towerName;
		});

		layout->PackEnd(button, false);
	}
}

void LevelGameState::centerView()
{
	levelView_.setCenter({
		(float)level_->getWidth() / 2.f,
		(float)level_->getHeight() / 2.f
	});
}

void LevelGameState::handleResize(int width, int height)
{
	// Set a view which excludes area claimed by UI
	const auto targetSize = sf::Vector2i(width, height);
	const int levelWidth = targetSize.x - RIGHT_PANEL_WIDTH;
	const sf::FloatRect levelScaledRect = {
		0.f, 0.f,
		float(levelWidth) / float(targetSize.x),
		1.f
	};

	windowSize_ = { width, height };

	levelView_.setSize((float)levelWidth, (float)targetSize.y);
	levelView_.setViewport(levelScaledRect);
	levelView_.zoom(1.f / 32.f);

	guiMainWindow_->SetAllocation({
		(float)levelWidth, 0.f,
		(float)RIGHT_PANEL_WIDTH, (float)targetSize.y
	});
}

void LevelGameState::handleClick(sf::Vector2i position)
{
	if (isPlacingTower_ && levelInstance_->canPlaceTowerHere(hoveredTile_)) {
		levelInstance_->createTowerAt(placedTowerTypeName_, hoveredTile_);
		isPlacingTower_ = false;
	}
	else if (!isPlacingTower_) {
		selectedObject_ = levelInstance_->selectAt(lastMouseLevelPosition_);

		guiInfoPanelLocation_->RemoveAll();
		if (selectedObject_)
			guiInfoPanelLocation_->PackEnd(selectedObject_->getPanel(), true);
	}
}

void LevelGameState::handleKeyPress(sf::Keyboard::Key key)
{
	switch (key) {
	case sf::Keyboard::Left:
		levelView_.move({ -1.f, 0.f });
		break;
	case sf::Keyboard::Right:
		levelView_.move({ 1.f, 0.f });
		break;
	case sf::Keyboard::Up:
		levelView_.move({ 0.f, -1.f });
		break;
	case sf::Keyboard::Down:
		levelView_.move({ 0.f, 1.f });
		break;
	default:
		break;
	}
}

void LevelGameState::createWonPopup()
{
	guiWonWindow_ = sfg::Window::Create();
	guiWonWindow_->SetTitle("Success!");

	auto label = sfg::Label::Create("You won!");
	auto button = sfg::Button::Create("OK");
	button->GetSignal(sfg::Button::OnLeftClick).Connect([&]() {
		game_.setNextState(std::make_unique<MenuGameState>(game_));
	});

	auto layout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	layout->PackEnd(label, false);
	layout->PackEnd(button, true);

	guiWonWindow_->Add(layout);
	guiDesktop_.Add(guiWonWindow_);
}

void LevelGameState::update()
{
	guiDesktop_.Update(Constants::FPS);
	levelInstance_->update();

	auto newCash = levelInstance_->getMoney();
	if (oldCash_ != newCash) {
		oldCash_ = newCash;
		guiCashLabel_->SetText("Cash: " + std::to_string(newCash));
	}

	// Check if the game has been won
	if (levelInstance_->hasWon() && !guiWonWindow_)
		createWonPopup();
}

void LevelGameState::render(sf::RenderTarget & target)
{
	target.clear(sf::Color::White);

	const auto oldView = target.getView();

	// Push/pop is needed on my machine, without it the UI
	// is incorrectly scaled for some reason
	target.pushGLStates();
	target.setView(levelView_);

	lastMouseLevelPosition_ = target.mapPixelToCoords(lastMousePosition_);
	hoveredTile_ = {
		(int)round(lastMouseLevelPosition_.x),
		(int)round(lastMouseLevelPosition_.y)
	};

	levelInstance_->render(target);

	if (isPlacingTower_) {
		sf::RectangleShape rs;
		if (levelInstance_->canPlaceTowerHere(hoveredTile_))
			rs.setFillColor(sf::Color(0, 255, 0, 127));
		else
			rs.setFillColor(sf::Color(255, 0, 0, 127));
		rs.setSize({ 1.f, 1.f });
		rs.setPosition((float)hoveredTile_.x - 0.5f, (float)hoveredTile_.y - 0.5f);
		target.draw(rs);
	}

	target.popGLStates();
	target.setView(oldView);
}

void LevelGameState::handleEvent(const sf::Event & evt)
{
	if (evt.type == sf::Event::Resized)
		handleResize(evt.size.width, evt.size.height);

	if (evt.type == sf::Event::MouseMoved) {
		lastMousePosition_.x = evt.mouseMove.x;
		lastMousePosition_.y = evt.mouseMove.y;
	}

	if (evt.type == sf::Event::MouseButtonPressed) {
		if (evt.mouseButton.x < windowSize_.x - RIGHT_PANEL_WIDTH) {
			handleClick({ evt.mouseButton.x, evt.mouseButton.y });
		}
	}

	if (evt.type == sf::Event::KeyPressed)
		handleKeyPress(evt.key.code);
	
	if ((evt.type == sf::Event::KeyPressed) && (evt.key.code == sf::Keyboard::Escape)) {
		isPlacingTower_ = false;
		selectedObject_ = nullptr;
		guiInfoPanelLocation_->RemoveAll();
	}

	guiDesktop_.HandleEvent(evt);
}
