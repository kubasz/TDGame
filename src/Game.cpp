#include <exception>
#include <fstream>
#include <iostream>
#include <SFGUI/Renderers.hpp>

#include "MakeUnique.hpp"
#include "Constants.hpp"
#include "Game.hpp"
#include "GameStates/GameState.hpp"
#include "GameStates/MenuGameState.hpp"

#ifdef _MSC_VER
#include <conio.h>
#define WAIT_ANY_KEY _getch()
#else
#include <cstdio>
#define WAIT_ANY_KEY getchar()
#endif

Game::Game(int /*argc*/, char ** /*argv*/)
{
	srand((unsigned int)time(nullptr));

	sfg::Renderer::Set(sfg::VertexArrayRenderer::Create());

	const sf::Uint32 style = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;
	window_.create(sf::VideoMode(800, 600), "TDFramework", style, sf::ContextSettings(0, 0, 8));
	if (!window_.isOpen())
		throw std::runtime_error("Failed to create window");

	window_.resetGLStates();
	window_.setFramerateLimit(60);
	window_.setActive();

	loadResources();

	setNextState(std::make_unique<MenuGameState>(*this));
}

Game::~Game()
{}

void Game::loadResources()
{
	loadTextures();
	loadSounds();
	loadAnimations();
}

void Game::loadTextures()
{
	//loading textures into resources holder
	textures_holder_.acquire("Tower",
		thor::Resources::fromFile <sf::Texture>("data/Images/Tower/tower.png"));
	textures_holder_.acquire("TowerHead",
		thor::Resources::fromFile <sf::Texture>("data/Images/Tower/tower_head.png"));

	textures_holder_.acquire("Wall",
		thor::Resources::fromFile <sf::Texture>("data/Images/Wall/wall.png"));
	textures_holder_.acquire("Creep",
		thor::Resources::fromFile <sf::Texture>("data/Images/Creep/creep.png"));
	textures_holder_.acquire("Floor",
		thor::Resources::fromFile <sf::Texture>("data/Images/Floor/floor.png"));
}

void Game::loadSounds()
{
	sounds_holder_.acquire("Tower",
		thor::Resources::fromFile <sf::SoundBuffer>("data/Audio/tower.ogg"));
	sounds_holder_.acquire("Laser",
		thor::Resources::fromFile <sf::SoundBuffer>("data/Audio/laser.ogg"));
}

void Game::loadAnimation(const std::string& path, const std::string& id)
{
	thor::FrameAnimation tmp;
	sf::IntRect rect;
	int n, x1, x2, y1, y2;

	std::ifstream fin(path.c_str());
	assert(fin.is_open());

	fin>>n;

	for(int i = 0; i < n; i++){
		fin>>x1>>y1>>x2>>y2;

		rect = sf::IntRect(x1, y1, x2, y2);
		tmp.addFrame(1.f/(float)n, rect);
	}

	fin.close();

	auto fun = [=](){
		return std::make_unique<thor::FrameAnimation>(tmp);
	};

	thor::ResourceLoader<thor::FrameAnimation> loaderTmp(fun, id);
	loaderTmp.load();

	animations_holder_.acquire(id,
		loaderTmp);
}

void Game::loadAnimations()
{
	//loading animations into resources holder
	loadAnimation("data/Images/Creep/creep.txt", "Creep");
}

void Game::setNextState(std::unique_ptr<GameState> gameState)
{
	nextState_ = std::move(gameState);
}

int Game::run()
{
	while (window_.isOpen()) {
		if (nextState_)
			currentState_ = std::move(nextState_);

		sf::Event evt;
		if (window_.pollEvent(evt)) {
			switch (evt.type) {
			case sf::Event::Closed:
				window_.close();
				break;
			default:
				if (currentState_)
					currentState_->handleEvent(evt);
				break;
			}
		}
		else {
			if (currentState_)
				currentState_->update(sf::seconds(Constants::SECONDS_PER_FRAME));
			if (currentState_)
				currentState_->render(window_);
			sfgui_.Display(window_);
			window_.display();
		}
	}

	return 0;
}

void Game::quitNextFrame()
{
	window_.close();
}

int Game::getWidth() const
{
	return window_.getSize().x;
}

int Game::getHeight() const
{
	return window_.getSize().y;
}

const sf::Texture& Game::getTexture(const std::string &id) const
{
	return textures_holder_[id];
}

const sf::SoundBuffer& Game::getSound(const std::string &id) const
{
	return sounds_holder_[id];
}

const thor::FrameAnimation& Game::getAnimation(const std::string &id) const
{
	return animations_holder_[id];
}

int main(int argc, char ** argv)
{
	try {
		return Game(argc, argv).run();
	}

	catch (std::runtime_error & err) {
		std::cout << "Runtime error: " << err.what() << std::endl;
		//WAIT_ANY_KEY;
		return 1;
	}
}
