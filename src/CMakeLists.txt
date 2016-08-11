set(SOURCES
	Bullet.cpp
	Creep.cpp
	Game.cpp
	GameState.cpp
	HashedGridSelectionOracle.cpp
	Level.cpp
	LevelGameState.cpp
	LevelSelectGameState.cpp
	MenuGameState.cpp
	Tower.cpp
	TowerComponent.cpp
	TowerFactory.cpp
)

set(LIBS
	${SFML_LIBRARIES}
	${SFGUI_LIBRARY}
	${THOR_LIBRARY}
)

if (TDGAME_STATIC)
	set(LIBS "${LIBS} ${SFGUI_DEPENDENCIES}")
endif ()

add_executable(TDGame ${SOURCES})
target_link_libraries(TDGame ${LIBS})
