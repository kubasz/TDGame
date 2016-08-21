#ifdef __linux
#include <limits.h>
#endif
#if defined(WIN32) || defined(WIN64)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <dirent.h>
#include <exception>
#include <fstream>
#include <vector>
#include <utility>
#include "../MakeUnique.hpp"
#include "../Game.hpp"
#include "LevelGameState.hpp"
#include "LevelSelectGameState.hpp"
#include "../ScopeGuard.hpp"

// TODO: Implement GetFullPathName equivalent for Linux

LevelSelectGameState::LevelSelectGameState(Game & game)
	: game_(game)
{
	guiMainWindow_ = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
	guiDesktop_.SetProperty( "*", "FontName", "data/FiraSans-Regular.ttf" );
	guiDesktop_.Add(guiMainWindow_);

	setPath("data");
}

void LevelSelectGameState::setPath(const std::string & path)
{
	auto layout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	DIR * dirp = opendir(path.c_str());
	if (!dirp)
		return;

	// This calls closedir(dirp) on function exit (also when exception is thrown)
	ScopeGuard dirpGuard(std::bind(closedir, dirp));

	std::vector<std::pair<bool, std::string>> entries;
	std::shared_ptr<std::string> pathShared = std::make_shared<std::string>(path);

	dirent * dent;
	while ((dent = readdir(dirp)))
		entries.push_back({ dent->d_type != DT_DIR, dent->d_name });
	std::sort(entries.begin(), entries.end());

	for (const auto & p : entries) {
		auto button = sfg::Button::Create(p.second);
		button->GetSignal(sfg::Button::OnLeftClick).Connect([this, p, pathShared]() {
			const auto path = *pathShared.get() + "/" + p.second;
			if (p.first) {
				// This is a file
				try {
					std::ifstream source(path);
					game_.setNextState(std::make_unique<LevelGameState>(game_, source));
				}
				catch (std::logic_error&) {
					popupLevelReadError();
				}
			}
			else {
				// This is a directory
				setPath(path);
			}
		});

		layout->PackEnd(button);
	}

	auto guiScrolledWindow = sfg::ScrolledWindow::Create();
	guiScrolledWindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS);
	guiScrolledWindow->AddWithViewport(layout);

	guiNextLayout_ = std::move(guiScrolledWindow);
}

void LevelSelectGameState::update(sf::Time dt)
{
	if (guiNextLayout_) {
		guiMainWindow_->RemoveAll();
		guiMainWindow_->Add(guiNextLayout_);
		guiNextLayout_ = nullptr;

		handleResize(game_.getWidth(), game_.getHeight());
	}

	guiDesktop_.Update(dt.asSeconds());
}

void LevelSelectGameState::render(sf::RenderTarget & target)
{
	target.clear();
}

void LevelSelectGameState::handleEvent(const sf::Event & evt)
{
	if (evt.type == sf::Event::Resized)
		handleResize(evt.size.width, evt.size.height);
	guiDesktop_.HandleEvent(evt);
}

void LevelSelectGameState::handleResize(int width, int height)
{
	guiMainWindow_->SetAllocation({ 0.f, 0.f, (float)width, (float)height });
}

void LevelSelectGameState::popupLevelReadError()
{
	if (guiErrorPopup_.expired()) {
		auto window = sfg::Window::Create();
		window->SetTitle(L"Level reading error");

		auto text = sfg::Label::Create("An error occured when trying to read the level!");
		auto button = sfg::Button::Create("OK");
		button->GetSignal(sfg::Button::OnLeftClick).Connect([&]() {
			auto locked = guiErrorPopup_.lock();
			if (locked)
				guiDesktop_.Remove(locked);
		});

		auto layout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
		layout->PackEnd(text);
		layout->PackEnd(button);
		window->Add(layout);

		guiDesktop_.Add(window);
		guiErrorPopup_ = window;
	}
	guiDesktop_.BringToFront(guiErrorPopup_.lock());
}
