#include <queue>
#include "Level.hpp"
#include "LevelServices.hpp"

GridNavigationProvider::GridNavigationProvider(
	LevelInstance & levelInstance,
	sf::Vector2i goal)
	: levelInstance_(levelInstance)
	, goal_(goal)
{
	const auto & level = levelInstance.getLevel();
	const int32_t tableSize = level->getWidth() * level->getHeight();
	path_ = std::unique_ptr<int32_t[]>(new int32_t[tableSize]);

	// Calculate paths for the first time
	update();
}

sf::Vector2i GridNavigationProvider::getGoal() const
{
	return goal_;
}

sf::Vector2i GridNavigationProvider::getNextStep(const sf::Vector2i & point) const
{
	assert(levelInstance_.getLevel()->pointLiesOnGrid(point));

	const int32_t fromIndex = point.y * levelInstance_.getLevel()->getWidth() + point.x;
	const int32_t toIndex = path_[fromIndex];
	const int32_t width = levelInstance_.getLevel()->getWidth();

	return{ toIndex % width, toIndex / width };
}

void GridNavigationProvider::update()
{
	// A simple BFS algorithm creating BFS-tree.
	// For small maps it should suffice.
	static const int32_t EMPTY = -1, FILLED = -2;
	const int32_t width = levelInstance_.getLevel()->getWidth();
	const int32_t height = levelInstance_.getLevel()->getHeight();
	const int32_t tableSize = width * height;
	std::queue<std::pair<int32_t, int32_t>> verts;

	// Prepare tables
	std::fill(path_.get(), path_.get() + tableSize, EMPTY);
	const int32_t goalIndex = goal_.y * width + goal_.x;
	verts.emplace(goalIndex, goalIndex);

	// Reserve locations occupied by towers
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (levelInstance_.getTowerAt({ x, y })) {
				path_[y * width + x] = FILLED;
			}
		}
	}

	do
	{
		const int32_t current = verts.front().first;
		const int32_t previous = verts.front().second;
		verts.pop();

		path_[current] = previous;

		const int32_t x = current % width;
		const int32_t y = current / width;

		auto tryPushVertex = [&](int32_t next)
		{
			if (next != previous && path_[next] == EMPTY) {
				verts.emplace(next, current);
			}
		};

		if (y < height - 1)
			tryPushVertex(current + width);
		if (y > 0)
			tryPushVertex(current - width);
		if (x < width - 1)
			tryPushVertex(current + 1);
		if (x > 0)
			tryPushVertex(current - 1);

	} while (!verts.empty());
}

GridTowerPlacementOracle::GridTowerPlacementOracle(LevelInstance & levelInstance)
	: levelInstance_(levelInstance)
{
	auto level = levelInstance.getLevel();
	const int32_t tableSize = level->getWidth() * level->getHeight();
	permanentlyOccupied_.reset(new bool[tableSize]);
	validTurretPlaces_.reset(new bool[tableSize]);
	occupiedByCreeps_.reset(new bool[tableSize]);
	parents_.reset(new int32_t[tableSize]);
	pre_.reset(new int32_t[tableSize]);
	low_.reset(new int32_t[tableSize]);
	dp_.reset(new int32_t[tableSize]);

	const int32_t width = levelInstance_.getLevel()->getWidth();
	std::fill(permanentlyOccupied_.get(), permanentlyOccupied_.get() + tableSize, false);

	// Forbid placing on goals, as it may be a non-cut point
	const auto goal = levelInstance_.getLevel()->getGoal();
	const auto goalIndex = goal.y * width + goal.x;
	permanentlyOccupied_[goalIndex] = true;

	// Forbid placing on spawn points
	for (const auto & spawnPoint : levelInstance_.getInvasionManager().getSpawnPoints())
		permanentlyOccupied_[spawnPoint.y * width + spawnPoint.x] = true;

	updateTowerRestrictions();
	updateCreepRestrictions();
}

