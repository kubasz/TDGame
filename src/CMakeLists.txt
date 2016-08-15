set(SOURCES
	Bullet/Bullet.cpp
	Bullet/BulletDisplayComponent.cpp
	Bullet/BulletFactory.cpp
	Bullet/BulletMovementComponent.cpp
	Creep/Creep.cpp
	Creep/CreepDisplayComponent.cpp
	Creep/CreepFactory.cpp
	Creep/CreepLifeDisplayComponent.cpp
	Creep/CreepQueryService.cpp
	Creep/CreepWalkComponent.cpp
	Decoration.cpp
	Game.cpp
	GameStates/LevelGameState.cpp
	GameStates/LevelSelectGameState.cpp
	GameStates/MenuGameState.cpp
	Level.cpp
	LevelServices.cpp
	Tower/Tower.cpp
	Tower/TowerDisplayComponent.cpp
	Tower/TowerFactory.cpp
	Tower/TowerShootingComponent.cpp
	Tower/TowerTargetingComponent.cpp
)

set(LIBS
	${SFML_LIBRARIES}
	${SFGUI_LIBRARY}
	${THOR_LIBRARY}
)

add_executable(TDGame ${SOURCES})
target_link_libraries(TDGame ${LIBS})