bool GridTowerPlacementOracle::canPlaceTowerHere(const sf::Vector2i & at) const
{
	const auto width = levelInstance_.getLevel()->getWidth();

	if (levelInstance_.getLevel()->pointLiesOnGrid(at)) {
		const auto index = at.y * width + at.x;
		return !permanentlyOccupied_[index] && validTurretPlaces_[index] && !occupiedByCreeps_[index];
	}

	return false;
}

void GridTowerPlacementOracle::updateTowerRestrictions()
{
	// The algorithm marks cut points, creep sources and the goal
	// as unsuitable to place a tower on. Every other point is marked as
	// suitable. It uses program-stack based dfs, so on big maps there
	// might be a stack overflow.
	// TODO: This algorithm marks invalid points too eagerly, as some
	// cut points are still valid turret placement points
	// (e.g. dead-end corridors leading neither to a source nor the goal).
	static const int32_t EMPTY = -1;
	static const int32_t ROOT = -2;
	const int32_t width = levelInstance_.getLevel()->getWidth();
	const int32_t height = levelInstance_.getLevel()->getHeight();
	const int32_t tableSize = width * height;
	int32_t preCounter = 0;
	
	// First, calculate low and pre numbers
	std::fill(parents_.get(), parents_.get() + tableSize, EMPTY);

	std::function<void(int32_t, int32_t, int32_t, int32_t)> dfs =
		[&](int32_t current, int32_t x, int32_t y, int32_t parent) {

		int32_t minimum = preCounter++;
		pre_[current] = minimum;
		parents_[current] = parent;
		dp_[current] = permanentlyOccupied_[current] || occupiedByCreeps_[current];

		auto processChild = [&](int32_t child, int32_t nx, int32_t ny) {
			if (child != parent && !levelInstance_.getTowerAt(nx, ny)) {
				int32_t v;
				if (parents_[child] == EMPTY) {
					dfs(child, nx, ny, current);
					dp_[current] += dp_[child];
					v = low_[child];
				}
				else
					v = pre_[child];
				minimum = std::min(minimum, v);
			}

		};

		if (y < height - 1)
			processChild(current + width, x, y + 1);
		if (y > 0)
			processChild(current - width, x, y - 1);
		if (x < width - 1)
			processChild(current + 1, x + 1, y);
		if (x > 0)
			processChild(current - 1, x - 1, y);

		low_[current] = minimum;
	};

	const auto goal = levelInstance_.getLevel()->getGoal();
	const auto goalIndex = goal.y * width + goal.x;
	dfs(goalIndex, goal.x, goal.y, ROOT);

	// Now see which vertices are cut points.
	// Note we don't process root differently, as it never is a valid place
	// for a turret
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			const int32_t current = y * width + x;

			if (levelInstance_.getTowerAt({ x, y })) {
				validTurretPlaces_[current] = false;
				continue;
			}

			auto checkChild = [&](int32_t child) {
				if (parents_[child] == current) {
					const auto low = low_[child];
					const auto pre = pre_[current];
					const auto dp = dp_[child];
					if ((low >= pre) && (dp > 0)) 
						validTurretPlaces_[current] = false;
				}
			};

			validTurretPlaces_[current] = true;

			if (y < height - 1)
				checkChild(current + width);
			if (y > 0)
				checkChild(current - width);
			if (x < width - 1)
				checkChild(current + 1);
			if (x > 0)
				checkChild(current - 1);
		}
	}
}

void GridTowerPlacementOracle::updateCreepRestrictions()
{
	const int32_t width = levelInstance_.getLevel()->getWidth();
	const int32_t height = levelInstance_.getLevel()->getHeight();
	const int32_t tableSize = width * height;

	std::fill(occupiedByCreeps_.get(), occupiedByCreeps_.get() + tableSize, false);

	// Forbid placing on fields occupied by Creeps
	for (const auto & creep : levelInstance_.getCreeps()) {
		for (auto point : creep->getOccupiedTurretPositions()) {
			occupiedByCreeps_[point.y * width + point.x] = true;
		}
	}

	updateTowerRestrictions();
}
